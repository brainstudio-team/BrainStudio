__version__ = 0.001
__abstract__ = False

from BrainStudioBECore.Node import Node as Node
from BrainStudioBECore.BSException import BSException as BSException
import numpy as np
import numpy.random as rn

class BrainStudioBEClass(Node):

    def __init__(self):
        super(BrainStudioBEClass,self).__init__()
        self.configurations['SBFNode'] = ([], [], [], []) 
        self.fields.append(['neurons', 'Neurons','integer', '1', ''])
        self.fields.append(['base_k', 'Base K','float', '0.04', ''])
        self.fields.append(['peak_k', 'Peak K','float', '0.2', ''])
        self.fields.append(['decay_k', 'Decay K (ms)','float', '5.0', ''])
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
        print "Enter SBFNode::set_data"
        # inputs = np.array(args['inputs'])
        first_neuron = args['first_neuron']
        last = args['last']
        # for n in range(first_neuron, last+1):
        #     self.inputs[n] += inputs[n-first_neuron]

        print "SBFNode::set_data args", args

        if args.has_key('actr_commands'): 
            if 'sync' in args['actr_commands']:
                self.time_since_last_sync = 0

        print "SBFNode setdata finish"

        
    def initialize(self, brain, node, args):
        super(BrainStudioBEClass,self).initialize(brain, node, args)
        self.size = self.safely_get(node, 'neurons', 'int') 
        self.inputs = np.zeros(self.size)
        self.outputs = np.zeros(self.size)
        #self.update_time = self.safely_get(node, 'update_time', 'integer')

        self.base_k = self.safely_get(node, 'base_k', 'float')
        self.peak_k = self.safely_get(node, 'peak_k', 'float')
        self.decay_k = self.safely_get(node, 'decay_k', 'float')

        self.freq_mean = self.safely_get(node, 'freq_mean', 'float')
        self.freq_std = self.safely_get(node, 'freq_std', 'float')
        self.omega = self.freq_mean + self.freq_std*rn.randn(self.size)

        self.time_since_last_sync = np.inf

        # Integration is in ms, but frequencies are in Hz
        self.dt = 0.001

        self.th = 2*np.pi*rn.rand(self.size)
        
        return self.size
            

    def update(self):
        print "ZAF1"
        super(BrainStudioBEClass,self).update()

        print "Start Kuramoto dynamics", self.th

        N = self.size
        K = self.base_k + np.exp(-self.time_since_last_sync/self.decay_k)
        d_th = np.zeros(N)
        # Kuramoto dynamic equation solved with Euler method
        for i in range(N):
            d_th[i] = self.omega[i] + (1.0/N) * K * np.sum(np.sin(self.th - self.th[i]))

        self.th = self.th + self.dt*d_th
        self.outputs = self.th

        self.time_since_last_sync += 1


#     for decay_k in [5,10,20,40,100] :
# 
#         N = 300
#         omega = 10 + 2*rn.randn(N)
#         dt = 0.01
# 
#         def kuramoto(th, K):
#             d_th = np.zeros(N)
#             for i in range(N):
#                 d_th[i] = omega[i] + (1.0/N) * K * np.sum(np.sin(th - th[i]))
#             return (th + dt*d_th)
# 
#         T = 1000
#         rate = np.zeros(T)
#         th = np.zeros(N)
#         base_k = 0.0
#         peak_k = 0.2
#         TH = np.zeros((T,N))
# 
#         for t in range(T):
#             th = kuramoto(th, base_k + peak_k*np.exp(-t/decay_k))
#             if t == 500:
#                 W = np.cos(th)
# 
#         th = np.zeros(N)
#         for t in range(T):
#             TH[t] = th
#             th = kuramoto(th, base_k + peak_k*np.exp(-t/decay_k))
#             rate[t] = np.dot(W, np.cos(th))

