import matplotlib.pyplot as plt

# Sample data for red and black team for 6 parameters
moves = list(range(1, 51))

# Generate some example data for 6 parameters
red_team_param1 = [2 * x + 10 for x in moves]
black_team_param1 = [1.8 * x + 12 for x in moves]

red_team_param2 = [1.5 * x + 15 for x in moves]
black_team_param2 = [1.2 * x + 14 for x in moves]

red_team_param3 = [3 * x + 5 for x in moves]
black_team_param3 = [2.5 * x + 7 for x in moves]

red_team_param4 = [4 * x + 8 for x in moves]
black_team_param4 = [3.8 * x + 10 for x in moves]

red_team_param5 = [1.8 * x + 18 for x in moves]
black_team_param5 = [1.6 * x + 16 for x in moves]

red_team_param6 = [2.3 * x + 9 for x in moves]
black_team_param6 = [2.1 * x + 10 for x in moves]

# Apply a seaborn style without importing seaborn
plt.style.use('bmh')

# Create a 2x3 grid of subplots
fig, axes = plt.subplots(2, 3, figsize=(15, 10))

# Define colors for the teams
red_team_color = 'red'
black_team_color = 'black'

# Plot each parameter in the grid
axes[0, 0].plot(moves, red_team_param1, color=red_team_color, marker='o')
axes[0, 0].plot(moves, black_team_param1, color=black_team_color, marker='o')
axes[0, 0].set_xlabel('Move Number')
axes[0, 0].set_ylabel('Parameter 1')

axes[0, 1].plot(moves, red_team_param2, color=red_team_color, marker='o')
axes[0, 1].plot(moves, black_team_param2, color=black_team_color, marker='o')
axes[0, 1].set_xlabel('Move Number')
axes[0, 1].set_ylabel('Parameter 2')

axes[0, 2].plot(moves, red_team_param3, color=red_team_color, marker='o')
axes[0, 2].plot(moves, black_team_param3, color=black_team_color, marker='o')
axes[0, 2].set_xlabel('Move Number')
axes[0, 2].set_ylabel('Parameter 3')

axes[1, 0].plot(moves, red_team_param4, color=red_team_color, marker='o')
axes[1, 0].plot(moves, black_team_param4, color=black_team_color, marker='o')
axes[1, 0].set_xlabel('Move Number')
axes[1, 0].set_ylabel('Parameter 4')

axes[1, 1].plot(moves, red_team_param5, color=red_team_color, marker='o')
axes[1, 1].plot(moves, black_team_param5, color=black_team_color, marker='o')
axes[1, 1].set_xlabel('Move Number')
axes[1, 1].set_ylabel('Parameter 5')

axes[1, 2].plot(moves, red_team_param6, color=red_team_color, marker='o')
axes[1, 2].plot(moves, black_team_param6, color=black_team_color, marker='o')
axes[1, 2].set_xlabel('Move Number')
axes[1, 2].set_ylabel('Parameter 6')

# Create a single legend for the entire figure
red_line = plt.Line2D([0], [0], color=red_team_color, marker='o', linestyle='', label='Red Team')
black_line = plt.Line2D([0], [0], color=black_team_color, marker='o', linestyle='', label='Black Team')

# Add the legend to the figure, outside the plots on the right
fig.legend(handles=[red_line, black_line], loc='center right', borderaxespad=1)

# Add a single title for the entire figure
fig.suptitle('Move Number vs Various Parameters for Red and Black Teams', fontsize=16)

# Adjust layout to make space for the legend
plt.subplots_adjust(right=0.85)

# Show the plot
plt.show()
