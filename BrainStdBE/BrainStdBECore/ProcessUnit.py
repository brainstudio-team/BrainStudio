__version__ = 0.001
__abstract__ = True

from BrainStudioObject import BrainStudioObject as BrainStudioObject
from abc import abstractmethod


class ProcessUnit(BrainStudioObject) :
    
    def __init__(self) :
        super(ProcessUnit,self).__init__()
        self.update_time = 0

    def get_version(self):
        return __version__
        
    def get_update_time(self):
        return self.update_time
                
     
        
 