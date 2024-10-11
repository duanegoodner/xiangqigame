"""This is the module docstring

This module does things.
"""

class ExampleClass:
    """
    A simple example class to demonstrate Doxygen-compatible docstrings.

    Detailed description of the class can go here.
    """

    def __init__(self, value):
        """
        The constructor.
        
        @param value An integer to set as an initial value.
        """
        self.value = value

    def add(self, add_value):
        """
        Adds a value to the object's value.

        Detailed explanation can be provided here.

        @param add_value The value to add to the object's value.
        @return The new value after addition.
        """
        self.value += add_value
        return self.value

    def multiply(self, factor):
        """
        Multiplies the object's value by a given factor.

        @param factor The factor by which to multiply the object's value.
        @return The new value after multiplication.
        """
        self.value *= factor
        return self.value

def example_function(param1, param2):
    """
    A simple function to demonstrate parameter and return documentation.

    @param param1 The first parameter.
    @param param2 The second parameter.
    @return The result of combining the parameters.
    """
    return param1 + param2
