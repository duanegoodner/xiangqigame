import seaborn as sns
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

# Example dataframe with random integer data
np.random.seed(0)
data = pd.DataFrame(np.random.randint(0, 100, size=(10, 5)),
                    columns=['Column1', 'Column2', 'Column3', 'Column4', 'Column5'])
# Create the heatmap using seaborn
plt.figure(figsize=(10, 6))
sns.heatmap(data.T, cmap="RdYlBu_r", annot=True, fmt="d", cbar_kws={'label': 'Values'})

# Set axis labels and title
plt.xlabel('Index')
plt.ylabel('Columns')
plt.title('Heatmap of DataFrame')

plt.show()
