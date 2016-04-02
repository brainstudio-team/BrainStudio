__version__ = 0.002
__abstract__ = False

from BrainStudioBECore.Node import Node as Node
from BrainStudioBECore.BSException import BSException as BSException
import numpy as np
import nemo
import exceptions
import os
import sys
import regex as re

class BrainStudioBEClass(Node) :
    
    added_classes = []
    
    def __init__(self) :
        super(BrainStudioBEClass,self).__init__()
        self.architecture = 'NeMo'
        self.model_type ='spike'
        self.input_field = 'neurons'
        self.output_field = 'neurons'
        self.units_field = 'neurons'
             
        self.fields.append(['neurons', 'Neurons','integer', '1', ''])
        self.fields.append(['eval_for_each', 'Evaluate formula for each neuron', 'bool', 'False'])

        
        self.start_neuron = -1
        self.size = -1
        self.neuron_type_id = -1
        
        self.fired = []

    def configure_static(self) :
        
        BrainStudioBEClass.added_classes = []
        
        print 'configuring NeMo neuron types'

        # PEDRO: TODO: if we want to support Windows we need to change the
        # definition of paths (e.g. change the file separator)
        ini_path = nemo.__nemo_path__ + '/lib/nemo/plugins/'
    
        for file in os.listdir(ini_path):
            if file.endswith('ini'):
                neuron_type = file[0:-4]
                handle = open(ini_path+file)
                text = handle.readlines()
                handle.close()
            
                isParameter = False
                isStateName = False
                parameters = []
                states = []
                default_parameters = dict()
                default_states = []
                paramRegex = re.compile('\[params:(?P<name>[A-Za-z]+)\]')
                paramSetName = ''
                for l in text: 
                    
                    l = l.rstrip("\r")                        
                    l = l.rstrip("\n")
                    
                    # First check if the line is empty, commented or has a
                    # section title (i.e. square brackets)
                    if l == '' or l[0] == '#':
                        continue
                    elif paramRegex.match(l):
                        m = paramRegex.match(l)
                        paramSetName = m.group('name')
                        if len(paramSetName) == 0:
                            sys.exit("Error while reading", file, ".ini file."\
                                   + "Parameter set name cant be empty. ABORT")
                        if not default_parameters.has_key(paramSetName):
                            default_parameters[paramSetName] = []
                        isParameter= True
                        isStateName = False
                    elif l == '[state_names]':
                        isStateName = True
                        isParameter = False

                    # If it isn't, then parse as normal text
                    else:
                        left = l[0:l.rfind('=')]
                        left = left.strip()
                        right =  l[l.rfind('=')+1:len(l)]
                        right = right.strip()
                        
                        if isParameter:
                            default_parameters[paramSetName].append(right)
                            if left not in parameters:
                                parameters.append(left)
                        elif isStateName:
                            states.append(left)   
                            default_states.append(right)           

                self.configurations[neuron_type] = (parameters, states, default_parameters, default_states)
    
    def get_version(self):
        return __version__
        
    def get_model_type(self, architecture = ''):
        if architecture == 'Kuramoto-NeMo':
            return 'rate'
        else:
            return self.model_type
        
    def get_input_field(self, architecture = ''):
        if architecture == 'Poisson-NeMo' or architecture == 'Input-NeMo':
            return ""
        else:
            return self.input_field

    def get_output_field(self, architecture = ''):
        return self.output_field

        
    def get_input_size(self):
        return self.size
    
    def get_output_size(self):
        return self.size
        
    def get_start_neuron(self):
        return self.start_neuron
        
    def set_fired(self, fired):
        self.fired = fired
        
    def get_stim(self):
        return self.stim
        
    def clear_stim(self):
         self.stim = []
        
    def get_data(self, args):
        first_neuron = args['first_neuron']
        last = args['last']
        if self.class_name == 'Kuramoto-NeMo': 
            return self.brain.get_nemo_neurons_state(range(self.get_start_neuron()+first_neuron, self.get_start_neuron()+last+1), 0)    
        else:

            outputs = np.zeros(last-first_neuron+1)
           
            n=0
            while n < len(self.fired) and self.fired[n] < first_neuron:
                n += 1
            
            while n < len(self.fired) and self.fired[n] <=last:
                outputs[self.fired[n]] = 1
                n += 1
            
        return outputs
        
    def get_all_data(self):
        if self.class_name == 'Kuramoto-NeMo': 
            return self.brain.get_nemo_neurons_state(range(self.get_start_neuron(), self.get_start_neuron()+self.size), 0)
        else:
            return self.fired

        
    def set_data(self, args):
        inputs = args['inputs']
        first_neuron = args['first_neuron']
        last = args['last']
        # PEDRO: this is quite inefficient. We could vectorize this loop.
        for n in range(first_neuron, last+1):
            self.stim.append((n+self.get_start_neuron(),inputs[n-first_neuron]))
        
    def initialize(self, brain, node, args):
        super(BrainStudioBEClass,self).initialize(brain, node, args)
        
        self.brain = brain
        
        if self.class_name == 'Kuramoto-NeMo': 
            self.model_type ='rate'
         
        self.start_neuron = args['start_neuron']
        
        self.size = self.safely_get(node, 'neurons', 'int')
        
        where = self.get_where()  
        cn =  self.class_name[0:self.class_name.find('-')]
        if self.class_name in BrainStudioBEClass.added_classes:
            self.neuron_type_id = brain.get_nemo_neuron_type(cn)
        else:         
            try:
                self.neuron_type_id = brain.add_nemo_neuron_type(cn)
                BrainStudioBEClass.added_classes.append(self.class_name)
            except exceptions.BaseException as e:
                what = 'Error in NeMo adding neuron type: ' + e.args[0]
                raise BSException(where, what)
            
        self.eval_for_each = self.safely_get(node, 'eval_for_each', 'bool')
        formulas = self.construct_formulas(node)
        
        
        if self.eval_for_each  == False:
            neuron_args = self.evaluate_formulas(formulas, node, self.attributes, dict(), dict())
            for n in range(self.size): 
                try:   
                    brain.add_nemo_neuron(self.neuron_type_id, neuron_args[0], neuron_args[1], 1);
                except exceptions.BaseException as e:
                    what = 'Error in NeMo adding neuron parameter/state:' + e.args[0]
                    raise BSException(where, what)
            
        else:    
            neuron_args = self.evaluate_formulas_batch(formulas, node, self.attributes, dict(), dict(), self.size)                                
            try:            
                brain.add_nemo_neuron(self.neuron_type_id, neuron_args[0], neuron_args[1], self.size);
            except exceptions.BaseException as e:
                what = 'Error in NeMo adding neuron parameter/state: ' + e.args[0]
                raise BSException(where, what)
        self.stim = []

        return self.size
            
    def update(self):
        super(BrainStudioBEClass,self).update()
        
