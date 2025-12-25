def nearest_in_list(lst, value):
    return min(lst, key=lambda x: abs(abs(x) - abs(value)))

def tf2sos(tf):
     #separate numerator and denominator polynomials
     num, den = sp.fraction(H)
     pnum = sp.Poly(num, s)
     pden = sp.Poly(den, s)

     poles = sp.nroots(den)
     zeros = sp.nroots(num)

     real_zeros = []
     complex_zeros = []

     for zero in zeros:
          if zero.is_real: real_zeros.append(float(zero))
          else: complex_zeros.append(complex(zero))

     real_zeros.sort()
     poles.sort()

     biquads = []

     for z1, z2 in zip(complex_zeros[::2], complex_zeros[1::2]):
          zero_frequency = np.abs(z1)

          p1 = nearest_in_list(poles, zero_frequency)
          poles.remove(p1)

          p2 = nearest_in_list(poles, z2)
          poles.remove(p2)

          biquads.append(np.array([(z1 * z2).real, (-z1 - z2).real, 1, p1 * p2, -p1 - p2, 1], dtype = float))

     for z1, z2 in zip(real_zeros[::2], real_zeros[1::2]):
          real_zeros.remove(z1)
          real_zeros.remove(z2)

          mean = (z1 + z2) / 2

          p1 = nearest_in_list(poles, mean)
          poles.remove(p1)
          p2 = nearest_in_list(poles, mean)
          poles.remove(p2)

          biquads.append(np.array([z1 * z2, -z1 - z2, 1, p1 * p2, -p1 - p2, 1], dtype = float))

     for z in real_zeros: # basically takes the last remaining zero if there's one left
          real_zeros.remove(z)

          p1 = nearest_in_list(poles, z)
          poles.remove(p1)
          p2 = nearest_in_list(poles, z)
          poles.remove(p2)

          biquads.append(np.array([-z, 1.0, 0.0, p1 * p2, -p1 - p2, 1], dtype = float))

     for p1, p2 in zip(poles[::2], poles[1::2]):
          poles.remove(p1)
          poles.remove(p2)

          biquads.append(np.array([1.0, 0.0, 0.0, p1 * p2, -p1 - p2, 1], dtype = float))

     for p in poles: # takes the last remaining pole if there's one left
          poles.remove(p)

          biquads.append(np.array([1.0, 0.0, 0.0, -p, 1.0, 0.0], dtype = float))

     k = pnum.LC() / pden.LC() # gain
     k_root = float(k ** (1 / len(biquads))) # gain distributed for each biquad

     for coeffs in biquads:
          coeffs[:3] *= k_root
          coeffs /= coeffs[3]

     print('zeros left:', real_zeros)
     print('poles left:', poles)
     print(biquads)
     return biquads

netlist = '''
C1 /A9 /B9
C2 /B1 /D1
C3 /B2 /D2
C4 /B3 /D3
C5 /B4 /D4
C6 /B5 /D5
C7 /B6 /D6
C8 /D1 0
C9 /D2 0
C10 /D3 0
C11 /D4 0
C12 /D5 0
C13 /D6 0
C14 /D7 0
C15 /D8 0
C16 /D9 0
R1 /E /A1
R2 /E /A2
R3 /E /A3
R4 /E /A4
R5 /E /A5
R6 /E /A6
R7 /A1 /B1
R8 /A2 /B2
R9 /A3 /B3
R10 /A4 /B4
R11 /A5 /B5
R12 /A6 /B6
R13 /D9 /B9
R14 /D1 /D2
R15 /D2 /D3
R16 /D3 /D4
R17 /D4 /D5
R18 /D5 /D6
R19 /D6 /D7
R20 /D7 /D8
R21 /D8 /D9
Rload /A9 0 10e3
P2 /A9 0
'''
#resistor values for each rank
rvalues = [
     [100e3, 100e3, 100e3, 100e3, 100e3, 100e3, 22e3, 22e3, 22e3, 33e3, 27e3, 47e3, 5.6e3, 15e3, 15e3, 15e3, 15e3, 15e3, 15e3, 15e3, 15e3],#16′
     [100e3, 100e3, 100e3, 100e3, 100e3, 100e3, 22e3, 33e3, 33e3, 22e3, 47e3, 47e3, 5.6e3, 15e3, 15e3, 15e3, 15e3, 15e3, 15e3, 15e3, 15e3],#8′
     [100e3, 100e3, 100e3, 100e3, 100e3, 100e3, 33e3, 47e3, 47e3, 58e3, 47e3, 82e3, 5.6e3, 15e3, 15e3, 15e3, 15e3, 15e3, 15e3, 15e3, 15e3],#4′
     [100e3, 100e3, 100e3, 100e3, 100e3, 100e3, 47e3, 33e3, 47e3, 47e3, 22e3, 22e3, 5.6e3, 15e3, 15e3, 15e3, 15e3, 15e3, 15e3, 15e3, 15e3],#2′
     [100e3, 100e3, 100e3, 100e3, 100e3, 100e3, 22e3, 27e3, 22e3, 33e3, 27e3, 100e3, 5.6e3, 15e3, 15e3, 15e3, 15e3, 15e3, 15e3, 15e3, 15e3],#5⅔′
     [100e3, 100e3, 100e3, 100e3, 100e3, 100e3, 47e3, 47e3, 47e3, 47e3, 22e3, 47e3, 1, 15e3, 15e3, 15e3, 15e3, 15e3, 15e3, 15e3, 15e3]#1⅗′
]

#capacitor values for each rank
cvalues = [
     [0.1e-6, 22e-9, 15e-9, 10e-9, 6.8e-9, 3.3e-9, 1.5e-9, 0.1e-6, 22e-9, 22e-9, 22e-9, 22e-9, 22e-9, 22e-9, 22e-9, 10e-9],#16′
     [47e-9, 10e-9, 6.8e-9, 4.7e-9, 2.2e-9, 1.5e-9, 680e-12, 47e-9, 10e-9, 10e-9, 10e-9, 10e-9, 10e-9, 10e-9, 10e-9, 4.7e-9],#8′
     [47e-9, 4.7e-9, 4.7e-9, 2.2e-9, 2.2e-9, 680e-12, 470e-12, 22e-9, 4.7e-9, 4.7e-9, 4.7e-9, 4.7e-9, 4.7e-9, 4.7e-9, 4.7e-9, 4.7e-9],#4′
     [10e-9, 22e-9, 1e-9, 1e-9, 560e-12, 560e-12, 330e-12, 6.8e-9, 2.2e-9, 2.2e-9, 2.2e-9, 2.2e-9, 2.2e-9, 2.2e-9, 2.2e-9, 2.2e-9],#2′
     [47e-9, 6.8e-9, 4.7e-9, 3.3e-9, 2.2e-9, 1e-9, 680e-12, 22e-9, 10e-9, 6.8e-9, 10e-9, 6.8e-9, 6.8e-9, 6.8e-9, 6.8e-9, 4.7e-9],#5⅔′
     [10e-9, 2.2e-9, 1e-9, 1e-9, 560e-12, 560e-12, 330e-12, 5.7e-9, 2.2e-9, 2.2e-9, 2.2e-9, 2.2e-9, 2.2e-9, 2.2e-9, 2.2e-9, 2.2e-9]#1⅗′
]

#rank labels for logging
ranks = ['16', '8', '4', '2', '5', '1']

#component labels used in the netlist, will be used to set component values
rlabels = [f'R{i+1}' for i in range(0, 21)]
clabels = [f'C{i+1}' for i in range(0, 16)]
inputs = [f'/A{i+1}' for i in range(0, 6)]

import lcapy as lp
import sympy as sp
from sympy.polys.polyroots import root_factors
s, z, T = sp.symbols('s z T')

import numpy as np
import matplotlib.pyplot as plt

NUMBER_OF_RANKS = 6
NUMBER_OF_INPUTS = 6

cct = lp.Circuit(netlist)

data_string = ''

for i in range(NUMBER_OF_RANKS):
     _rvalues = rvalues[i]
     _cvalues = cvalues[i]
     _rank = ranks[i]
     print('Beginning rank: '+_rank)

     #substitute numeric R and C values for this rank
     _cct = cct.copy()
     _cct = _cct.subs(dict(zip(rlabels, _rvalues)))
     _cct = _cct.subs(dict(zip(clabels, _cvalues)))

     for _input in inputs:
          _input_string = str(_input)
          _log_string = _rank + _input_string + ' '

          print(_rank + ' input:', _input)
          _cct2 = _cct.copy()
          _cct2.add(f'P1 {_input} 0')

          #compute the analog transfer function H(s)
          H = _cct2.P1.transfer('P2').expr.evalf()
          H = H.xreplace({list(H.free_symbols)[0]: s})
          #print(_log_string + 'analog transfer function ready')

          analog_sos_coeffs = tf2sos(H)
          
          #frequencies for frequency responses
          _s = np.linspace(1,20e3,20000) * 2j * np.pi
          _s2 = _s**2
          _z = (2 + _s * T) / (2 - _s * T)
          _z2 = _z**2

          #transfer function of analog sos cascade
          analog_sos_transfer = 1
          for coeffs in analog_sos_coeffs:
               analog_sos_transfer *= (coeffs[0] + coeffs[1] * _s + coeffs[2] * _s2) / (coeffs[3] + coeffs[4] * _s + coeffs[5] * _s2)

          analog_transfer_lambda = sp.lambdify(s, H, 'numpy')
          analog_transfer = analog_transfer_lambda(_s)

          plt.xscale('log')
          plt.plot(np.log10(np.abs(analog_transfer)) * 20)
          plt.plot(np.log10(np.abs(analog_sos_transfer)) * 20)
          plt.savefig('python/filter-rc-network/' + _rank + _input[1:] + '.png')
          plt.clf()

          coeffs_string = '\n{\n'
          #output analog coefficients
          for coeffs in analog_sos_coeffs:
               coeffs_string += '{ '
               coeffs_string += np.array2string(coeffs, precision=10, max_line_width=120, separator=', ')[1:-1]
               coeffs_string += ' },\n'
          
          coeffs_string = coeffs_string[:-2]
          coeffs_string += '\n},\n'

          print(coeffs_string)

          data_string += coeffs_string

data_string = data_string[:-2]
file_string = f"""#pragma once

#include "dsp/filter/Biquad.h"

// THIS FILE IS AUTOMATICALLY GENERATED BY python/filter-rc-network/script.py

namespace Ath::Forma
{{

static  Dsp::Filter::Biquad::AnalogBiquadCoefficients<double> BIQUAD_COEFFS[36][8] = 
{{
{data_string}
}};
}}
"""

with open('source/FormaBiquadCoefficients.h', "w") as f:
  f.write(file_string)