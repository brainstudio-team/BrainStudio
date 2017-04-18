__version__ = 0.001
__abstract__ = False;

__master_not_set__ = -1
__exiting__ = -2

import socket

from sys import stderr
from threading import Thread, Lock


# Import the brainself
from BrainStudioBECore.ControllerBase import Controller as Controller
from BrainStudioBECore.Simulation import Brain as brain
from BrainStudioBECore.BSException import BSException as BSException 
import exceptions

import json
from time import sleep

FILENAME = "simulation.brn"

help ="\nHOW TO USE BRAINSTUDIO TREADED CONTROLLER:\n"
help+="\nTo use this controller type:\n"
help+="\tpython BrainStudioBE.py -controller ThreadedController\n\n"
help+="\tThe simulator will be open for connections from BrainStudio.\n"
help+="\tWhen a new simulation is requested by BrainStudio, the .brn file\n"
help+="\tof this simulation will be downloaded automatically and saved\n"
help+="\tlocally as '"+FILENAME+"'.\n\n"
help+="\tTo run "+FILENAME+" or any other 'brn' file, use the option:\n"
help+="\t\t -file XXX\n"
help+="\n\twhere XXX is the 'brn' file.\n"
help+="\nOther options:\n"
help+="\t-port XXX: used to specify the port of the TCP/IP connection.\n"
help+="\t-ip XXX: used to specify the ip address of the TCP/IP connection\n"
help+="\t(deault is localhost).\n"


class BrainStudioBEClass(Controller) :


    def __init__(self) :
        self.port = 10000
        self.ip = 'localhost'
        
        self.connection_thread = -1
        
        self.block_for_exit = Lock()
        self.block_for_exit.acquire()
        self.block_for_local = Lock()
        self.block_for_client = Lock()
 
        
        
        self.master = __master_not_set__
        self.client_count = 0
        self.clients = []

       
        self.filename = ""
        self.timestep = 0
        self.paused = True
        
        self.steping_sim = False

    def __enter__(self) :
         return self

    def __exit__(self, type, value, tracebselfack) :
        self.close()
        
    def close(self):

        if self.master != __exiting__:

            while not self.get_master(__exiting__):
                pass

            self.clear_sim(__exiting__, False) 
            self.inform_clients_terminated(__exiting__)
            self.block_for_exit.release()
            print "Threaded controler waves goodbye!"
       
    @staticmethod
    def print_help() :
        print help
        
    def execute(self, argv) :
        i = 1
        while len(argv) > i :
            if argv[i] == "-help" :
                # skip handled is BrainStudioBE.py 
                i=i+1
                continue
            elif argv[i] == "-controller" and len(argv) > i+1 :
                # skip handled is BrainStudioBE.py
                i+=2
                continue
            elif argv[i] == "-file" and len(argv) > i+1 :
                i+=1
                self.filename = argv[i]
            elif argv[i] == "-port" and len(argv) > i+1 :
                i+=1
                self.port = int(argv[i])
            elif argv[i] == "-ip" and len(argv) > i+1 :
                i+=1
                self.ip = str(argv[i])
            else :
                print "Argument '" + argv[i] + "' not recognized. Please type python",\
                "BrainStudioBE.py -help."
                exit()
            
            i+=1
            
        print 'Creating simulator'
        self.sim = brain()
            
        if len(self.filename) > 0:
            try:
                self.sim.load_from_brn('./brn/' + self.filename)
            except BSException as e:
                errorin = 'Error in ' + e.get_where()
                print errorin
                print e.get_what()
                exit()
                
        self.start_server()
        
        while self.block_for_exit.locked():
            pass
             

    def local_initialize(self, ip = 'localhost', port = 10000) :
        self.ip = ip
        self.port = port
        self.sim = brain()
        self.start_server()
     
    def clear_sim(self, client_id, respawn = True):
               
 
        # MUTEX HERE 
        self.block_for_local.acquire()
        
       
        if self.master != client_id:
            # MUTEX HERE 
            self.block_for_local.release()
            return self.master
   
        self.paused = True
          
        print 'Deleting simulator'
        del self.sim
        self.sim = 0;
        if respawn:
            print 'Creating simulator'
            self.sim = brain()
        self.timestep = 0
        self.filename = "" 

        # MUTEX HERE
        self.block_for_client.acquire()                 

        for c in self.clients:
            if c[1] != self.master:
                c[0].sendall('killed')
                c[3] = True 
        
        
        # MUTEX HERE
        self.block_for_client.release()
        
        # MUTEX HERE 
        self.block_for_local.release();
               
        return -1;
            
    def load_file(self, client_id, filename):           
        
        # MUTEX HERE      
        self.block_for_local.acquire()
        
        if self.master != client_id:
            # MUTEX HERE      
            self.block_for_local.release()
            return self.master;
            
        # MUTEX HERE      
        self.block_for_local.release()

        
        self.clear_sim(__master_not_set__, True)
        
        # MUTEX HERE      
        self.block_for_local.acquire()
        
        self.filename = filename
        
        print "initializing simulation locally.."   
	
        try:
            self.sim.load_from_brn('./brn/' + self.filename)
        except BSException as e:
            errorin = 'Error in ' + e.get_where()
            print errorin
            print e.get_what()
            self.clear_sim(__master_not_set__, True)
            # MUTEX HERE      
            self.block_for_local.release()
            return -3
            
        # MUTEX HERE      
        self.block_for_local.release()
        
        self.inform_clients_new_file(__master_not_set__)
        return -1


    def inform_clients_new_file(self, master):
        # MUTEX HERE
        self.block_for_client.acquire()
        
        for c in self.clients:
            if c[1] != master:
                c[0].sendall('new file')
                c[3] = True 
                self.paused = True
        
     
        # MUTEX HERE
        self.block_for_client.release()
        
    def inform_clients_terminated(self, master):
        # MUTEX HERE
        self.block_for_client.acquire()
        
        
        for c in self.clients:
            if c[1] != master:
                c[0].sendall('terminated')
        
     
        # MUTEX HERE
        self.block_for_client.release()
       
       
    def get_master(self, client_id):
        while True:
            # MUTEX HERE 
            self.block_for_local.acquire()
            
            if self.steping_sim == False and (self.master == __master_not_set__ or client_id == __exiting__):
                old_master = self.master
                self.master = client_id
                self.block_for_client.acquire()                    
                self.paused = True
                self.block_for_client.release()  
                
                # MUTEX HERE      
                self.block_for_local.release()
                
                if client_id == __exiting__ and old_master > 0:
                    # MUTEX HERE
                    self.block_for_client.aquire()

                    #inform old client
                    for c in self.clients:
                        if c[2] == old_master:
                            c[0].sendall('slave')
                            break 
        
                    # MUTEX HERE
                    self.block_for_client.release()
                    

                return True                  
                
            # MUTEX HERE      
            self.block_for_local.release()
            return False    
        
        
    def delete_client(self, client_id, client_address):
               
        self.block_for_client.acquire()
        
        #remove from client list
        count = 0
        for c in self.clients:
            if c[2] == client_id:
                del self.clients[count]
                break 
            count+=1
            
            
        if  self.master == client_id:  
            self.master = __master_not_set__
            self.paused = False
            

        # MUTEX HERE
        self.block_for_client.release()
        print "Connection with", client_address, '(', str(client_id), ") terminated"
        
    def is_paused(self):
        self.block_for_client.acquire()
        paused = self.paused
        self.block_for_client.release()
        return paused
        
    def pause(self, client_id):
        # MUTEX HERE
        self.block_for_client.acquire()

        #pause  client
        for c in self.clients:                                  
            if c[2] == client_id :
                c[3] = True 
        
        self.paused = True
        
        # MUTEX HERE
        self.block_for_client.release()
        
        
    def play(self, client_id):
        # MUTEX HERE
        self.block_for_client.acquire()

        #play  client
        for c in self.clients:
            if c[2] == client_id :
                c[3] = False
                break
               
        self.paused = False
        for c in self.clients:                                  
            if c[3] == True :
                self.paused = True
                break
            
        if not self.paused:
            for c in self.clients:
                c[0].sendall("done "+str(self.timestep))
                
                
        # MUTEX HERE
        self.block_for_client.release()
        
    def get_node_data_by_name(self, name):
        self.block_for_local.acquire()
        data = self.sim.get_node_data_by_name(name)
        self.block_for_local.release()
        return data

    def set_node_data_by_name(self, name, args):
        self.block_for_local.acquire()
        self.sim.set_node_data_by_name(name, args)
        self.block_for_local.release()    

    def step_sim(self, client_id, speed, I_stim = [], return_data = False):
        # MUTEX HERE      
        self.block_for_local.acquire()
            
        if self.master == __exiting__:
            # MUTEX HERE      
            self.block_for_local.release()
            errorin = 'Error in ThreadedController' 
            what = 'Can not step sim, controller is dying'
            print errorin
            print what
            reply = dict()
            reply['where'] = errorin
            reply['what'] = what
            return_string = "error " + json.dumps(reply)
            return return_string;
            
        self.steping_sim = True
        
        # MUTEX HERE      
        self.block_for_local.release()
        
        if len(self.filename) == 0:
            error = "Can not step simulation. Simulation does not exist." 
            reply = dict()
            reply['where'] = 'Error: Threaded Controller'
            reply['what'] = error
            return_string = "error " + json.dumps(reply)
            return return_string;
                   
        print "Timestep:", self.timestep
        # Simulation here ..........
        try:
            spikes = self.sim.run(I_stim=I_stim)
            rates = self.sim.get_rates()
        except BSException as e:
            errorin = 'Error in ' + e.get_where()
            print errorin
            print e.get_what()
            reply = dict()
            reply['where'] = errorin
            reply['what'] = e.get_what()
            return_string = "error " + json.dumps(reply)
            return return_string
        # ..........................
        if speed > 0 :
            sleep(speed/1000.0)
            
        self.timestep += 1.0
        
        str_spikes = ','.join(str(x) for x in spikes)
        str_rates = ','.join(str(e) for e in rates[0])+' '+','.join(str(e) for e in rates[1])
        data = "done "+str(self.timestep) + " spikes " + str_spikes + ' end_spikes' + ' rates ' + str_rates + ' end_rates'
        
             
        # MUTEX HERE      
        self.block_for_local.acquire()
        
        self.steping_sim = False
        # MUTEX HERE      
        self.block_for_local.release()
        
        
        # MUTEX HERE
        self.block_for_client.acquire()
        #inform  client s
        for c in self.clients:
            if c[2] != self.master and c[4] == True:
                print 'Sending to slave', c[2]
                c[0].sendall(data)   

        # MUTEX HERE
        self.block_for_client.release()
        if not return_data :
            return "done "+str(self.timestep)
        else:
            return data
  
    def start_server(self) :
        try:
            self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.sock.settimeout(10)

            #ZAF: sock.settimeout(5) ???

            # Bind the socket to the port
            self.server_address = (self.ip, self.port)
            print >>stderr, 'starting up on %s port %s' % self.server_address
            self.sock.bind(self.server_address)
        except exceptions.BaseException as e:
            print 'Brain Studio error port:' + str(self.port) + ' - ' + str(e.args[0])
            exit()
            
        self.connection_thread = Thread(target=self.listen_for_connection)    
         
        self.connection_thread.start()
        
            
    def listen_for_connection(self):
        # Listen for incoming connections
        self.sock.listen(5)
        print >>stderr, 'waiting for connections'        
        while True:
            
            if not self.block_for_exit.locked():
                break

            try:
                # Wait for a connection
                connection, client_address = self.sock.accept()
            except:
                continue                
                    
                
            # MUTEX HERE
            self.block_for_client.acquire()
           
                             
            self.clients.append([connection, client_address, self.client_count+1, True, False])            
            listen_thread = Thread(target = self.listen, args=(connection, client_address, self.client_count+1))

            self.paused = True            
            self.client_count+=1
            listen_thread.start()
     
            
            connection.sendall("OK!!")
            
            # MUTEX HERE
            self.block_for_client.release()
            
            print >>stderr, 'New connection from', client_address
            
        # MUTEX HERE
        self.block_for_client.acquire()
            
        clients = list(self.clients)
   
        # MUTEX HERE
        self.block_for_client.release()
  
        for c in clients:
            self.delete_client(c[2], c[1])
            
             
        
        print 'Threaded controller stopped threads.'
            
        
    def listen(self, connection, client_address, client_id) :
        
        recieving_file = False 
       

        try:
            
            while True:
                
                if not self.block_for_exit.locked():
                    break
                
                # MUTEX HERE
                self.block_for_client.acquire()
          
                deleted = True 
                for c in self.clients:
                    if c[2] == client_id:
                        deleted = False
                        break
   
                # MUTEX HERE
                self.block_for_client.release()
  
                if deleted:
                    break
            
                
                data = connection.recv(4096) # It was 16
                                              
                if data:
                                                                      
                    # -- RECIEVING FILE ------------------------------------                            
                    if recieving_file and self.master == client_id:
                        if data == "end" :
                            recieving_file = False
                            print "File ended. Saving.."
                            # Saving file..
                            self.brn_file.close()
                            
                            print "Loading file.."
                            # Loading file..
                            
                            try:
                                self.sim.load_from_brn('./brn/' + self.filename)
                            except BSException as e:
                                errorin = 'Error in ' + e.get_where()
                                print errorin
                                print e.get_what()
                                reply = dict()
                                reply['where'] = errorin
                                reply['what'] = e.get_what()
                                connection.sendall("error " + json.dumps(reply) )
                                break
                            
                            self.block_for_client.acquire()
                                
                            connection.sendall("file loaded "+str(self.sim.get_size()-1))
                                       
                            for c in self.clients:                                  
                                if c[2] == client_id :
                                    c[4] = True 
                                    
                            self.block_for_client.release()
                                
                            self.inform_clients_new_file(client_id)
                            
                            reply = dict()
                            reply['where'] = 'ThreadedController'
                            reply['what'] = 'You are connected as a master'
                            connection.sendall("message " + json.dumps(reply) )
                            
                            
                        else :
                            #print "Adding to file:\t", data
                            self.brn_file.write(data)
                            connection.sendall("give me") # more..

                    elif data == "sending file":
                        
                        if not self.get_master(client_id):
                            self.pause(client_id)
                            self.block_for_client.acquire()
                            connection.sendall("new file" )
                            
                            for c in self.clients:                                  
                                if c[2] == client_id :
                                    c[4] = True 

                            connection.sendall("file loaded "+str(self.sim.get_size()-1))
                                                                
                            self.block_for_client.release()
                            
                            #reply = dict()
                            #reply['where'] = errorin
                            #reply['what'] = 'Another client already has control. You can not send file as a slave'
                            #connection.sendall("error " + json.dumps(reply) )                                   
                        
                        elif self.paused :
                            print "Start recieving brn file.."
                            self.clear_sim(client_id, True)
    
                            recieving_file = True
                            self.filename = FILENAME
                            self.brn_file = open('./brn/'+self.filename, 'w')
                            connection.sendall("give me")
                            
                    elif data == "get file" :
                        command = data.split()
                        
                        # MUTEX HERE      
                        self.block_for_local.acquire()
       
                        if len(self.filename) == 0:
                            reply = dict()
                            reply['where'] = 'ThreadedController'
                            reply['what'] = 'Can not get brain file. No simulation is running'
                            connection.sendall("error " + json.dumps(reply) )
                            # MUTEX HERE      
                            self.block_for_local.unlock()
                            break;
                            
                        brn_file = open(self.filename, 'r')                                
                        data = brn_file.read()     
                        brn_file.close()
                        connection.sendall('file '+ data + ' done')
                        self.pause(client_id)
                        
                        
                        self.block_for_client.acquire()
                        
                        for c in self.clients:                                  
                            if c[2] == client_id :
                                c[4] = True
                                
                        self.block_for_client.release()
                        
                                       
                        

                        # MUTEX HERE      
                        self.block_for_local.release()
                        
                       
                        reply = dict()
                        reply['where'] = 'ThreadedController'
                        reply['what'] = 'You are connected as a slave, a new brain file has been loaded'
                        connection.sendall("message " + json.dumps(reply) )
                        
                        
                            
                    # Format: step <number> spikes <indeces> speed <int_in_ms> stim <indeces> <current>
                    elif data[:4] == "step" :
                        self.block_for_client.acquire()
                        play = not self.paused
                        self.block_for_client.release()
                        if play:
                            command = data.split()
                            if self.master == client_id:
                                print 'Client:', client_id, client_address, data
                                timestep = float(command[1])
                                
                                try:
                                    
                                    self.timestep = timestep
                                    
                                    I_stim = []
                                    speed = 0
                                   
                                    if len(command) >= 7 :                                    
                                        speed = int(command[5])
                                        if command[7][0] == '-':
                                            what = "Stim neuron is negative: "+ command[7]
                                            errorin = 'Error in ThreadesdController' 
                                            print errorin
                                            print what
                                            reply = dict()
                                            reply['where'] = errorin
                                            reply['what'] = what
                                            connection.sendall("error " + json.dumps(reply) )
                                            break
                                        
                                        stim_a = int(command[7].split('-')[0])
                                        stim_b = int(command[7].split('-')[1])
                                        
                                        stim_current = float(command[8])
    
                                        for indx in range(stim_a,stim_b) :
                                            I_stim.append( (indx, stim_current) )
                                            
                                    if len(command) <= 2 :
                                        return_data=False
                                    else:
                                        return_data = True
    
                                    return_string = self.step_sim(client_id, speed, I_stim, return_data)
    
                                    connection.sendall(return_string)
                                    
                                except ValueError:
                                    what = "Step not a float:  "+ command[8]
                                    errorin = 'Error in ThreadedController' 
                                    print errorin
                                    print what
                                    reply = dict()
                                    reply['where'] = errorin
                                    reply['what'] = what
                                    connection.sendall("error " + json.dumps(reply) )
                                    break
                          
                    elif data[:5] == "pause" :
                        self.pause(client_id)
                      
                    elif data[:4] == "play":
                        self.play(client_id)
                        
                    elif data == "get nodes" :

                        self.block_for_local.acquire()                                  
                         
                        print "Sending nodes.."
                        my_nodes = self.sim.get_instantiatable_nodes()
                        reply = dict()
                        for node in my_nodes :
                            params, states, default_params, default_states = my_nodes[node][1]
                            fields = my_nodes[node][2]
                            version = my_nodes[node][3]
                            model = my_nodes[node][5]
                            inputfield = my_nodes[node][6]
                            outputfield = my_nodes[node][7]
                            unitsfield = my_nodes[node][8]
			    help = my_nodes[node][9]
                            reply[node] = dict()
                            reply[node]['fields'] = fields
                            reply[node]['params'] = params
                            reply[node]['states'] = states
                            reply[node]['default_params'] = default_params
                            reply[node]['default_states'] = default_states
                            reply[node]['model'] = model
                            reply[node]['input_field'] = inputfield
                            reply[node]['output_field'] = outputfield
                            reply[node]['units_field'] = unitsfield
                            reply[node]['version'] = version
			    reply[node]['help'] = help
                            
                        self.block_for_local.release()
                        
                        connection.sendall("nodes " + json.dumps(reply) + "nodes end")
                    
                    elif data == "get edges" :
                        print "Sending edges.."
                        
                        self.block_for_local.acquire()
                         
                        my_edges = self.sim.get_instantiatable_edges()
                        reply = dict()
                        for edge in my_edges :
                            params, states, default_params, default_states = my_edges[edge][1]
                            fields = my_edges[edge][2]
                            version = my_edges[edge][3]
                            inputmodel = my_edges[edge][5]
                            outputmodel = my_edges[edge][6]
			    help = my_edges[edge][7]
                            reply[edge] = dict()
                            reply[edge]['fields'] = fields
                            reply[edge]['params'] = params
                            reply[edge]['states'] = states
                            reply[edge]['default_params'] = default_params
                            reply[edge]['default_states'] = default_states
                            reply[edge]['input_model'] = inputmodel
                            reply[edge]['output_model'] = outputmodel
                            reply[edge]['version'] = version
                            reply[edge]['help'] = help
                        
                        self.block_for_local.release()
                        
                        connection.sendall("edges " + json.dumps(reply) + 'edges end' )
                        
                        
                        break
                        
                else:
                    break
                    
        finally:
            self.delete_client(client_id, client_address)      
                
