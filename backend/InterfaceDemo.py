from BrainStdInterface import BrainStudioInterface 
import numpy as np
bsInterface = BrainStudioInterface()
bsInterface.initialize('localhost', 10000)

for a in range(0, 1):
    print 'Loop: '+ str(a)
    if bsInterface.load_file("test.brn", 1000) == 1:
        bsInterface.play()
        rv = 1;
        count = 1
        while rv == 1 and count < 1000:
            count +=1
            print count            
            inputs = np.array([150, 150, 150, 150, 150, 150, 150, 150, 150, 150])
            args = {'first_neuron': 210, "last":219, "inputs":inputs}
            bsInterface.set_node_data("Layer 1", args)
            print 'set'
            rv = bsInterface.step_sim(1, [], 0)
            data = bsInterface.get_node_data("Layer 1")
            print "Data: " + str(data)

print rv      
print 'Ending interface demo';          
bsInterface.clear_sim(0, False)
del bsInterface

