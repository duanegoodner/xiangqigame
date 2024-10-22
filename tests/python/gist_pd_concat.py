
import pandas as pd

# Example DataFrames
df1 = pd.DataFrame({
    'A': [1, 2, 3],
    'B': [4, 5, 6],
    'C': [7, 8, 9],
    'D': [10, 11, 12],
    'E': [13, 14, 15]
})
df2 = pd.DataFrame({
    'A': [16, 17, 18],
    'B': [19, 20, 21],
    'C': [22, 23, 24],
    'D': [25, 26, 27],
    'E': [28, 29, 30]
})
df3 = pd.DataFrame({
    'A': [31, 32, 33],
    'B': [34, 35, 36],
    'C': [37, 38, 39],
    'D': [40, 41, 42],
    'E': [43, 44, 45]
})

# List of DataFrames
dataframes = [df1, df2, df3]

# Columns to stack: 3 to 5 (Python uses 0-based indexing, so these are columns 'C', 'D', 'E')
selected_columns = dataframes[0].columns[2:5]

# Concatenate the selected columns from each DataFrame
stacked_df = pd.concat([df[selected_columns] for df in dataframes], axis=0)

# Show the result
print(stacked_df)