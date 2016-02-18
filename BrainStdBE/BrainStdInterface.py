__version__ = 0.001

from sys import argv
import exceptions
import time



from BrainStdBECore import Controllers

class BrainStudioInterface:
    
    def __init__(self) :
        controller_name = "ThreadedController"

        try:
            found_controller = Controllers.classes[controller_name]
            print '\nExecuting controller: %s...' %controller_name
            
            self.controller = found_controller.BrainStdBEClass()
            self.controller.set_classname(controller_name)
            
            self.timestep = 0
            self.client_id = 0
            self.controller.get_master(self.client_id)
        
        except KeyError:
            print '\nBrain Studio Error: Controller does not exist: %s' %controller_name
            print('\n')
            exit()

    def __enter__(self) :
         return self
         
    def __del__(self):
        self.controller.close();
        del self.controller;        
        print "Brain Studio Interface waves goodbye!"
        
    def initialize(self, ip = 'localhost', port = 10000) :
        self.controller.local_initialize(ip, port)
        
    def pause(self):
        self.controller.pause(-1)        

    def play(self):
        self.controller.play(-1)     
        
    def clear_sim(self, timeout = 0, respawn = False):
        rv = 0        
        start_time = int(round(time.time() * 1000))
        print 'Brain Studio attempting to clear simulation locally: '
        while (timeout == 0 or int(round(time.time() * 1000))-start_time < timeout) and rv >= 0:
            rv = self.controller.clear_sim(self.client_id, respawn)
            
        if rv == -2:
           print "Brain Studio can not clear simulation. Brain Studio ThreadedController DESTROYED." 
           return -2
        else:
           if rv != -1:
                print "Brain Studio can not clear simulation. Client in control." 
                return -1
           else:
                if timeout != 0 and int(round(time.time() * 1000))-start_time >= timeout:
                    print 'Brain Studio local interface timed out clearing simulation!'
                    return 0;
                
        self.timestep = 0
        print 'Brain Studio cleared simulation locally'
        
    def load_file(self, filename, timeout = 0):
        print 'Brain Studio attempting to load file locally: ' + filename
        rv = 0
        start_time = int(round(time.time() * 1000))
        while  (timeout == 0 or int(round(time.time() * 1000))-start_time < timeout) and rv >= 0: 
            rv = self.controller.load_file(self.client_id, filename)
        
        if rv==-1:
            self.timestep = 0   
            print 'Brain Studio loaded file locally: ' + filename        
            return 1
              
        if rv == -2:
            print "Brain Studio can not load simulation. Brain Studio ThreadedController DESTROYED." 
            return -2
        else:
            if rv != -1:
                print "Brain Studio can not load simulation. Client in control." 
                return -1
            else:
                if  timeout != 0 and int(round(time.time() * 1000))-start_time >= timeout:
                    print 'Brain Studio local interface timed out loading file!'
                    return 0;
           
        if rv == -3:
            print "Brain Studio can not load simulation." 
            return -1   
      
               
    
    def step_sim(self, speed, I_stim = [], timeout = 0,):
        msg = '';
        start_time = int(round(time.time() * 1000))
        while (timeout == 0 or int(round(time.time() * 1000))-start_time < timeout) and (msg == '' or msg == 'badmaster'):
            if not self.controller.is_paused():
                msg = self.controller.step_sim(self.client_id, 1)
             
        if msg == 'badcontroller':
            print "Brain Studio can not update simulation. Brain Studio ThreadedController DESTROYED." 
            return -2
        else:
            if msg == 'badmaster':
                print "Brain Studio can not update simulation. Client in control." 
                return -1
            else:
                if  timeout != 0 and int(round(time.time() * 1000))-start_time >= timeout:
                    print 'Brain Studio local interface timed out updating!'
                    return 0;    
            

        command = msg.split()
        timestep = float(command[1])
        if timestep != self.timestep:
            self.timestep = timestep
        return 1

    def get_node_data(self,  name):
        return self.controller.get_node_data_by_name(name)

    def set_node_data(self, name, args):
        self.controller.set_node_data_by_name(name, args)            
        return 1
  
