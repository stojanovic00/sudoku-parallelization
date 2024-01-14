import pandas as pd
import matplotlib.pyplot as plt

# Read the CSV file into a Pandas DataFrame
df = pd.read_csv('results.csv')

# Plotting
plt.figure(figsize=(10, 6))

# Iterate through columns (excluding the first column 'thr/proc')
for col in df.columns[1:4]:
    plt.plot(df['thr/proc'], df[col], label=col)

# Set labels and title
plt.xlabel('Number of Threads')
plt.ylabel('Time (seconds)')
plt.title('Execution Time vs Number of Threads')
plt.legend()

# Show the plot
plt.show()

