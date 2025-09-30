import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

file_in = 'csv/transfer10k.csv'

csv = pd.read_csv(file_in, header = 0, names = ['out', ''], sep = ';', decimal = ',')

y = csv['out'].to_numpy()
y -= y[300] # remove DC offset by a center point
y /= -y[0] # normalize to 0db and flip the polarity
x = np.linspace(-3, 3, y.size)

# This transfer curve is assymmetrical
# From top signal is clipped very gradually
# From bottom signal is almost hard-clipped

split = 390 # this is where the hard-clipping happens

# Approximation of this curve will consist of two stages:
# 1st stage approximates the hard-clipping part
# 2nd stage approximates the soft-clipping part

# 1st stage can be made with a clipper function with a variable knee
# 2nd stage can be made with a polynomial approximation

### 1st stage ###
print('### 1st stage ###')

clip = y.max()

def f(x): return x - x**n

n = 15
k = (1 / n) ** (1 / (n - 1))
l = f(k)
scale = 1

print('n:', n)
print('prescale: (x + 1) * 0.5 * 0.95')
print('postscale: 2 * x - 1')

clip_max = clip
clip_min = -3
clip_span = clip_max - clip_min

def prescale(x): return (x + 1) / 2 * 0.95
def postscale(x): return 2 * x - 1

def f1(x): return f(x * k) / l
def f2(x):
    x = prescale(x)
    y0 = np.sign(x)
    y1 = f1(x)

    mask = np.less(x, 1/scale) * np.greater(x, -1/scale)

    return postscale(y1 * mask + y0 * ~mask)

plt.plot(y)
plt.plot(f2(np.linspace(-3, 3, y.size)) * clip)
plt.show()

### 2nd stage ###
print('### 2nd stage ###')

# Let's build a curve for the positive clipper
der2 = np.diff(y, 2) # Second order derivative of the curve
# After the breakpoint, derivative is made constant, this removes the hard-clipping part
der2[split:] = der2[split]
# Then it's integrated
der = np.cumsum(der2)
# And integrated again. Two orders of integration ensure that the curve is still curved. 
# With one order of derivation/integration, after-the-breakpoint part would be linear
y1 = np.cumsum(der) + y[0]
x1 = x[:y1.size]

# Polynomial fitting
pol = np.polyfit(x1, y1, 13)
offset = np.polyval(pol, 0)
# Rinse and repeat to remove DC offset
pol = np.polyfit(x1, y1 - offset, 13)
offset = np.polyval(pol, 0)

p = np.polyval(pol, x)

print('size:', y1.size)
print('offset:', np.polyval(pol, 0))
print('polynomial:')
print(pol)

plt.plot(y)
plt.plot(p)
plt.show()

### Combined ###
print('### Combined ###')

plt.plot(y)
plt.plot(np.polyval(pol, f2(np.linspace(-3, 3, y.size))))
plt.show()

#print((p[300] - p[301]) * 100)
#print(pol)