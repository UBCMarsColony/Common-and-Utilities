class Sphere():
    """Sphere object represents a sphere of influence;
       may be a planet, spaceship, asteroid ,etc."""

       def __init__(self, modSys):
           self.modSys = modSys


class ModulatorSystem():
    """ModSys object is a graph or structured
       connection made up of Modulator objects
       and the links connecting them."""

       def __init__(self, collectionOfModulators):
           self.collectionOfModulators = collectionOfModulators

class Modulator():
    """Modulator object takes in an input (mass,
       information, etc.) and also outputs
       something. Uses function object to do
       conversions."""

       def __init__(self, channels, function):
           self.channels = channels
           self.function = function

class Function():
    """Function object converts an input into an
       output; represents the relationship
       between the two."""
