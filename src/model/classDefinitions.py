from abc import ABC, abstractmethod
import math
import random

class Sphere():
    """Sphere object represents a sphere of influence;
       may be a planet, spaceship, asteroid ,etc."""

    def __init__(self, modSys, spatialVector, functionVector, timeStep):
        self.modSys = modSys
        self.spatialVector = spatialVector
        self.functionVector = functionVector
        self.timeStep = timeStep

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

    def __init__(self, channels, function, inputVal, outputVal):
        self.channels = channels
        self.function = function
        self.inputVal = inputVal
        self.outputVal = outputVal
 
class Function(ABC):
    """Function is an abstract base class for other
       types of function classes such as polynomial,
       exponential, sinusoidal, logarithmic, custom,
       etc.
       Function object converts an input into an
       output; represents the relationship
       between the two."""
    
    def __init__(self, characteristic):
        self.characteristic = characteristic
        super().__init__()
    
    @abstractmethod
    def calculate(self, x):
        pass

class polynomialFunction(Function):
    """Represents a polynomial function.  Characteristic 
       is a list containining coefficients of x^n terms
       from n = 0, to n = n.  Coefficients are written
       such that the degree of the x^n terms is 
       increasing."""
    def __init__(self, characteristic):
        super().__init__(characteristic)

    def calculate(self, x):
        size = len(self.characteristic)
        sumSoFar = 0
        for i in range(size):
            sumSoFar += self.characteristic[i]*(x**i)
        return sumSoFar

class monomialSpecialExponent(Function):
    """Represents a monomial with a non-int exponent. 
       Characteristic is a list containing the exponent."""

    def __init__(self, characteristic):
        super().__init__(characteristic)

    def calculate(self, x):
        return math.pow(x, self.characteristic[0])

class exponentialFunction(Function):
    """Represents a exponential (base e) function.  Characteristic
       represents the polynomial in the exponent of the
       exponential function.  Characteristic 
       is a list containining coefficients of x^n terms
       from n = 0, to n = n.  Coefficients are written
       such that the degree of the x^n terms is 
       increasing."""

    def __init__(self, characteristic):
        super().__init__(characteristic)

    def calculate(self, x):
        f = polynomialFunction(self.characteristic)
        power = f.calculate(x)
        return math.exp(power)

class exponentialDiffBaseFunction(Function):
    """Represents a exponential function with a non-e base.
       characteristic[0] is the base.  The rest
       represents the polynomial in the exponent of the
       exponential function.  Characteristic 
       is a list containining coefficients of x^n terms
       from n = 0, to n = n.  Coefficients are written
       such that the degree of the x^n terms is 
       increasing."""

    def __init__(self, characteristic):
        super().__init__(characteristic)

    def calculate(self, x):
        f = polynomialFunction(self.characteristic[1:])
        power = f.calculate(x)
        return math.pow(self.characteristic[0], power)

class logarithmicFunction(Function):
    """Represents a logarithmic function.  Characteristic
        is a list containing the base at which logarithm is 
        taken at."""
    def __init__(self, characteristic):
        super().__init__(characteristic)

    def calculate(self, x):
        return math.log(x, self.characteristic[0])

class sinusoidalFunction(Function):
    """Represents a sinusoidal function.  Characteristic
       is one of ['sin'], ['cos'], or ['tan']."""

    def __init__(self, characteristic):
        super().__init__(characteristic)

    def calculate(self, x):
        if (self.characteristic[0] == 'sin'):
            return math.sin(x)
        elif (self.characteristic[0] == 'cos'):
            return math.cos(x)
        elif (self.characteristic[0] == 'tan'):
            return math.tan(x)
        else:
            pass

    

class coinTossFunction(Function):
    """Represents a coin toss function, which 
       returns 0 =< value =< 1.  Characteristic 
       is emptyy list, to preserve abstract class behaviour. 
       If x is 1, value returned is 0 or 1.
       Otherise, returns the ratio of 1s out of all the 
       coin tosses."""

    def __init__(self, characteristic = []):
        super().__init__(characteristic)

    def calculate(self, x):
        if(x == 1):
            return random.randint(0,1)
        else:
            noOfTosses = 0
            sumSoFar = 0
            for i in range(x):
                sumSoFar += random.randint(0,1)
                noOfTosses += 1
            return sumSoFar/noOfTosses

class customFunction(Function):
    
    """Represents a custom function that is composed
       of two other functions.  The relation between each 
       function can be '+', '-', '*', '/' or '^'.  This is
       represented in characterisitic as 
       [function 1, function 2, relation]. """

    def __init__(self, characteristic):
        super().__init__(characteristic)

    def calculate(self, x):
        f1atX = self.characteristic[0].calculate(x)
        f2atX = self.characteristic[1].calculate(x)

        if (self.characteristic[2] == '+'):
            return f1atX + f2atX
        elif (self.characteristic[2] == '-'):
            return f1atX - f2atX
        elif (self.characteristic[2] == '*'):
            return f1atX * f2atX
        elif (self.characteristic[2] == '/'):
            return f1atX / f2atX
        elif (self.characteristic[2] == '^'):
            return f1atX ** f2atX
        else:
            pass
