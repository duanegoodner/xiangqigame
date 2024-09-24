import matplotlib.pyplot as plt
import matplotlib.gridspec as gridspec
import numpy as np

# Create a figure
fig = plt.figure(figsize=(12, 16))

# Create a 4x4 grid layout
gs = gridspec.GridSpec(
    7, 2, figure=fig, height_ratios=[1, 1, 0.2, 1, 1, 0.3, 1]
)

search_type_plots = np.empty(shape=(2, 2), dtype=object)
search_time_plots = np.empty(shape=(2, 2), dtype=object)
eval_score_plots = np.empty(shape=(1, 2), dtype=object)

for gs_row in range(2):
    for gs_col in range(2):
        search_type_plots[gs_row, gs_col] = fig.add_subplot(gs[gs_row, gs_col])

for gs_row in range(3, 5):
    for gs_col in range(2):
        search_time_plots[gs_row - 3, gs_col] = fig.add_subplot(
            gs[gs_row, gs_col]
        )

for gs_row in range(6, 7):
    for gs_col in range(2):
        eval_score_plots[gs_row - 6, gs_col] = fig.add_subplot(
            gs[gs_row, gs_col]
        )

search_type_plots[0, 0].plot([1, 2, 3], [4, 5, 6])
search_time_plots[0, 0].plot([4, 5, 6], [3, 2, 1])
eval_score_plots[0, 1].plot([1, 2, 3], [4, 4, 4])

# Display the figure
plt.tight_layout()
plt.show()
