

import numpy as np;

SIZE = 20000

x = np.linspace(0, 1, SIZE)
y = np.random.standard_normal(SIZE)

y.sort()

pol = np.polyfit(x, y, 13)

print(repr(pol))
