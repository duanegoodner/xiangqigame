import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

# Generate sample data
np.random.seed(0)
data = {
    'A': range(1, 11),  # Column A with integers from 1 to 10
    'B': np.random.rand(10) * 100,  # Column B with random floats multiplied by 100
    'C': np.random.rand(10) * 200  # Column C with random floats multiplied by 200
}
df = pd.DataFrame(data)

# Create a new figure and a subplot
fig, ax1 = plt.subplots()

# Plotting B vs A with the primary y-axis (left)
color = 'tab:blue'
ax1.set_xlabel('A (int)')
ax1.set_ylabel('B (float)', color=color)
ax1.plot(df['A'], df['B'], color=color, marker='o')  # Added markers for visibility
ax1.tick_params(axis='y', labelcolor=color)

# Creating a second y-axis for C vs A
ax2 = ax1.twinx()  # instantiate a second axes that shares the same x-axis
color = 'tab:red'
ax2.set_ylabel('C (float)', color=color)  # we already handled the x-label with ax1
ax2.plot(df['A'], df['C'], color=color, marker='x')  # Added markers for visibility
ax2.tick_params(axis='y', labelcolor=color)

# To make the plot more readable, let's add a title and optionally display a grid
fig.tight_layout()  # to ensure tight layout so labels don't cut off
plt.title('Overlay Plot of B vs A and C vs A')
ax1.grid(True)

# Show the plot
plt.show()
