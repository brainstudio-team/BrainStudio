__version__ = 0.001

from BrainStudioBECore.BSException import BSException as BSException 
import random, nemo, numpy as np, matplotlib.pyplot as plt, pickle, time
import Nodes
import Edges
import exceptions

class Brain :
    

    def __init__(self):
        
        self.file = ''
        
        self.has_nemo_neurons = False
        self.neurons = 0
        self.all_neurons = 0
        self.synapses = 0
        self.time_passed = 0
        self.stopped = False
    
        self.groups = []
        
        self.net = nemo.Network()
        self.conf = nemo.Configuration()
        self.nemo_neuron_types = dict()     
        self.edges = []
        self.actions = []
        self.scheduler = []

        self.instantiatable_nodes = dict()
        for key in Nodes.classes:
           node = Nodes.classes[key]
           if node.__abstract__ == False:              
                node_object = node.BrainStudioBEClass()
                node_object.configure_static()
                configurations = node_object.get_configurations() 
                for con in configurations: 
                    node_name = con+'-'+ node_object.get_architecture()
                    self.instantiatable_nodes[node_name] = \
                        (node, configurations[con], \
                        node_object.get_fields(), node_object.get_version(),\
                        node_object.get_architecture(), \
                        node_object.get_model_type(node_object.get_architecture()),\
                        node_object.get_input_field(node_object.get_architecture()),  \
                        node_object.get_output_field(node_object.get_architecture()), \
                        node_object.get_units_field())
                    
                del node_object

        self.instantiatable_edges = dict()
        for key in Edges.classes:
           edge = Edges.classes[key]
           if edge.__abstract__ == False:
                edge_object = edge.BrainStudioBEClass()
                edge_object.configure_static()
                configurations = edge_object.get_configurations() 
                for con in configurations: 
                    self.instantiatable_edges[con+'-'+edge_object.get_architecture()] = \
                    (edge, configurations[con], \
                    edge_object.get_fields(), edge_object.get_version(), \
                    edge_object.get_architecture(), \
                    edge_object.get_input_model(), edge_object.get_output_model())
                    
                del edge_object

        self.instantiatable_actions = dict()
        self.instantiatable_actions["stimulate"] = []
        self.instantiatable_actions["adjust"] = []
        self.instantiatable_actions["stop"] = []
        # TODO: Finish this

        print "Initialized Brain"

    def __enter__(self) :
        return self

    def __exit__(self, type, value, traceback) :
        if hasattr(self, 'net'):
            del self.net
        if hasattr(self, 'conf'):
            del self.conf
        if hasattr(self, 'sim'):
            print 'DESTROYING NEMO'
            del self.sim
        print "NeMo salutes!"

    def get_size(self):
        return self.all_neurons
    
    def __del__(self):
        if hasattr(self, 'net'):
            del self.net
        if hasattr(self, 'conf'):
            del self.conf
        if hasattr(self, 'sim'):
            print 'DESTROYING NEMO'
            del self.sim
        print "NeMo died"
        
    def get_file(self):
        return self.file
        
    def get_instantiatable_nodes(self):
        return self.instantiatable_nodes

    def get_instantiatable_edges(self):
        return self.instantiatable_edges
        
    def get_node(self, index):
        return self.groups[index]['node']
        
    def get_node_by_name(self, name):
        for g in self.groups:
	    if g['name'] == name:
		return g['node']
    
    def get_node_size(self, name):
        for g in self.groups:
	    if g['name'] == name:
		return g['size']

        return -1
        
    def get_node_first_neuron(self, name):
        for g in self.groups:
	    if g['name'] == name:
		return g['size']

        return -1
    def add_nemo_neuron_type(self, name):
        try:
           neuron_type_id = self.nemo_neuron_types[name] 
        except KeyError:
            neuron_type_id = self.net.add_neuron_type(name)
            self.nemo_neuron_types[name] = neuron_type_id
            
        return neuron_type_id
    
    def get_nemo_neuron_type(self, name):
        return self.nemo_neuron_types[name] 

        
    def add_nemo_neuron(self, neuron_type_id, paras, states, num):
        self.net.add_neuron(neuron_type_id, range(self.neurons, self.neurons+num), paras, states);
        self.neurons += num
        self.has_nemo_neurons = True
        
    def add_nemo_synapse(self, sources, targets, delays, weights, plastic):
        self.net.add_synapse(sources, targets, delays, weights, plastic)
        self.synapses += len(targets)
        return;
        
    def get_nemo_synapse_weight(self, indexes):
        return self.net.get_synapse_weight(indexes)
        
    def get_nemo_neurons_state(self, indexes, state):
        return self.net.get_neuron_state(indexes, state)

    def get_nemo_neurons_paramater(self, indexes, para):
        return self.net.get_neuron_parameter(indexes, para)
        
    def get_model_type(self, index):
         return self.groups[index]['node'].get_model_type()
         
    def get_rates(self):
        indexes = []
        rates = []
        for group in self.groups:
            node = group['node']
            if node.get_model_type() == 'rate':
                first =  group['first_all_neurons'] 
                size = group['size']
                indexes += range(first, first+size)
                rates += node.get_all_data()
        return [indexes, rates]

        
    def get_node_data(self, index):
        group = self.groups[index]
        fired = group['node'].get_all_data()
        if group['node'].get_model_type() == 'spike':
                indexed = np.array(fired)+group['first_all_neurons']
                fired=indexed.tolist()
        return fired

    def get_edge_data(self, index):
        return self.edges[index].get_all_data()
        
    def get_node_data_by_name(self, name):
        node = self.get_node_by_name(name)
        return node.get_all_data()

    def set_node_data_by_name(self, name, args):
        node = self.get_node_by_name(name)
        size = self.get_node_size(name)
        first_neuron = args['first_neuron']
        last = args['last'] 
        if first_neuron >= 0 and first_neuron < size and last >= 0 and last <size:
            node.set_data(args)
            return 1
        else:
            return 0
        
    def load_from_brn(self, path) :
        self.file = path
        import xml.etree.ElementTree as ET
        try:
            tree = ET.parse(path)
        except:
            where = 'Simulator Brain '
            what = 'Error parsing file: ' + path
            raise BSException(where, what)
            
        root = tree.getroot()

        nodes = root.find('nodes')
        edges = root.find('edges')
        my_actions = root.find('actions')
        speed = float(root.attrib['speed'])
        print "Speed:", speed
        print "actions:", len(my_actions)

        for node in nodes :
            print node.tag;
            self.add_node(node)

        for edge in edges :
            print edge.tag;
            self.add_edge(edge)

        for action in my_actions :
            print action.tag;
            self.add_action(action)
        
        if self.has_nemo_neurons:
            try:
                self.conf = nemo.Configuration()
                self.sim = nemo.Simulation(self.net, self.conf)
            except exceptions.BaseException as e:
                where = 'Simulator Brain '
                what = 'Error creating Nemo simulation: ' + e.args[0]
                raise BSException(where, what)
            
        print "Neurons:", self.neurons, self.net.neuron_count()
        print "Synapses:", self.synapses

        self.scheduler.sort(cmp=by_time) 

    def add_node(self, node) :

        node_model = node.tag
        try:
            data = self.instantiatable_nodes[node_model]
            node_object = data[0].BrainStudioBEClass()

        except KeyError:
            where = 'Simulator Brain '
            what = 'Error can not find Node: ' + node_model
            raise BSException(where, what)
        
        node_object.set_name(node)
        node_object.set_class_name(node_model)
        node_object.set_attributes(data[1][0], data[1][1])
        node_object.check_version(node)
        
        start_neuron = self.neurons
        
        node_args = dict();
        node_args['start_neuron'] = start_neuron 
        try:
            size = node_object.initialize(self, node, node_args)
        except exceptions.BaseException as e:
            where = 'Simulator Brain '
            what = 'Error initializing Node: ' +node_model + ' -> '+ node_object.get_name() + ' -> '+ e.args[0]
            raise BSException(where, what)

        group = {'name' : node_object.get_name(), 'binned' : [], 'first_all_neurons' : self.all_neurons, 'first_neuron' : node_object.get_start_neuron(), 'size' : size, 'node' : node_object}

        self.scheduler.append([node_object.get_update_time(), node_object, node_model, node_object.get_name()])
        
        self.groups.append(group)
        
        self.all_neurons+=size

    def add_edge(self, node) :
        edge_model = node.tag
        try:
            data = self.instantiatable_edges[edge_model]
            edge_object = data[0].BrainStudioBEClass()

        except KeyError:
            where = 'Simulator Brain '
            what = 'Error can not find Edge: ' + edge_model
            raise BSException(where, what)

        edge_object.set_name(node)
        edge_object.set_class_name(edge_model)
        edge_object.set_attributes(data[1][0], data[1][1])
        edge_object.check_version(node)        
        edge_args = dict()
        try:
            edge_object.initialize(self, node, edge_args)
        except exceptions.BaseException as e:
            where = 'Simulator Brain '
            what = 'Error initializing Edge: ' + edge_model + ' -> ' + edge_object.get_name() + ' -> '+ str(e.args[0])
            raise BSException(where, what)
            
        self.edges.append(edge_object)


    # TODO: Finish this method for general actions!
    def add_action(self, action) :
        action_type = action.tag

        try:
            data = self.instantiatable_actions[action_type]
            action_object = {'type' : action_type }
            for at in action.attrib :
                action_object[at] = action.attrib[at]
            action_object['t'] = int(action_object['t'])
        except KeyError:
            where = 'Simulator Brain '
            what = 'Error can not find Action: ' + action_type
            raise BSException(where, what)

        self.actions.append(action_object)

            
    def update_nemo(self, I_stim_extra=[]):
        
        if not self.has_nemo_neurons:
            return
 
        #re index stims taking out non NeMo neurons
        if len(I_stim_extra)>0:
            indexes = []
            for i in I_stim_extra:
                indexes.append(i[0])
            indexes = np.array(indexes)
            
            rev_groups = list(self.groups)
            rev_groups.reverse()
            for group in rev_groups:
                node = group['node']
                if node.get_architecture() != 'NeMo':
                    size = group['size']
                    start = group['first_all_neurons']
                    indexes[indexes>start]-=size
  
            count = 0
            for i in I_stim_extra:
                I_stim_extra[count] = (indexes[count], i[1])
                count+=1
                
           
        # get internally set stims
        I_stim = []        
        for group in self.groups:
            node = group['node']
            if node.get_architecture() == 'NeMo':        
                I_stim = I_stim + node.get_stim()
                
        # stem Nemo
        fired = self.sim.step(istim=I_stim_extra+I_stim) #=I_stim)

        # clear firings
        for group in self.groups:
            node = group['node']
            node.set_fired([])
            if node.get_architecture() == 'NeMo':
                node.clear_stim()                
                
        # record firings inside node                 
        neuron = 0
        while neuron < len(fired):           
            for group in self.groups:
                if neuron >= len(fired):
                    break
                node = group['node']
                if node.get_architecture() == 'NeMo':
                    group_fired =[]
                    first = group['first_neuron']
                    last = first+group['size']-1
                    
                    if fired[neuron] >= first:
                        while neuron < len(fired) and fired[neuron] <= last:
                            group_fired.append(fired[neuron]-first) 
                            neuron += 1
                    else:
                        neuron += 1
                         
                    node.set_fired(group_fired)
          
        return
          
    def run(self, duration=1, I_stim=[], freq=0.0) :
        
        # Check actions
        for i,action in enumerate(self.actions) :
            if action['t'] <= self.time_passed :
                # Execute action
                print "Executing action:", action
                if action['type'] == 'stop' :
                    self.stopped = True
                    self.time_passed = 0
                    return []
                if action['type'] == 'stimulate' :
                    print "not done yet"
                if action['type'] == 'adjust' :
                    print "not done yet"
                # Delete it
                self.actions.pop(i)

        #separate NeMo and non Nemo stims           
        N_stim=[]
        i=0
        while i < len(I_stim):
            for group in self.groups:
                if i==len(I_stim):
                    break
                node = group['node']
                first = group['first_all_neurons']
                last = first+group['size']-1
                stim=I_stim[i]
                if stim[0] >=first and stim[0]<=last:
                    if node.get_architecture() != 'NeMo':
                        start = stim[0]
                        end = start
                        inputs = []
                        while stim[0] >=first and stim[0]<=last:
                            inputs.append(stim[1])
                            stim=I_stim[i]
                            end = stim[0]
                            i+=1
                            if i==len(I_stim):
                                break
                            
                        args = {'inputs' : inputs, 'first_neuron' : start-first, 'last' :  end-first }                            
                        node.set_data(args)
                        
                    else:
                        while stim[0] >=first and stim[0]<=last:
                            N_stim.append((stim[0],stim[1]))
                            i+=1
                            if i==len(I_stim):
                                break
                            stim=I_stim[i]

                                              
                
         
        # update nodes
        for t in range(duration):
            
            ran_nemo = False
            fired=[]
            for event in self.scheduler:

                if ran_nemo == False and event[0] > 0:
                    self.update_nemo(N_stim)                  
                    ran_nemo = True
                try:
                    event[1].update()
                except exceptions.BaseException as e:
                    where = 'Simulator Brain '
                    what = 'Error updating: ' + event[2] + ' -> '+ event[3] + ' : ' + e.args[0]
                    raise BSException(where, what)
                
            if ran_nemo == False:
                self.update_nemo(N_stim)
                ran_nemo = True
             
        #set firings
        fired=[]            
        for group in self.groups:
            node = group['node']
            if node.get_model_type() == 'spike':
                indexed = np.array(node.get_all_data())+group['first_all_neurons'] 
                fired+=indexed.tolist()
                
        self.time_passed += duration
        print "Spikes:", len(fired), "t:", self.time_passed
        return fired

    def save(self) :
        with open('groups.pkl', 'wb') as output :
            # Pickle dictionary using protocol 0.
            pickle.dump(self.groups, output)
    
    def plot(self) :
        all_graphs = 2 + len(self.groups)        
        
        plt.subplot(all_graphs, 1, 1)
        plt.scatter(self.raster_x, self.raster_y, s=1)
        plt.xlim(0,self.time_passed)
        plt.ylim(0,self.neurons)
        plt.subplot(all_graphs, 1, 2)
        plt.plot(self.trace)
        plt.title("observed")

        i = 3
        for g in self.groups :
            plt.subplot(all_graphs, 1, i)
            plt.plot(g['binned'])
            plt.title("group "+str(i-2))
            i += 1


def by_time(word1,word2):
    return word1[0]-word2[0]
