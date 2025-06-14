import numpy as np

from numpy import array2string
def write_array_ubyte(file_out, data):
    _array =  array2string(data, 
                           max_line_width = 100, 
                           separator = ', ', 
                           threshold = 100000
                           )    
    _array = _array.replace('[', '')
    _array = _array.replace(']', '')
    
    with open(file_out, 'w') as f:
        f.write(_array)

def file_to_numerical(file_in, file_out):
    data = np.fromfile(file_in, np.ubyte)
    write_array_ubyte(file_out, data)

file_to_numerical("./RussoOne-Custom.otf", "RussoOne-Custom.data")
file_to_numerical("./Oxanium-Bold.ttf", "Oxanium-Bold.data")