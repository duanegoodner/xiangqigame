import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

# Original DataFrame with 20 points and consecutive zeros for each series at different positions
data1 = {
    'Series A': [100, 200, 300, 400, 500, 0, 0, 0, 0, 600, 700, 800, 900, 1000, 1100, 1200, 1300, 1400, 1500, 1600],
    'Series B': [50, 40, 30, 80, 90, 100, 110, 0, 0, 0, 0, 150, 160, 170, 180, 190, 200, 210, 220, 230],
    'Series C': [200, 250, 300, 350, 0, 0, 0, 0, 400, 450, 500, 550, 600, 650, 700, 750, 800, 850, 900, 950]
}

# New DataFrame with values of columns A and B swapped
data2 = {
    'Series A': [80, 90, 100, 110, 0, 0, 0, 150, 160, 170, 180, 190, 0, 200, 210, 220, 230, 50, 40, 30],  # Previously Series B values
    'Series B': [300, 0, 400, 200, 100, 500, 600, 0, 0, 1000, 1100, 800, 1200, 1300, 700, 1400, 1500, 600, 200, 0],  # Previously Series A values
    'Series C': [100, 200, 300, 0, 400, 0, 0, 450, 500, 550, 600, 650, 700, 750, 800, 0, 0, 250, 300, 350]
}

# Create DataFrames with custom index
index = pd.date_range('2024-01-01', periods=20, freq='D')
df1 = pd.DataFrame(data1, index=index)
df2 = pd.DataFrame(data2, index=index)

# Assign colors using 'tab10' colormap for consistent colors across both DataFrames
cmap = plt.get_cmap('tab10')
column_colors = {col: cmap(i) for i, col in enumerate(df1.columns)}  # Assign fixed colors to columns

# Sort each DataFrame by total magnitude (sum of each series) in ascending order
df1_sorted = df1[sorted(df1.columns, key=lambda col: df1[col].sum())]
df2_sorted = df2[sorted(df2.columns, key=lambda col: df2[col].sum())]

# Create a figure with two subplots (one for each DataFrame)
fig, axes = plt.subplots(1, 2, figsize=(15, 6))

# Data for x-axis: use DataFrame index
x = df1.index

# Plot the first DataFrame (df1)
colors = [column_colors[col] for col in df1_sorted.columns]  # Use pre-assigned colors
axes[0].stackplot(x, df1_sorted.T, labels=df1_sorted.columns, colors=colors)
axes[0].set_title("DataFrame 1")
axes[0].tick_params(axis='x', rotation=45)
axes[0].legend(loc='upper left')  # Add legend

# Plot the second DataFrame (df2) with swapped values for Series A and B
colors = [column_colors[col] for col in df2_sorted.columns]  # Use the same pre-assigned colors
axes[1].stackplot(x, df2_sorted.T, labels=df2_sorted.columns, colors=colors)
axes[1].set_title("DataFrame 2 (A & B Swapped)")
axes[1].tick_params(axis='x', rotation=45)
axes[1].legend(loc='upper left')  # Add legend

# Adjust layout to avoid overlap
plt.tight_layout()

# Show the plot
plt.show()
