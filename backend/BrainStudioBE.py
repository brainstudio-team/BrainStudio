__version__ = 0.001

from sys import argv
import exceptions


help ="\n                  HOW TO USE BRAINSTUDIO BACKEND\n"
help+="----------------------------------------------------------------------\n"
help+="For default behaviour, where the default controller is used type:\n"
help+="\tpython BrainStudioBE.py\n"
help+="\nTo select a different controller type:\n"
help+="\tpython BrainStudioBE.py -controller XXX\n"
help+="\nwhere XXX is the controller name.\n"
help+="\nSee the induividual help for each controller for extra options.\n"
help+="\nAvailable Controllers:.\n"


from BrainStudioBECore import Controllers


controller_name = "DefaultController"


i = 1
while len(argv) > i :
    if argv[i] == "-help" :
        print help
        for c in Controllers.__all__:
            print(c)
            
        print('\n')
        
        for c in Controllers.classes:
            Controllers.classes[c].BrainStudioBEClass().print_help()
        
        exit()
        
    elif argv[i] == "-controller" and len(argv) > i+1 :
        i+=1
        controller_name = argv[i]
        
    i+=1


try:
    found_controller = Controllers.classes[controller_name]
    print '\nExecuting controller: %s...' %controller_name
    
    controller = found_controller.BrainStudioBEClass()
    controller.set_classname(controller_name)
    #try:
    controller.execute(argv)
    #except exceptions.BaseException:
    #    print 'Error in Brain Studio backend!!'
            
except KeyError:
    print '\nError: Controller does not exist: %s' %controller_name
    print('\n')

exit()
