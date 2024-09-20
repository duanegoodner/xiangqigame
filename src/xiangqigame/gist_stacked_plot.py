import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from matplotlib.colors import LinearSegmentedColormap


def plot_stacked_log(df1, df2):
    """
    Generates a figure with two stacked area plots, each with a logarithmic y-axis, using increasingly darker shades of gray.
    Each column in both DataFrames is assigned a fixed color, and the DataFrame columns are sorted by their maximum magnitude
    for plotting in ascending order. A single legend is displayed in the right margin of the figure.

    Parameters:
    df1 : pandas.DataFrame
        First DataFrame to be plotted on the first subplot.
    df2 : pandas.DataFrame
        Second DataFrame to be plotted on the second subplot.
    """

    # Function to generate stacked plot on a given axis
    def create_stacked_plot(ax, df, title):
        num_columns = df.shape[1]

        # Create a colormap from light gray to black
        cmap = LinearSegmentedColormap.from_list("shades_of_gray", ["#d9d9d9", "#000000"], N=num_columns)

        # Assign a fixed color to each column based on the order of columns in the DataFrame
        column_colors = {col: cmap(i) for i, col in enumerate(df.columns)}

        # Sort the DataFrame columns by the maximum value in each column
        sorted_columns = df.max().sort_values().index

        # Extract sorted data and use the fixed colors for each column
        sorted_y_values = [df[col].values for col in sorted_columns]
        sorted_colors = [column_colors[col] for col in sorted_columns]

        # Stacking the sorted data with fixed colors (shades of gray)
        ax.stackplot(df.index, *sorted_y_values, colors=sorted_colors)

        # Setting the y-axis to log scale
        ax.set_yscale('log')

        # Adding labels and title
        ax.set_xlabel('X axis')
        ax.set_ylabel('Y axis (log scale)')
        ax.set_title(title)

        # Return the original column order and corresponding colors for the legend
        return df.columns, [column_colors[col] for col in df.columns]

    # Create a figure with two subplots
    fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(10, 12), sharex=True, sharey=True)

    # Plot each DataFrame on its respective subplot
    labels1, colors1 = create_stacked_plot(ax1, df1, 'Stacked Plot 1')
    labels2, colors2 = create_stacked_plot(ax2, df2, 'Stacked Plot 2')

    # Combine the labels and colors from both subplots for a single legend
    all_labels = list(labels1) + list(labels2)
    all_colors = list(colors1) + list(colors2)

    # Create the handles for the legend
    handles = [plt.Line2D([0], [0], color=all_colors[i], lw=4) for i in range(len(all_labels))]

    # Adding the legend to ax1, but placing it in the right margin
    ax1.legend(handles, all_labels, loc='center left', bbox_to_anchor=(1, 0.5))

    # Adjust the layout to ensure space for the legend
    plt.subplots_adjust(right=0.8)

    # Show the figure with both plots and the legend
    plt.show()


# Example usage with two DataFrames:

# Sample data: x values
x = np.linspace(1, 10, 100)

# Data for first DataFrame (df1)
y1 = (np.abs(np.sin(x)) * 1e1).astype(int)
y2 = (np.abs(np.cos(x)) * 1e3).astype(int)
y3 = (np.abs(np.sin(2 * x)) * 1e5).astype(int)
y1[::10] = 0
y2[::15] = 0
y3[::20] = 0
df1 = pd.DataFrame({'y1': y1, 'y2': y2, 'y3': y3}, index=x)

# Data for second DataFrame (df2)
y4 = (np.abs(np.sin(x)) * 2e1).astype(int)
y5 = (np.abs(np.cos(x)) * 2e3).astype(int)
y6 = (np.abs(np.sin(2 * x)) * 2e5).astype(int)
y4[::10] = 0
y5[::15] = 0
y6[::20] = 0
df2 = pd.DataFrame({'y4': y4, 'y5': y5, 'y6': y6}, index=x)

# Call the function to generate the figure with two stacked plots and one legend
plot_stacked_log(df1, df2)
