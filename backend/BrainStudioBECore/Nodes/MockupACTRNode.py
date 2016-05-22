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
        print "get_data()", self.sync, self.learn
        outDict = {'actr_commands': []}
        if self.sync > 0 :
            self.transfering = False
            print "========= EMIT SYNC ======================="
            outDict['actr_commands'].append('sync')
            
        if self.learn > 0 :
            self.transfering = False
            print "========= EMIT LEARN ======================="
            outDict['actr_commands'].append('learn')
        return outDict
        
    def set_data(self, args):
        print "set_data()", args
        if 'sync' in args :
            self.sync = args['sync']
            if self.sync > 0 :
                self.transfering = True
        elif 'learn' in args :
            self.learn = args['learn']
            if self.learn > 0 :
                self.transfering = True
        # This thing should come from the real ACT-R
        pass

        
    def initialize(self, brain, node, args):
        super(BrainStudioBEClass,self).initialize(brain, node, args)
        self.size = self.safely_get(node, 'neurons', 'int') 
        self.inputs = np.zeros(self.size)
        self.outputs = np.zeros(self.size)

        self.sync = self.safely_get(node, 'sync', 'int')
        self.learn = self.safely_get(node, 'learn', 'int')
        
        # ZAF: This is not a 100% correct solution because the transfer between 
        # synapses occurs along with the update. So one 'target' node can be
        # processed before this and one after this!
        # As a result, the new events do not always pass to the target nodes!
        # TODO: Create a different function that processes synaptic transfers
        #       and a different one for the update of the node equations
        #       i.e. keep the update() almost as it is and create an
        #       update_connections() one.
        self.transfering = False
        
        return self.size
            
    def update(self):
        print "update()", self.sync, self.learn
        if not self.transfering and self.sync > 0 :
            self.sync = 0
        if not self.transfering and self.learn > 0 :
            self.learn = 0

        super(BrainStudioBEClass,self).update()

        # Wait for actual ACT-R stuff 
        
