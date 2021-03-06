__version__ = 0.001
__abstract__ = True

__functions__ = [ ['EXP', 'exp'],
                ['LOG', 'log'],
                ['ROUND', 'roundup'],
                ['FLOOR', 'floor'],
                ['CEIL', 'ceil'],
                ['RANDF', 'randfloat'],
                ['RANDN', 'randnorm'],
                ['RANDI', 'randint'], 
                ['BINOMIAL', 'binomial'],
                ['CHISQUARE', 'chisquare'],
                ['DIRICHLET', 'dirichlet'],
                ['EXPONENTIAL', 'exponential'],
                ['GAMMA', 'gamma'],
                ['GEOMETRIC', 'geometric'],
                ['GUMBEL', 'gumbel'],
                ['LAPLACE', 'laplace'],
                ['LOGISTIC', 'logistic'],
                ['LOGNORMAL', 'lognormal'],
                ['NORMAL', 'normal'],
                ['PARETO', 'pareto'],
                ['POISSON', 'poisson'],
                ['POWER', 'power'],
                ['RAYLEIGH', 'rayleigh'],
                ['CAUCHY', 'cauchy'],
                ['STUDENTT', 'studentt'],
                ['UNIFORM', 'uniform'],
                ['VONMISES', 'vonmises'],
                ['WALD', 'wald'],
                ['WEIBULL', 'weibull'],
                ['ZIPF', 'zipf'],
                ['GREATEREQ', 'greatereq'],
                ['LESSEQ', 'lesseq'],
                ['LESS', 'less'],
                ['GREATER', 'greater'],
                ['EQUAL', 'equal'],
                ['NOTEQUAL', 'notequal']]
                
from abc import ABCMeta, abstractmethod
import numpy as np
from BSException import BSException as BSException 
import Evaluate as Evaluate
import copy


class BrainStudioObject() :

    __metaclass__ = ABCMeta
  
    def __init__(self) :
        self.class_name = '__UNDEFINED__'
        self.configurations = dict()
        self.parameters = []
        self.states=[]
        self.attributes = []
        self.fields = []
        self.namespace = ''
        self.name = '__UNNAMED__'
        self.architecture = 'Python'
     
    def set_class_name(self, class_name) :
        self.class_name = class_name

    def get_class_name(self) :
        return self.class_name
        
    def get_name(self):
        return self.name

    def set_name(self, node) :
        try:
            self.name = node.get('id') #.find('name').text
        except:
            where = self.get_where()
            what = 'Can not find name in brn file'
            raise BSException(where, what)

    def set_namespace(self, namespace) :
        self.namespace = namespace  
        
    def get_configurations(self):
        return self.configurations
        
    def set_attributes(self, parameters, states):
       self.parameters=parameters
       self.states=states
       self.attributes = self.parameters + self.states
       
    def get_fields(self):
        return self.fields
        
    def get_architecture(self):
        return self.architecture;

    def get_help(self, object_type = ''):
        return "No help available.";
        
    def get_version(self):
        return __version__
        
    def check_version(self, node):
        try:
            v= float(node.find('version').text)
        except:
            where = self.get_where()
            what = 'Can not find version in brn file'
            raise BSException(where, what)
        if v > self.get_version():
            where = self.get_where()
            what = 'Requires version ' + node.find('version').text + ' backend only has version %f' %self.get_version()
            raise BSException(where, what)
 
    def get_all_data(self):
        return list(np.array())
        
    def get_where(self):
        return self.class_name + ' -> ' + self.namespace + '::' + self.name
        
    def configure_static(self) :  
        pass
        
    def initialize(self, brain, node, node_args):
        dave=1
            
    def safely_get(self, node, fieldname, fieldtype):
        where = self.get_where()
        what = 'Error can not find field ' + fieldname
        try:
            x = node.find(fieldname).text
            what = 'Error evaluating field ' + fieldname + ': ' + x
            
            if fieldtype == 'string':
                value = str(x)
            elif fieldtype == 'int':
                value = int(x)
            elif fieldtype == 'float':
                value = float(x)
            elif fieldtype == 'bool':
                if x == 'True':
                    value = True
                else:
                    value = False
            elif fieldtype == 'intlist':
                value = []
                lst = x.split(",")
                for l in lst:
                    value.append(int(l))
            elif fieldtype == 'floatlist':
                value = []
                lst = x.split(",")
                for l in lst:
                    value.append(float(l))
        except:
            raise BSException(where, what)
            
        return value
            
            
    def construct_formula_single(self, formula):
        for f in __functions__ :
            formula = formula.replace(f[0], f[1])
        return formula
       
    def construct_formulas(self, node) :
        sorted_attributes = copy.copy(self.attributes);
        sorted_attributes.sort(cmp=by_length)
                
        values = dict();
        replace_list = [];
        rcount = 0;
        formulas = dict()    
        for a in self.attributes:
            
            formula = node.find(a).text
            newformula = formula
            
            for sa in sorted_attributes:
                
                
                ra = "REPLACE%d" % rcount
                replace_list.append(ra)
                
                attr = next((x for x in self.parameters if x==sa), '_NOT_FOUND_')            
                if attr==sa:
                    values[ra] = "PARAS['" + sa + "']"
                else:
                    values[ra] = "STATES['" + sa + "']"
    
                rcount = rcount +1                    
                newformula = newformula.replace(sa, ra)
    
                        
            replace_list.sort(cmp=by_length)
            for ra in replace_list:
                value = values[ra]
                newformula = newformula.replace(ra, value)
    
            for f in __functions__ :
                newformula = newformula.replace(f[0], f[1])        
                
            formulas[a] = newformula
        
        return formulas
        
    def evaluate_formulas(self, formulas, node, attributes, PARAS, STATES):
        Evaluate.size = 1
        for a in attributes:
            formula = formulas[a]

            try:          
                result = Evaluate.evaluate(formula, PARAS, STATES)            
            
            except:
                where = self.get_where()
                what = 'Error evaluating expression: ' + a + ' = ' + node.find(a).text + ' -> ' + formula
                raise BSException(where, what)
                  
            attr = next((x for x in self.parameters if x==a), '_NOT_FOUND_')
            if attr==a:
                PARAS[a] = result
            else:
                STATES[a] = result 
                  
        return (PARAS, STATES)

    def evaluate_formulas_batch(self, formulas, node, attributes, PARAS, STATES, num):
        Evaluate.size = num
        for a in attributes:
            formula = formulas[a]

            try:          
                result = Evaluate.evaluate(formula, PARAS, STATES)            
            
            except:
                where = self.get_where()
                what = 'Error evaluating expression: ' + a + ' = ' + node.find(a).text + ' -> ' + formula
                raise BSException(where, what)
                  
            attr = next((x for x in self.parameters if x==a), '_NOT_FOUND_')
             
            if (not isinstance(result, list)) and ( not isinstance(result, np.ndarray)):
                result = [result]*num    
           
            if attr==a:
                PARAS[a] = result
            else:
                STATES[a] = result 

        return (PARAS, STATES)

    def row(self):
        where = self.get_where()
        what = 'Error no function ROW for this object type'
        raise BSException(where, what)
          
    def col(self):
        where = self.get_where()
        what = 'Error no function COL for this object type'
        raise BSException(where, what)
        
   
    
    
def by_length(word1,word2):
    return len(word2)-len(word1)  
    
