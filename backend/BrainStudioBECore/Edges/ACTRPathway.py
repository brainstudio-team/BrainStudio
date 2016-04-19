__version__ = 0.001
__abstract__ = False


from BrainStudioBECore.WeightedPathway import WeightedPathway as WeightedPathway
from BrainStudioBECore.BSException import BSException as BSException 
import numpy as np

class BrainStudioBEClass(WeightedPathway) :
    
    def __init__(self) :
        super(BrainStudioBEClass,self).__init__()
        self.configurations['ACTRPathway'] = (self.pathway_parameters, [], self.pathway_parameters_default, [])
        
        self.input_model = 'rate'
        self.output_model = 'rate'
        self.size=0;
   
    def get_version(self):
        return __version__
        
    def initialize(self, brain, node, args):  
        super(BrainStudioBEClass,self).initialize(brain, node, args)
        
    def add_synapses(self, brain, sources, targets, synapse_args):
        pass
            
    def get_all_data(self):
        return np.array([])
        
    def transfer_data(self, args):       
        args = {'first_neuron' :  self.preFirst, 'last' : self.preLast }
        inputs = self.source.get_data(args)


        args = {'first_neuron' :  self.postFirst, 'last' : self.postLast }
        if inputs.has_key('actr_commands'):
            args['actr_commands'] = inputs['actr_commands']

        print "AAA", inputs, args

        self.target.set_data(args)

