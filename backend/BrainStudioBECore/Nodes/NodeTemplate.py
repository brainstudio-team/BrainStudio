__version__ = 0.001
__abstract__ = False

from BrainStudioBECore.Node import Node
import numpy as np

class BrainStudioBEClass(Node):
    """
    This is a base class to illustrate how to add a new type of node to
    BrainStudio. Follow the indications along this file and implement
    all specified methods and members.
    """
    
    def __init__(self):
        """
        This method acts as a constructor and is run every time a node of
        this type is instantiated.
        """

        # Initialise the parent Node class. All nodes must do this.
        super(Node,self).__init__()

        # Type of I/O the node handles. Currently supported types are 'rate',
        # for modules that take an array of real numbers as input and output;
        # and 'spike' for modules that take a list of activated neurons.
        self.model_type = 'rate'

        self.input_field = ''
        self.output_field = ''
        self.units_field = ''

        # Here you can also add any other class variables the node should have
        # self.mlue = 42


    def get_input_size(self):
        """
        This method returns the size of the arrays the node will be accepting
        when set_data() is called.
        """
        pass
          

    def get_output_size(self):
        """
        This method returns the size of the arrays the node will be returning
        when get_data() is called.
        """
        pass
    

    def set_data(self, args):
        """
        This method sets the input to the node.

        Inputs:
        args  --  Dictionary with various input fields. The values of the
                  input itself are in args['inputs'].
        """

        # Usually the input will be of the size specified by get_input_size()
        # and will be an array of real numbers if model_type == 'rate' or
        # an array of 0's and 1's if model_type == 'spike'.
        inputs = args['inputs']

        pass


    def get_data(self, args):
        """
        This method returns the output of the node.
        
        If there's any operation on the input the node should do each timestep
        this is a good place to do it.
        """

        # The output must be of the same size as specified by
        # get_output_size(). Same considerations as set_data apply.

        pass

