__version__ = 0.001
__abstract__ = False

from BrainStudioBECore.Node import Node as Node
from BrainStudioBECore.BSException import BSException as BSException
import numpy as np
import numpy.random as rn

class BrainStudioBEClass(Node):

    def __init__(self):
        super(BrainStudioBEClass,self).__init__()
        self.configurations['BFNode'] = ([], [], [], []) 
        self.fields.append(['neurons', 'Neurons','integer', '1', ''])
        self.fields.append(['freq_mean', 'Frequency mean (Hz)','float', '10.0', ''])
        self.fields.append(['freq_std', 'Frequency std (Hz)','float', '2.0', ''])
        self.input_field = 'neurons'
        self.output_field = 'neurons'
        self.units_field = 'neurons'
        self.size = -1
        
    def get_input_size(self):
        return self.size
    
    def get_output_size(self):
        return self.size
        
    def get_all_data(self):
        return list(self.th)
        
    def get_data(self, args):
        first_neuron = args['first_neuron']
        last = args['last']
        outputs = self.th[first_neuron: last+1]            
        return outputs

        
    def set_data(self, args):
        # inputs = np.array(args['inputs'])
        first_neuron = args['first_neuron']
        last = args['last']
        # for n in range(first_neuron, last+1):
        #     self.inputs[n] += inputs[n-first_neuron]

        if args.has_key('actr_commands'): 
            if 'sync' in args['actr_commands']:
                print "========= SBF SYNC ======================="
                self.time_since_last_sync = 0
                self.th = np.zeros(self.size)
                self.counter = 0

        
    def initialize(self, brain, node, args):
        super(BrainStudioBEClass,self).initialize(brain, node, args)
        self.size = self.safely_get(node, 'neurons', 'int') 
        self.inputs = np.zeros(self.size)
        self.outputs = np.zeros(self.size)
        #self.update_time = self.safely_get(node, 'update_time', 'integer')

        self.freq_mean = self.safely_get(node, 'freq_mean', 'float')
        self.freq_std = self.safely_get(node, 'freq_std', 'float')
        self.omega = self.freq_mean + self.freq_std*rn.randn(self.size)

        self.time_since_last_sync = np.inf

        # Integration is in ms, but frequencies are in Hz
        self.dt = 0.001

        self.counter = 0
        
        return self.size
            

    def update(self):
        super(BrainStudioBEClass,self).update()

        self.outputs = self.omega*self.counter*0.001*2*np.pi
        self.counter += 1

        self.time_since_last_sync += 1

