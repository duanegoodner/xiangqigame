from scipy.stats import kstest
import numpy as np
import xiangqi_bindings as bindings


if __name__ == "__main__":

    zkeys_64 = bindings.ZobristCalculator64()
    zkeys_128 = bindings.ZobristCalculator128()

    # Example usage with numpy array of data
    data = np.array([10, 20, 50, 60, 90, 100, 120, 150, 180, 200])
    # Run KS test against a uniform distribution
    ks_statistic, p_value = kstest(
        data, "uniform", args=(data.min(), data.max() - data.min())
    )

    print(f"KS Statistic: {ks_statistic}, P-value: {p_value}")
