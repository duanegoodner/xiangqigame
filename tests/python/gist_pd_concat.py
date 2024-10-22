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

# List of original DataFrames
dataframes = [df1, df2, df3]

# Make copies of the DataFrames and reset index
copies = [df.copy() for df in dataframes]
for i, df in enumerate(copies):
    df.reset_index(inplace=True)
    df.rename(columns={'index': 'Original_Index'}, inplace=True)

# Columns to stack: include 'Original_Index' and columns 3 to 5 (Python uses 0-based indexing)
selected_columns = ['Original_Index'] + copies[0].columns[3:6].tolist()

# Concatenate the selected columns from each DataFrame copy with keys
stacked_df = pd.concat([df[selected_columns] for df in copies], keys=['1', '2', '3'], axis=0)

# Reset index to bring the keys into a column and clean up the DataFrame, dropping the multi-level index
stacked_df.reset_index(level=0, inplace=True)
stacked_df.rename(columns={'level_0': 'Source_ID'}, inplace=True)

# Additional step: Reset index again to make it monotonically increasing across the entire DataFrame
stacked_df.reset_index(drop=True, inplace=True)

# Show the result
print(stacked_df)
