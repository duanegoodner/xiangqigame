import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit

# Example large dataset (simulated)
k_values = np.linspace(1000, 100000, 100)  # Larger range of entries


# observed_collisions = k_values**2 / (2 * 1e6)  # Simulated observed collisions for an example n = 1e6

increment_probabilities = np.linspace(0.01, 0.1, 100)
observed_collisions = np.zeros_like(k_values, dtype=int)
for i in range(1, len(k_values)):
    if np.random.rand() < increment_probabilities[i]:
        observed_collisions[i] = observed_collisions[i - 1] + 1 if observed_collisions[i - 1] < 4 else 4
    else:
        observed_collisions[i] = observed_collisions[i - 1]


# Define the model for expected collisions using Poisson approximation
def poisson_collision_model(k, n):
    return k**2 / (2 * n)

# Fit the model
params, params_covariance = curve_fit(poisson_collision_model, k_values, observed_collisions)

# Calculate fitted collisions using the optimized n
fitted_collisions = poisson_collision_model(k_values, *params)

# Plotting the results
plt.figure(figsize=(10, 6))
plt.scatter(k_values, observed_collisions, label='Simulated Observed Data')
plt.plot(k_values, fitted_collisions, label='Fitted Poisson Approximation Model', color='red')
plt.title('Fit of Poisson Approximation Collision Model to Data')
plt.xlabel('Number of Entries (k)')
plt.ylabel('Expected Number of Collisions')
plt.legend()
plt.grid(True)
plt.show()

# Print the fitted number of slots (n)
print("Fitted number of slots (n):", params[0])
