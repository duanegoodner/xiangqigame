import matplotlib.pyplot as plt
import numpy as np

# Sample data: x values
x = np.linspace(1, 10, 100)

# Non-monotonic y values with extreme differences in magnitude and integer values
y1 = (np.abs(np.sin(x)) * 1e5).astype(int)   # Scaling by 10 and converting to int
y2 = (np.abs(np.cos(x)) * 1e3).astype(int)   # Scaling by 1000 and converting to int
y3 = (np.abs(np.sin(2 * x)) * 1e1).astype(int)  # Scaling by 100,000 and converting to int

# Introducing zeros into each data series at random indices
y1[::10] = 0  # Every 10th element is zero
y2[::15] = 0  # Every 15th element is zero
y3[::20] = 0  # Every 20th element is zero

# Store the series in a list with their labels and assign specific colors
series = [
    ('Series 1 (y1)', y1, 'blue'),  # Fixed color: blue for y1
    ('Series 2 (y2)', y2, 'orange'),  # Fixed color: orange for y2
    ('Series 3 (y3)', y3, 'green')   # Fixed color: green for y3
]

# Sort the series based on the maximum value of each data series
sorted_series = sorted(series, key=lambda s: np.max(s[1]))

# Extract the sorted y-values, labels, and colors after sorting
sorted_labels = [s[0] for s in sorted_series]
sorted_y_values = [s[1] for s in sorted_series]
sorted_colors = [s[2] for s in sorted_series]

# Plotting the stacked area plot
fig, ax = plt.subplots()

# Stacking the sorted data with fixed colors
ax.stackplot(x, *sorted_y_values, labels=sorted_labels, colors=sorted_colors)

# Setting the y-axis to log scale
ax.set_yscale('log')

# Adding labels and title
plt.xlabel('X axis')
plt.ylabel('Y axis (log scale)')
plt.title('Stacked Area Plot with Logarithmic Y-axis (Sorted by Magnitude)')

# Adding a legend in the right margin
ax.legend(loc='center left', bbox_to_anchor=(1, 0.5))

# Adjust the layout to make room for the legend
plt.tight_layout()

plt.show()
