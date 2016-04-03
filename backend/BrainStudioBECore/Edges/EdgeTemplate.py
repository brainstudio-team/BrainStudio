__version__ = 0.001
__abstract__ = False

from BrainStudioBECore.Edge import Edge
import numpy as np

class BrainStudioBEClass(Edge):
    """
    This is a base class to illustrate how to add a new type of edge to
    BrainStudio. Follow the indications along this file and implement
    all specified methods and members.

    All edges have two very important fields, self.source and self.target,
    that store the ID of the nodes that this edge is linking.
    """

    def __init__(self):
        """
        This method acts as a constructor and is run every time an edge of
        this type is instantiated.
        """

        # Initialise the parent Edge class. All edges must do this.
        super(Edge,self).__init__()

        # Fields input_model and output_model specify the kind of node types
        # this edge links. See comments in Nodes/NodeTemplate.py for more
        # information on node types.
        self.input_model = 'rate'
        self.output_model = 'rate'


    def transfer_data(self, args):
        """
        This method transfers the data from the source node to the target.
        The data must be fetched and delivered using the nodes' get_data()
        and set_data() methods.
        """

        # First fetch the inputs from the source node
        inputs = self.source.get_data(args)

        # Calculate the output with any arbitrary function
        foobar = lambda x : x
        outputs = foobar(inputs)

        # Form a dictionary to feed the target node with the output of the edge
        args = {'inputs' : outputs, 'first_neuron' :  self.postFirst, 'last' : self.postLast }       
        self.target.set_data(args)

