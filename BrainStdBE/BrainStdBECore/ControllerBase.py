__version__ = 0.001
__abstract__ = True


from abc import ABCMeta, abstractmethod

class Controller() :

    __metaclass__ = ABCMeta
    
    def __init__(self) :
        self.class_name = '__UNDEFINED__'
        
    def set_classname(self, class_name) :
        self.class_name = class_name
    
    @abstractmethod    
    def execute(self, argv) :
        pass


        
  