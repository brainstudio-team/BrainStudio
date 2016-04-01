__version__ = 0.001
__abstract__ = False

from BrainStudioBECore.WeightedPathway import WeightedPathway as WeightedPathway
from BrainStudioBECore.BSException import BSException as BSException
import numpy as np
import exceptions

class BrainStudioBEClass(WeightedPathway) :
    
    def __init__(self) :
        super(BrainStudioBEClass,self).__init__()
        self.pathway_parameters.append('delay')
        self.pathway_parameters.append('plastic')
        self.pathway_parameters_default.append('RANDI(1,60)')
        self.pathway_parameters_default.append('False')
        self.configurations['NeMoSynapticPathway'] = (self.pathway_parameters, [], self.pathway_parameters_default, [])
        self.size = 0
        
        self.architecture = 'NeMo'
        self.input_model = 'spike'
        self.output_model = 'spike'
   
    def get_version(self):
        return __version__
        
    def initialize(self, brain, node, args):
        super(BrainStudioBEClass,self).initialize(brain, node, args)
        self.brain = brain
        return self.size
        
    def add_synapses(self, brain, sources, targets, paras):
        delays = paras[0]['delay']
        weights = paras[0]['weight']
        plastic = paras[0]['plastic'] 
   
        t = np.array(targets)+self.target.get_start_neuron()
        s = np.array(sources)+self.source.get_start_neuron()
 

        try:            
            brain.add_nemo_synapse(s.tolist(), t.tolist(), delays, weights, plastic)
        except exceptions.BaseException as e:
            where = self.get_where()
            what = 'Error in NeMo adding synapses: ' + e.args[0]
            raise BSException(where, what)
       
        self.size += len(targets)

    def get_all_data(self):
        weights=list()        
        for s in self.synapse:
            indexes = range(s[1],s[1]+s[2])
            weights.append(self.brain.get_nemo_synapse_weight(indexes))
        return weights      
        
    def transfer_data(self, args):
        return


    
        
        
 
