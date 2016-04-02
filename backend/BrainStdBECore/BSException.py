__version__ = 0.001




class BSException() :
    
    def __init__(self, where, what) :
        self.where = where
        self.what = what

    def get_where(self):
        return self.where

    def get_what(self):
        return self.what        
        
 