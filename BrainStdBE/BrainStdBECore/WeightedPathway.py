__version__ = 0.001
__abstract__ = True

from Edge import Edge as Edge
from BSException import BSException as BSException 
import Evaluate as Evaluate
import random as random
import numpy as np
from abc import abstractmethod

class WeightedPathway(Edge) :
    
    def __init__(self) :
        super(WeightedPathway,self).__init__()
        self.fields.append(['__connectivity', 'Connectivity pattern','picklist', ['full','probability','topographic','formula','file'], 'full'])
        self.fields.append(['__probability', 'Connection probability','float', '0', '1'])
        self.fields.append(['__formula', 'Connection formula','string', 'LESSEQ(RANDF(), 0.05, 1, 0)'])
        self.fields.append(['__file', 'Connectivity file','string', ''])
        
        self.pathway_parameters = ['weight']
        self.pathway_parameters_default = ['RANDF()']
   
    def get_version(self):
        return __version__
        
    def initialize(self, brain, node, args):        
        super(WeightedPathway,self).initialize(brain, node, args)
        self.get_connection_ranges(node)
                
        connectivity = self.safely_get(node, '__connectivity', 'string')
        

        if connectivity == 'probability':
            probability = self.safely_get(node, '__probability', 'float')
        elif connectivity == 'formula':
            distribution = self.safely_get(node, '__formula', 'string')
            formula = self.construct_formula_single(distribution)
        elif connectivity == 'file':
            from_file = self.safely_get(node, '__file', 'string')
            print './files/'+from_file
            try:
                conn_matrix = np.loadtxt('./files/'+from_file)
            except:
                where = self.get_where()
                what = 'Error loading connectivity file: ./files/'+ from_file
                raise BSException(where, what)
            mtx_size = conn_matrix.shape
            required = (self.postLast-self.postFirst+1, self.preLast-self.preFirst+1)
            if mtx_size[1] < required[1] or mtx_size[0] < required[0]:
                where = self.get_where()
                what = 'Connectivity file to small: ' + str(mtx_size) + ' requires ' + str(required)
                raise BSException(where, what)
        elif connectivity == 'topographic' and self.inputs != self.outputs:
            where = self.get_where()
            what = 'Number of inputs and outputs must be the same for topographic connection'
            raise BSException(where, what)



        target_size = self.postLast-self.postFirst
        
        formulas = self.construct_formulas(node) 
        sources =range(self.preFirst, self.preLast+1)

        col = 0
        src=[]
        trg=[]
        for n in sources:
            targets=[]            
            if connectivity == 'full':
                targets += range(self.postFirst, self.postLast)
            elif connectivity == 'probability':
                targets += random.sample(range(self.postFirst, self.postLast), int(probability*target_size))   
            elif connectivity == 'formula':
                for t in range(self.postFirst, self.postLast):             
                    try:          
                        result = Evaluate.evaluate_single(formula)                 
                    except:
                        where = self.get_where()
                        what = 'Error evaluating distribution: ' + distribution + ' -> ' + formula
                        raise BSException(where, what)
                    if result:
                        targets.append(t)
            elif connectivity == 'file': 
                c=0
                for t in range(self.postFirst, self.postLast):
                    if conn_matrix[c,col] != 0:
                        targets.append(t)
                    c+=1
            elif connectivity == 'topographic':
                targets += [self.postFirst+col]
                
            
            src+=[n]*len(targets)
            trg+=targets
            col+=1
            
        if len(targets) > 0: 
            sa = self.evaluate_formulas_batch(formulas, node, self.attributes,dict(), dict(), len(trg))                                   
            self.add_synapses(brain, src, trg, sa)
                

    
    @abstractmethod 
    def add_synapses(self, brain, source, targets, synapse_args):
        pass
        
        
 
