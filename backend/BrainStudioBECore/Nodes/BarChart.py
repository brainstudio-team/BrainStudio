__version__ = 0.001
__abstract__ = False

from BrainStudioBECore.Visual import Visual as Visual
from BrainStudioBECore.BSException import BSException as BSException
import numpy as np
import io
import matplotlib.pyplot as plt
from PIL import Image

class BrainStudioBEClass(Visual) :
    


    def __init__(self) :
        super(BrainStudioBEClass,self).__init__()
 
	self.model_type = 'rate'
	self.configurations['BarChart'] = ([], [], [], []) 
	
    def get_visual_type(self):
	return 'png'     	
	
    def initialize(self, brain, node, args):
        super(BrainStudioBEClass,self).initialize(brain, node, args)
        return self.size
            
    def update(self):
        super(BrainStudioBEClass,self).update()

	y_pos = np.arange(len(self.inputs))	

	plt.figure()
	plt.bar(y_pos, self.inputs, align='center', alpha=0.5)
	plt.title("Test Visual")

	self.buffer.close()
	self.buffer = io.BytesIO()
	plt.savefig(self.buffer, format='png')
	plt.close()	
	self.buffer_size = self.buffer.tell()
	self.buffer.seek(0)
	

        self.inputs = np.zeros(self.size)
        
