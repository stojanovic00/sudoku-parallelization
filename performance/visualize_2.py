import pandas as pd
import matplotlib.pyplot as plt

# Read the CSV file into a Pandas DataFrame, skipping the first row with data
df = pd.read_csv('results.csv', header=None, skiprows=1)

# Filter rows where the last column (4th column) is not equal to 0
filtered_df = df[df[4] != 0]

# Plotting
plt.figure(figsize=(10, 6))

# Plot the last column (4th column) with the first column ('thr/proc') as the x-axis
plt.plot(filtered_df[0], filtered_df[4], label='MPI')

# Set labels and title
plt.xlabel('Number of Processes')
plt.ylabel('Time (seconds)')
plt.title('MPI Execution Time vs Number of Processes')
plt.legend()

# Show the plot
plt.show()
