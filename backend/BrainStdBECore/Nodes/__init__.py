"""
The BrainStudioBE Nodes
=======================

"""
import sys
import os

__version__ = '0.0.1'

sys.path.append('./BrainStdBECore/Nodes')

modulePath = 'BrainStdBECore.Nodes.'
__all__ = []

classes = dict()

#print('Nodes:')
for file in os.listdir("./BrainStdBECore/Nodes"):
    if file.endswith(".py"):
        if file != '__init__.py' :
            module = file[:-3]
            module_obj = __import__(module)
            if module_obj.__abstract__ == False :
                __all__.append(module)     
                classes[module] = module_obj


