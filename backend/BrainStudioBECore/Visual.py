__version__ = 0.001
__abstract__ = True

from BrainStudioBECore.Node import Node as Node
from BrainStudioBECore.BSException import BSException as BSException
from abc import abstractmethod
import numpy as np
import io

class Visual(Node) :
    

    def __init__(self) :
        super(Visual,self).__init__()
 
	self.architecture = 'Visual'
        self.fields.append(['units', 'Units','integer', '1', ''])
        self.fields.append(['update_time', 'Update time','integer', '', ''])
        self.input_field = 'units'
        self.units_field = 'units'
        self.size = -1

	self.buffer = io.BytesIO()
	self.buffer_size = 0;

    def get_input_size(self):
        return self.size
    
    def get_output_size(self):
        return 0
        
    def get_all_data(self):
        return list(self.inputs)
        
    def get_data(self, args):
        first_neuron = args['first_neuron']
        last = args['last']
        return self.inputs[first_neuron: last+1]   
        
    def set_data(self, args):
        first_neuron = args['first_neuron']
        last = args['last']
	inputs = np.array(args['inputs'])
        for n in range(first_neuron, last+1):
	    self.inputs[n] += inputs[n-first_neuron]

    @abstractmethod
    def get_visual_type(self):
	pass

    def get_buffer_size(self):
	return self.buffer_size

    def get_visual_buffer(self):
	return self.buffer
        	
	
    def initialize(self, brain, node, args):
        super(Visual,self).initialize(brain, node, args)
        self.size = self.safely_get(node, 'units', 'int') 
        self.inputs = np.zeros(self.size)
        #self.update_time = self.safely_get(node, 'update_time', 'integer')
        
        return self.size
            
    def update(self):
        super(Visual,self).update()

        
