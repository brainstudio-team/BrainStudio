__version__ = 0.001
__abstract__ = False

from BrainStudioBECore.Node import Node as Node
from BrainStudioBECore.BSException import BSException as BSException
import numpy as np


class BrainStudioBEClass(Node) :
    


    def __init__(self) :
        super(BrainStudioBEClass,self).__init__()
        self.fields.append(['neurons', 'Neurons','integer', '1', ''])
        self.configurations['MockupACTRNode'] = ([], [], [], []) 
        self.input_field = 'neurons'
        self.output_field = 'neurons'
        self.units_field = 'neurons'
        self.size = -1
        
    def get_input_size(self):
        return self.size
    
    def get_output_size(self):
        return self.size
        
    def get_all_data(self):
        return list(self.outputs)
        
    def get_data(self, args):

        # first_neuron = args['first_neuron']
        # last = args['last']
        # outputs = self.outputs[first_neuron: last+1]            
        # return outputs

        # This thing should come from the real ACT-R
        outDict = {'actr_commands': []}
        outDict['actr_commands'].append('learn')
        outDict['actr_commands'].append('sync')
        return outDict
        
    def set_data(self, args):
        # This thing should come from the real ACT-R
        pass

        
    def initialize(self, brain, node, args):
        super(BrainStudioBEClass,self).initialize(brain, node, args)
        self.size = self.safely_get(node, 'neurons', 'int') 
        self.inputs = np.zeros(self.size)
        self.outputs = np.zeros(self.size)
        #self.update_time = self.safely_get(node, 'update_time', 'integer')
        
        return self.size
            
    def update(self):
        super(BrainStudioBEClass,self).update()

        # Wait for actual ACT-R stuff 
        
