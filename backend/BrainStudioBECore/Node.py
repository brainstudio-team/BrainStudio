__version__ = 0.001
__abstract__ = True

from ProcessUnit import ProcessUnit as ProcessUnit
from abc import abstractmethod
import numpy as np

class Node(ProcessUnit) :
    
    def __init__(self) :
        super(Node,self).__init__()
        self.model_type = 'rate'
        self.input_field = ''
        self.output_field = ''
        self.units_field = ''
        self.sources = []
        
    def get_version(self):
        return __version__
        
    def get_model_type(self, architecture = ''):
        return self. model_type
        
    def get_input_field(self, architecture = ''):
        return self.input_field

    def get_output_field(self, architecture = ''):
        return self.output_field
   
    def get_units_field(self):
        return self.units_field
        
    def get_where(self):
        return 'Node ' + self.class_name + ' -> ' + self.namespace + '::' + self.name
        
    def add_source(self, source):
        self.sources.append(source)
        
    def get_stim(self):
        return np.array()
    
    @abstractmethod
    def get_input_size(self):
        pass
          
    @abstractmethod
    def get_output_size(self):
        pass
    
    @abstractmethod
    def get_data(self, args):
        pass
    
    @abstractmethod
    def set_data(self, args):
        pass
    
    def update(self):     
        args = dict()
        for s in self.sources:
            s.transfer_data(args)  
    
    def get_start_neuron(self):
        return 0
        
    def set_fired(self, fired):
        pass  
   
