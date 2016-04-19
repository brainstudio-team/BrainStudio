__version__ = 0.001
__abstract__ = False

from BrainStudioBECore.Node import Node as Node
from BrainStudioBECore.BSException import BSException as BSException
import numpy as np


class BrainStudioBEClass(Node) :
    
    def __init__(self) :
        super(BrainStudioBEClass,self).__init__()
        self.configurations['SBFReadout'] = ([], [], [], []) 
        self.fields.append(['neurons', 'Neurons','integer', '1', ''])
        self.fields.append(['threshold', 'Threshold','float', '100.0', ''])
        self.input_field = 'neurons'
        self.output_field = 'neurons'
        self.units_field = 'neurons'
        self.size = -1
        
    def get_input_size(self):
        return self.size
    
    def get_output_size(self):
        return 1
        
    def get_all_data(self):
        return list(self.outputs)

        
    def get_data(self, args):
        return 1.0*(self.outputs[args['first_neuron']] > self.threshold)
        

    def set_data(self, args):
        print "Enter SBFReadout::setData"

        first_neuron = args['first_neuron']
        last = args['last']
        # for n in range(first_neuron, last+1):
        #     self.inputs[n] += inputs[n-first_neuron]

        # Inputs from the SBFNode
        if args.has_key('inputs'):
            self.inputs = args['inputs']

        # Inputs from the ACT-R interface        
        if args.has_key('actr_commands'): 
            if 'learn' in args['actr_commands'] and args.has_key('inputs'):
                self.weights = np.cos(inputs)


        print "Finish SBFReadout::setData"


    def initialize(self, brain, node, args):
        super(BrainStudioBEClass,self).initialize(brain, node, args)
        self.size = self.safely_get(node, 'neurons', 'int') 
        self.inputs = np.zeros(self.size)
        self.outputs = np.zeros(1)
        self.threshold = self.safely_get(node, 'threshold', 'float')

        self.weights = np.zeros(self.size)
        
        return self.size
            
    def update(self):
        super(BrainStudioBEClass,self).update()
    
        self.outputs = self.weights*np.cos(self.inputs)
        
