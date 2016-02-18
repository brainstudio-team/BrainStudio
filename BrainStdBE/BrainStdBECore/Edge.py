__version__ = 0.001
__abstract__ = True

from BrainStudioObject import BrainStudioObject as BrainStudioObject
from BSException import BSException as BSException 

class Edge(BrainStudioObject) :
    
    def __init__(self) :
        super(Edge,self).__init__()
        self.source = -1
        self.target = -1 
        self.input_model = 'rate'
        self.output_model = 'rate'
   
    def get_version(self):
        return __version__
        
    def get_where(self):
        return 'Edge ' + self.class_name + ' -> ' + self.namespace + '::' + self.name
       
    def get_input_model(self):
        return self.input_model
    
    def get_output_model(self):
        return self.output_model
    
    def initialize(self, brain, node, args):
        super(Edge,self).initialize(brain, node, args)
        where = self.get_where()
        source = self.safely_get(node, 'source', 'string')
        target = self.safely_get(node, 'target', 'string')
        where = self.get_where()  
          
        try:       
            self.source = brain.get_node_by_name(source)
        except BSException:  
            what = 'Error: can not find source node: ' + source
            raise BSException(where, what)
        try:            
            self.target = brain.get_node_by_name(target)
        except BSException:
            print 'dave'
            what = 'Error: can not find target node: ' + target
            raise BSException(where, what)
            
        self.target.add_source(self)
    
    def get_connection_ranges(self, node):
       
        self.preFirst = self.safely_get(node, 'preFirst', 'int')
        self.preLast = self.safely_get(node, 'preLast', 'int')
        self.postFirst = self.safely_get(node, 'postFirst', 'int')
        self.postLast = self.safely_get(node, 'postLast', 'int')
        
        self.inputs = self.preLast-self.preFirst+1
        self.outputs = self.postLast-self.postFirst+1
        
        where = self.get_where()
                    
        if self.preFirst < 0  or self.preLast > self.source.get_output_size()-1:
            prerange = self.preFirst, self.preLast,  self.source.get_output_size()
            what = 'Error input out of range: ' + str(prerange)
            raise BSException(where, what)
            
        if self.postFirst < 0  or self.postLast > self.target.get_input_size()-1:
            postrange = self.postFirst , self.postLast, self.target.get_input_size()
            what = 'Error ouput out of range: ' + str(postrange)
            raise BSException(where, what)
            
