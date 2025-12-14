def group_pairs(roots):
    #sort roots by real part, then imaginary part
    roots = sorted(roots, key=lambda r: (complex(r.evalf()).real, complex(r.evalf()).imag))
    pairs = []
    used = set() #indices of roots already paired
    for i, r in enumerate(roots):
        if i in used:
            continue
        
        #treat (numerically) real roots as first-order factors
        if abs(sp.im(r)) < 1e-12:
            pairs.append([r])  # real root
            used.add(i)
        else:
            #match with conjugate partner
            for j in range(i+1, len(roots)):
                if j in used: continue
                if abs(r.conjugate() - roots[j]) < 1e-12:
                    pairs.append([r, roots[j]])
                    used.add(i); used.add(j)
                    break
    return pairs

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
import scipy
from scipy.signal import tf2sos, freqz_sos
import matplotlib.pyplot as plt

NUMBER_OF_RANKS = 6
NUMBER_OF_INPUTS = 6

cct = lp.Circuit(netlist)

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
          print(_log_string + 'analog transfer function ready')

          #separate numerator and denominator polynomials
          num, den = sp.fraction(H)
          pnum = sp.Poly(num, s)
          pden = sp.Poly(den, s)

          poles = sp.nroots(den)
          zeros = sp.nroots(num)

          pole_pairs = group_pairs(poles)
          zero_pairs = group_pairs(zeros)

          n = max(len(pole_pairs), len(zero_pairs))
          second_order_sections = []
          analog_sos_coeffs = []
          digital_sos_coeffs = []

          #restore overall gain from leading coefficients
          k = pnum.LC() / pden.LC()
          k_root = float(k ** (1 / n)) #this will be the gain for every sos section, which will improve precision over single huge gain

          for i in range(n):
               ppair = pole_pairs[i] if i < len(pole_pairs) else []
               zpair = zero_pairs[i] if i < len(zero_pairs) else []
               
               #denominators
               if len(ppair) == 2:
                    p1, p2 = ppair
                    a2, a1, a0 = 1, -(p1+p2), p1*p2
               elif len(ppair) == 1:
                    p = ppair[0]
                    a2, a1, a0 = 0, 1, -p
               else:
                    a2, a1, a0 = 0, 0, 1

               #numerators
               if len(zpair) == 2:
                    z1, z2 = zpair
                    b2, b1, b0 = 1, -(z1+z2), z1*z2
               elif len(zpair) == 1:
                    z = zpair[0]
                    b2, b1, b0 = 0, 1, -z
               else:
                    b2, b1, b0 = 0, 0, 1

               #normalize everything by a0
               b0 = (b0 / a0).evalf()
               b1 /= a0
               b2 /= a0

               a1 /= a0
               a2 /= a0
               a0 = 1

               #multiply numerator by nth root of overall gain
               b0 *= k_root
               b1 *= k_root
               b2 *= k_root

               second_order_sections.append(sp.simplify((b2*s**2 + b1*s + b0)/(a2*s**2 + a1*s + a0)))
               analog_sos_coeffs.append(np.array([b0, b1, b2, a0, a1, a2]).astype(np.float32))


          #sanity check
          #prod = sp.prod(second_order_sections)
          #print(prod.subs(s, 1).evalf())
          #print(H.subs(s, 1).evalf())

          #calculate bilinear transform for testing

          SAMPLE_RATE = 48000
          T = 1 / SAMPLE_RATE
          T2 = T * T

          K = 2 / T
          K2 = K * K

          #bilinear transform of soses
          for coeffs in analog_sos_coeffs:
               b0, b1, b2, a0, a1, a2 = coeffs

               B0 = b2*K2 + b1*K + b0
               B1 = -2*b2*K2 + 2*b0
               B2 = b2*K2 - b1*K + b0

               A0 = a2*K2 + a1*K + a0
               A1 = -2*a2*K2 + 2*a0
               A2 = a2*K2 - a1*K + a0

               digital = np.array([B0, B1, B2, A0, A1, A2], dtype=np.float32)
               digital /= digital[3]   #normalize by A0

               digital_sos_coeffs.append(digital)

          #frequencies for frequency responses
          _s = np.linspace(1,20e3,20000) * 2j * np.pi
          _s2 = _s**2
          _z = (2 + _s * T) / (2 - _s * T)
          _z2 = _z**2

          #transfer function of analog sos cascade
          analog_sos_transfer = 1
          for coeffs in analog_sos_coeffs:
               analog_sos_transfer *= (coeffs[0] + coeffs[1] * _s + coeffs[2] * _s2) / (coeffs[3] + coeffs[4] * _s + coeffs[5] * _s2)

          #transfer function of digital sos cascade
          digital_sos_transfer = 1
          for coeffs in digital_sos_coeffs:
               digital_sos_transfer *= (coeffs[0] + coeffs[1] * _z + coeffs[2] * _z2) / (coeffs[3] + coeffs[4] * _z + coeffs[5] * _z2)

          analog_transfer_lambda = sp.lambdify(s, H, 'numpy')
          analog_transfer = analog_transfer_lambda(_s)

          plt.xscale('log')
          plt.plot(np.log10(np.abs(analog_transfer)) * 20)
          plt.plot(np.log10(np.abs(analog_sos_transfer)) * 20)
          plt.plot(np.log10(np.abs(digital_sos_transfer)) * 20)
          plt.savefig('python/filter-rc-network/' + _rank + _input[1:] + '.png')
          plt.clf()