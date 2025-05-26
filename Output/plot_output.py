import matplotlib.pyplot as plt
import numpy as np

# Load the data
fid = input("Enter the filename: ")
data = np.loadtxt(fid)

# Create a figure with two subplots side by side
fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(12, 6))  # Adjust figsize as needed

# Plot on the first subplot (linear scale)
ax1.plot(data[:, 0], data[:, 1])
ax1.set_xlabel("Energy (keV)")
ax1.set_ylabel("Counts")
ax1.set_title("Linear Scale")

# Plot on the second subplot (logarithmic scale)
ax2.plot(data[:, 0], data[:, 1])
ax2.set_xlabel("Energy (keV)")
ax2.set_ylabel("Counts")
ax2.set_yscale("log")  # Set logarithmic scale
ax2.set_title("Logarithmic Scale")

# Adjust layout and display the plot
plt.tight_layout()  # Adjust subplot parameters to give specified padding
plt.show()
