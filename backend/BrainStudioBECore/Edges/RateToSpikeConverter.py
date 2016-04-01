__version__ = 0.002
__abstract__ = False

from BrainStudioBECore.WeightedPathway import WeightedPathway as WeightedPathway
from BrainStudioBECore.BSException import BSException as BSException 
import numpy as np

class BrainStudioBEClass(WeightedPathway) :
    
    def __init__(self) :
        super(BrainStudioBEClass,self).__init__()
        self.configurations['RateToSpikeConverter'] = (self.pathway_parameters, [], self.pathway_parameters_default, [])
        
        self.input_model = 'rate'
        self.output_model = 'spike'
        self.size=0;
   
    def get_version(self):
        return __version__
        
    def initialize(self, brain, node, args):        
        super(BrainStudioBEClass,self).initialize(brain, node, args)
            
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
        inputs = self.source.get_data(args)      
        
        outputs = np.matrix(inputs)*self.weights
        outputs = outputs.tolist()
        outputs = outputs[0]        
                
        
        args = {'inputs' : outputs, 'first_neuron' :  self.postFirst, 'last' : self.postLast }       
        self.target.set_data(args)

    


        
        
 
