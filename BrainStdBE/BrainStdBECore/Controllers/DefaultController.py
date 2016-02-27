__version__ = 0.001
__abstract__ = False;


import socket

from sys import stderr

# Import the brainself
from BrainStdBECore.ControllerBase import Controller as Controller
from BrainStdBECore.Simulation import Brain as brain
from BrainStdBECore.BSException import BSException as BSException 
import exceptions

import json
from time import sleep, time

FILENAME = "simulation.brn"

help ="\nHOW TO USE BRAINSTUDIO DEFAULT CONTROLLER:\n"
help+="\nTo use this controller type:\n"
help+="\tpython BrainStdBE.py -controller DefaultControler\n"
help+="\nWith GUI:\n"
help+="\tFor default behaviour, the simulator just waits for a\n"
help+="\tconnection from BrainStd.\n"
help+="\tWhen a new simulation is requested by BrainStd, the .brn file\n"
help+="\tof this simulation will be downloaded automatically and saved\n"
help+="\tlocally as '"+FILENAME+"'.\n\n"
help+="Without GUI:\n"
help+="\tTo run "+FILENAME+" or any other 'brn' file, use the option:\n"
help+="\t\t -nogui -file XXX\n"
help+="\n\twhere XXX is the 'brn' file.\n"
help+="\nOther options:\n"
help+="\t-port XXX: used to specify the port of the TCP/IP connection.\n"


class BrainStdBEClass(Controller) :


    def __init__(self) :
        self.no_sockets = False
        self.filename = "simulation.brn"
        self.port = 10000
        self.ip = 'localhost'
  
    def __enter__(self) :
         return self

    def __exit__(self, type, value, tracebselfack) :
        #del self.net
        print "Default controler waves goodbye!"
       
    @staticmethod
    def print_help() :
        print help
        
    def execute(self, argv) :
        i = 1
        while len(argv) > i :
            if argv[i] == "-help" :
                # skip handled is BrainStdBE.py 
                i=i+1
                continue
            elif argv[i] == "-controller" and len(argv) > i+1 :
                # skip handled is BrainStdBE.py
                i+=2
                continue
            elif argv[i] == "-nogui" :
                self.no_sockets = True
                print('NOGUI......')
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
                "BrainStdBE.py -help."
                exit()
            
            i+=1

        if self.no_sockets :
            print "Starting simulation without GUI.."
            
	
            self.sim = brain()
            self.spikes = []
            try:
                self.sim.load_from_brn('./brn/' + self.filename)
                self.exp_start = time()
                while not self.sim.stopped :
                    self.spikes = self.sim.run()
                print "End. Duration:", time() - self.exp_start
            except BSException as e:
                errorin = 'Error in ' + e.get_where()
                print errorin
                print e.get_what()
                exit()
             
        else:
            self.start_server()
            self.listen()
            
    def start_server(self) :
        # Create a TCP/IP socket
        try:
            self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

            #ZAF: sock.settimeout(5) ???

            # Bind the socket to the port
            self.server_address = (self.ip, self.port)
            print >>stderr, 'starting up on %s port %s' % self.server_address
            self.sock.bind(self.server_address)
        except exceptions.BaseException as e:
            print 'Brain Studio error port:' + str(self.port) + ' - ' + str(e.args[0])
            exit()


    def listen(self) :
        # -- Backend ---------------------------------------------------------------
        spikes = []
        # --------------------------------------------------------------------------

        # Listen for incoming connections
        self.sock.listen(5)

        paused = True
        recieving_file = False
        timestep = 0.0
        exp_start = time()
       
    
        while True:
            # Wait for a connection
            print >>stderr, 'waiting for a connection'
            connection, client_address = self.sock.accept()
     
            try:
                print >>stderr, 'New connection from', client_address

                connection.sendall("OK!!")
    

                # Start a new nemo simulation
                with brain() as sim :
                    # Receive the data in small chunks and retransmit it
                    while True:

                        #print "ZAF:",connection
                        data = connection.recv(4096) # It was 16
                
                        if data:
                            print 'Client:', data
        
                            # -- RECIEVING FILE ------------------------------------                            
                            if recieving_file :
                                if data == "end" :
                                    recieving_file = False
                                    print "File ended. Saving.."
                                    # Saving file..
                                    self.brn_file.close()
                                    print "Loading file.."
                                    # Loading file..
                                    
                                    try:
                                        sim.load_from_brn('./brn/' + self.filename)
                                    except BSException as e:
                                        errorin = 'Error in ' + e.get_where()
                                        print errorin
                                        print e.get_what()
                                        reply = dict()
                                        reply['where'] = errorin
                                        reply['what'] = e.get_what()
                                        connection.sendall("error " + json.dumps(reply) )
                                        break
                                    
                                    connection.sendall("file loaded "+\
                                        str(sim.get_size()))
                                else :
                                    #print "Adding to file:\t", data
                                    self.brn_file.write(data)
                                    connection.sendall("give me") # more..
                            # ------------------------------------------------------

                            # Format: step <number> spikes <indices> speed <int_in_ms> stim <indices> <current>
                            if data[:4] == "step" and not paused:
                                command = data.split()

                                # TODO: Now we rely on the assumption that the messages always follow the order
                                # mentioned above. It would be safer to check every part of the string

                                try:
                                    
                                    timestep = float(command[1])
                                    speed = 0
                                   
                                    if len(command) >= 7 :                                    
                                        speed = int(command[5])
                                        if command[7][0] == '-':
                                            what = "Stim neuron is negative: "+ command[7]
                                            errorin = 'Error in DefaultController' 
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

                                        I_stim = [ (indx, stim_current) for indx in range(stim_a, stim_b) ]

                                    print "Timestep:", timestep

                                    # Simulation here ..........
                                    try:
                                        spikes = sim.run(I_stim=I_stim)
                                        rates = sim.get_rates()
                                    except BSException as e:
                                        errorin = 'Error in ' + e.get_where()
                                        print errorin
                                        print e.get_what()
                                        reply = dict()
                                        reply['where'] = errorin
                                        reply['what'] = e.get_what()
                                        connection.sendall("error " + json.dumps(reply) )
                                        break
                                    # ..........................

                                    # If the execution of run() found an action that
                                    # requires the simulation to pause, inform the
                                    # controller..
                                    if sim.stopped :
                                        print "Simulation stopped. Duration:", time() - self.exp_start
                                        timestep = -1.0
                                        paused = True

                                    if speed > 0 :
                                        sleep(speed/1000.0)

                                    timestep += 1.0

                                    if len(command) <= 2 :
                                        connection.sendall("done "+str(timestep))

                                    elif len(command) >= 7 :
                                        if command[3] == "all" :
                                            # Delete first and last element, i.e.'[' and ']'
                                            str_spikes = ','.join(str(x) for x in spikes)
                                            str_rates = ','.join(str(e) for e in rates[0])+' '+','.join(str(e) for e in rates[1])
                                            connection.sendall("done "+str(timestep) + \
                                                               " spikes " + str_spikes + ' end_spikes' + \
                                                               ' rates ' + str_rates + ' end_rates')
                                                               
                          
                                except ValueError:
                                    what = "Step not a float:  "+ command[1]
                                    errorin = 'Error in DefaultController' 
                                    print errorin
                                    print what
                                    reply = dict()
                                    reply['where'] = errorin
                                    reply['what'] = what
                                    connection.sendall("error " + json.dumps(reply) )
                                    break
                                        
                            elif data[:5] == "pause" :
                                print "Pausing.. Duration:", time() - self.exp_start
                                paused = True
                  
                            elif data[:4] == "play":
                                print "Starting.."
                                paused = False
                                sim.stopped = False
                                connection.sendall("done "+str(timestep))
                                self.exp_start = time()
                               
                            elif data == "get nodes" :
                                print "Sending nodes.."
                                my_nodes = sim.get_instantiatable_nodes()
                                reply = dict()
                                for node in my_nodes :
                                    params, states, default_params, default_states = my_nodes[node][1]
                                    fields = my_nodes[node][2]
                                    version = my_nodes[node][3]
                                    model = my_nodes[node][5]
                                    inputfield = my_nodes[node][6]
                                    outputfield = my_nodes[node][7]
                                    unitsfield = my_nodes[node][8]
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
                                connection.sendall("nodes " + json.dumps(reply) + "nodes end")

                            elif data == "get edges" :
                                print "Sending edges.."
                                my_edges = sim.get_instantiatable_edges()
                                reply = dict()
                                for edge in my_edges :
                                    params, states, default_params, default_states = my_edges[edge][1]
                                    fields = my_edges[edge][2]
                                    version = my_edges[edge][3]
                                    inputmodel = my_edges[edge][5]
                                    outputmodel = my_edges[edge][6]
                                    reply[edge] = dict()
                                    reply[edge]['fields'] = fields
                                    reply[edge]['params'] = params
                                    reply[edge]['states'] = states
                                    reply[edge]['default_params'] = default_params
                                    reply[edge]['default_states'] = default_states
                                    reply[edge]['input_model'] = inputmodel
                                    reply[edge]['output_model'] = outputmodel
                                    reply[edge]['version'] = version
                                connection.sendall("edges " + json.dumps(reply) + 'edges end' )

                            elif data[:4] == "node" :
                                index = int(data[4:-1])
                                data = sim.get_node_data(index)
                                if sim.get_model_type(index == 'spike'):
                                    str_spikes = ','.join(str(x) for x in data)
                                    connection.sendall("spikes " + str_spikes + " done" )
                                elif sim.get_model_type(index == 'rate'):
                                    all_rates = ''
                                    for d in data:
                                        str_rates = '['
                                        str_rates += ','.join(str(x) for x in d)
                                        str_rates += ']'
                                    all_rates+=str_rates    
                                    connection.sendall("rates " + all_rates + " done"  )
                                else :
                                    connection.sendall("done")
                                
                            elif data[:4] == "edge" :
                                data = sim.get_edge_data(int(data[4:-1]))
                                all_weights = ''
                                for d in data:
                                    str_weights= '['
                                    str_weights += ','.join(str(x) for x in d)
                                    str_weights += ']'
                                all_weights+=str_weights    
                                connection.sendall("weights " + all_weights + " done" )

                            elif data == "sending file" and paused :
                                print "Start recieving brn file.."

                                # In case of having already a simulation running, one  
                                # option would be to resume simulation, and then I would
                                # just need to return the following msg:
                                #if sim.neurons > 0 :# Already have a simulation running
                                #    connection.sendall("already running "+\
                                #                       str(sim.net.neuron_count()))
                                #else :
                                #    recieving_file = True
                                #    brn_file = open('simulation.brn', 'w')
                                #    connection.sendall("give me")
    
                                # However here I will just reinitialize the network:
                                if sim.neurons > 0 : # Already have a simulation running
                                    print "Error: Sim neurons:", sim.neurons
                                    exit()
                
                                recieving_file = True
                                self.brn_file = open('./brn/' + 'simulation.brn', 'w')
                                connection.sendall("give me")
                        else:
                            print "Connection with", client_address, "terminated"
                            break
                            """                
                            print >>stderr, 'received "%s"' % data
                            if data:
                                print >>stderr, 'sending data back to the client'
                                connection.sendall(data)
                            else:
                                print >>stderr, 'no more data from', client_address
                                break
                            """

            finally:
                #Clean up the connection
                paused = True
                connection.close()
