"""
The BrainStudioBE Edges
=======================

"""
import sys
import os

__version__ = '0.0.1'

sys.path.append('./BrainStudioBECore/Edges')

modulePath = 'BrainStudioBECore.Edges.'
__all__ = []

classes = dict()

#print('Edges:')
for file in os.listdir("./BrainStudioBECore/Edges"):
    if file.endswith(".py"):
        if file != '__init__.py' :
            module = file[:-3]
            module_obj = __import__(module)
            if module_obj.__abstract__ == False :
                __all__.append(module)     
                classes[module] = module_obj

