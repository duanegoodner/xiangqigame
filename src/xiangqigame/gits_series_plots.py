import pandas as pd
import matplotlib.pyplot as plt

# Sample data (replace this with your DataFrame)
data = {
    'Column1': [1, 2, 3, 4, 5],
    'Column2': [5, 4, 3, 2, 1]
}
df = pd.DataFrame(data)

# Create the plot
plt.figure(figsize=(8, 6))

# Plot Column1 with a specific color (e.g., red)
plt.plot(df.index, df['Column1'], label='Column 1', color='red')

# Plot Column2 with a different color (e.g., blue)
plt.plot(df.index, df['Column2'], label='Column 2', color='blue')

# Add legend to differentiate between the two columns
plt.legend()

# Show the plot
plt.show()
