import matplotlib.pyplot as plt
import numpy as np
from matplotlib.axes import Axes
from typing import cast

fig, axes = plt.subplots(nrows=2, ncols=1)
axes = np.reshape(axes, (2, 1))

# Use typing.cast to explicitly cast to Axes
my_single_axes = cast(Axes, axes[0, 0])
