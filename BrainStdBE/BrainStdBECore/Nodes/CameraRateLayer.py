__version__ = 0.001
__abstract__ = False

from BrainStdBECore.Node import Node as Node
from BrainStdBECore.BSException import BSException as BSException
import numpy as np
import pygame
import pygame.camera
from pygame.locals import *
from pygame.surfarray import array2d
from pygame.transform import scale
import time
import os


class BrainStdBEClass(Node) :
    


    def __init__(self) :
        super(BrainStdBEClass,self).__init__()
        self.configurations['CameraRateLayer'] = ([], [], [], []) 
        self.fields.append(['neurons', 'Neurons','integer', '1', ''])
        self.fields.append(['update_time', 'Update time','integer', '', ''])
        self.input_field = 'neurons'
        self.output_field = 'neurons'
        self.units_field = 'neurons'
        self.model_type = 'spike'
        self.prev_img = np.array([])
        self.size = -1

        self.spike_threshold = 1000000

    def __exit__(self, exc_type, exc_value, traceback):
        self.cam.stop()
        self.cam.quit()
        
    def get_input_size(self):
        return self.size
    
    def get_output_size(self):
        return self.size
        
    def get_all_data(self):
        return list(self.outputs)
        
    def get_data(self, args):
        return list(self.outputs)

    def set_data(self, args):
        pass
        
    def initialize(self, brain, node, args):
        super(BrainStdBEClass,self).initialize(brain, node, args)
        new_size = self.safely_get(node, 'neurons', 'int')
        if abs(np.sqrt(new_size) - int(np.sqrt(new_size))) > 1e-6:
            # Square root is decimal, so number is not perfect square
            raise BSException(self.get_where(), 'Number of neurons in a CameraRateLayer has to be a perfect square')
        self.size = new_size
        self.inputs = np.zeros(self.size)
        self.outputs = np.zeros(self.size)

        print "PYGAME init"
        pygame.init()
        pygame.camera.init()
        print "PYGAME cam"
        self.cam = pygame.camera.Camera("/dev/video0")
        print "PYGAME start"
        self.cam.start()
        
        return self.size
            
    def update(self):
        super(BrainStdBEClass,self).update()
    
        # The image will be the largest possible square image with the given
        # number of neurons, and the rest will stay silent
        h = int(np.sqrt(self.size))

        if len(self.prev_img) == 0:
            img = array2d(scale(self.cam.get_image(), (h,h)))
            self.prev_img = np.reshape(img, (1, h*h))
            self.outputs = np.zeros(0)
        else:
            img = array2d(scale(self.cam.get_image(), (h,h)))
            img = np.reshape(img, (1, h*h))
            diff = img - self.prev_img
            self.prev_img = img
            self.outputs = np.where(diff.squeeze() > self.spike_threshold)[0]

        # FIXME: this causes a mismatch between the real image and the GUI rasterplot
        # when the number of neurons is not a perfect square. Zaf says we could fix this by
        # transposing the image before thresholding

    def __del__(self):
        if getattr(self, 'cam', None) != None :
            self.cam.stop()
            self.cam.quit()

