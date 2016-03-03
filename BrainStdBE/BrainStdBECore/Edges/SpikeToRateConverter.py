__version__ = 0.002
__abstract__ = False

from BrainStdBECore.WeightedPathway import WeightedPathway as WeightedPathway
from BrainStdBECore.BSException import BSException as BSException 
import numpy as np

class BrainStdBEClass(WeightedPathway) :
    
    def __init__(self) :
        super(BrainStdBEClass,self).__init__()
        self.configurations['SpikeToRateConverter'] = (self.pathway_parameters, [], self.pathway_parameters_default, [])
        self.fields.append(['window', 'Window size','integer', '1', ''])
        
        self.input_model = 'spike'
        self.output_model = 'rate'
        self.size = 0
   
    def get_version(self):
        return __version__
        
    def initialize(self, brain, node, args):        
        super(BrainStdBEClass,self).initialize(brain, node, args)

        self.window = self.safely_get(node, 'window', 'int')      
        self.buffer = np.zeros((self.inputs,self.window))
        self.index = 0
 
    def add_synapses(self, brain, sources, targets, synapse_args):
        if self.size==0:
            self.weights = np.matrix(np.zeros((self.inputs, self.outputs)))

        for count in range(0,len(targets)):
            s=sources[count]-self.preFirst
            t=targets[count]-self.postFirst
            self.weights[s, t] = synapse_args[0]['weight'][count]
    
        
        self.size += len(targets)
                
    def get_all_data(self):
        return list(self.weights)
            
    def transfer_data(self, args): 
        args = {'first_neuron' :  self.preFirst, 'last' : self.preLast }
        self.buffer[:, self.index] = self.source.get_data(args)
        if self.index == self.window:
            self.index = 0
          
        inputs = np.sum(self.buffer, axis=1)/self.window
        
        outputs = np.matrix(inputs)*self.weights
        outputs = outputs.tolist()
        outputs = outputs[0]   
        
        args = {'inputs' : outputs, 'first_neuron' :  self.postFirst, 'last' : self.postLast }
        self.target.set_data(args)
        

        
        
 
