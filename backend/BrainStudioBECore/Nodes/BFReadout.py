__version__ = 0.001
__abstract__ = False

from BrainStudioBECore.Node import Node as Node
from BrainStudioBECore.BSException import BSException as BSException
import numpy as np
from numpy import random as rn


class BrainStudioBEClass(Node) :
    
    def __init__(self) :
        """
        This method acts as a constructor and is run every time a node of
        this type is instantiated.
        """
        super(BrainStudioBEClass,self).__init__()
        self.configurations['BFReadout'] = ([], [], [], []) 
        self.fields.append(['neurons', 'Neurons','integer', '1', ''])
        self.fields.append(['readouts', 'Readouts','integer', '1', ''])
        self.fields.append(['alpha', 'Alpha','float', '0.99', ''])
        self.fields.append(['learning_rate', 'Learning rate','float', '0.1', ''])
        self.input_field = 'neurons'
        self.output_field = 'neurons'
        self.units_field = 'neurons'
        self.size = -1
        

    def get_input_size(self):
        """
        This method returns the size of the arrays the node will be accepting
        when set_data() is called.
        """
        return self.size
    

    def get_output_size(self):
        """
        This method returns the size of the arrays the node will be returning
        when get_data() is called.
        """
        return self.nb_readouts

        
    def get_all_data(self):
        return list(self.outputs)

        
    def get_data(self, args):
        """
        This method returns the output of the node.
        
        If there's any operation on the input the node should do each timestep
        this is a good place to do it.
        """
        if self.event_next:
            if not np.isnan(self.max_idx):
                print "BFReadout selected readout: ", self.max_idx
            else:
                print "BFReadout had event, but there was no readout above threshold."

        return 1.0*(self.outputs > self.output_threshold)
        

    def set_data(self, args):
        """
        This method sets the input to the node.

        Inputs:
        args  --  Dictionary with various input fields. The values of the
                  input itself are in args['inputs'].
        """

        first_neuron = args['first_neuron']
        last = args['last']

        # TODO: FIXME: I don't know how to do this, but the max_idx should
        # only be retained for one timestep
        self.max_idx = np.nan

        # Inputs from the SBFNode
        if args.has_key('inputs'):
            self.inputs = args['inputs']

            # When the system is learning, the weights to the readout that is
            # most active at the time move slightly in the direction of the
            # input.
            if self.learn_next or self.event_next:

                activities = 1.0*(np.cos(self.inputs) > self.alpha)
                self.outputs = 1.0*(self.weights.dot(activities) > self.activity_threshold)

                # Output selection for Hedderik's multi-readout model
                if any(self.outputs > self.output_threshold):
                    self.max_idx = np.argmax(self.outputs)
                    if self.learn_next:
                      self.weights[self.max_idx,:] = self.weights[self.max_idx,:] + self.learning_rate*activities

                self.learn_next = False
                self.event_next = False

        # Inputs from the ACT-R interface        
        if args.has_key('actr_commands'): 
            if 'learn' in args['actr_commands']:
                print "========= BF LEARN ======================="
                self.learn_next = True
            if 'event' in args['actr_commands']:
                print "========= BF EVENT ======================="
                self.event_next = True
                self.learn_next = True  # I assume that when there's an event we also learn


    def initialize(self, brain, node, args):
        super(BrainStudioBEClass,self).initialize(brain, node, args)
        self.size = self.safely_get(node, 'neurons', 'int') 
        self.nb_readouts = self.safely_get(node, 'readouts', 'int') 
        self.inputs = np.zeros(self.size)
        self.outputs = np.zeros(self.nb_readouts)
        self.alpha = self.safely_get(node, 'alpha', 'float')
        self.learning_rate = self.safely_get(node, 'learning_rate', 'float')

        self.weights = 0.8*rn.randn(self.nb_readouts, self.size)
        self.learn_next = False
        self.event_next = False
        self.max_idx = np.nan
        self.activity_threshold = 0.75*self.size*np.arccos(self.alpha)/np.pi
        self.output_threshold = 1  # FIXME: I have no idea of what should this be
        
        return self.size


    def update(self):
        super(BrainStudioBEClass,self).update()

        activities = 1.0*(np.cos(self.inputs) > self.alpha)
        self.outputs = 1.0*(self.weights.dot(activities) > self.activity_threshold)

