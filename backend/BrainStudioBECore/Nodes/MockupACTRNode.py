__version__ = 0.001
__abstract__ = False

from BrainStudioBECore.Node import Node as Node
from BrainStudioBECore.BSException import BSException as BSException
import numpy as np


class BrainStudioBEClass(Node) :
    
    def __init__(self) :
        super(BrainStudioBEClass,self).__init__()
        self.configurations['MockupACTRNode'] = ([], [], [], []) 
        self.fields.append(['neurons', 'Neurons','integer', '1', ''])
        self.fields.append(['sync', 'SYNC ACT-R command','integer', '0', '1'])
        self.fields.append(['learn', 'LEARN ACT-R command','integer', '0', '1'])
        self.input_field = 'neurons'
        self.output_field = 'neurons'
        self.units_field = 'neurons'
        self.size = 1
        
    def get_input_size(self):
        return self.size
    
    def get_output_size(self):
        return self.size
        
    def get_all_data(self):
        return list(self.outputs)
        
    def get_data(self, args):
        outDict = {'actr_commands': []}
        if self.sync > 0 :
            self.sync = 0
            print "========= EMIT SYNC ======================="
            outDict['actr_commands'].append('sync')
            
        if self.learn > 0 :
            self.learn = 0
            print "========= EMIT LEARN ======================="
            outDict['actr_commands'].append('learn')
        return outDict
        
    def set_data(self, args):
        if 'sync' in args :
            self.sync = args['sync']
        elif 'learn' in args :
            self.learn = args['learn']
        # This thing should come from the real ACT-R
        pass

        
    def initialize(self, brain, node, args):
        super(BrainStudioBEClass,self).initialize(brain, node, args)
        self.size = self.safely_get(node, 'neurons', 'int') 
        self.inputs = np.zeros(self.size)
        self.outputs = np.zeros(self.size)

        self.sync = self.safely_get(node, 'sync', 'int')
        self.learn = self.safely_get(node, 'learn', 'int')
        
        return self.size
            
    def update(self):
        super(BrainStudioBEClass,self).update()

        # Wait for actual ACT-R stuff 
        
