#include <boost/math/distributions/uniform.hpp>
#include <iostream>
#include <vector>
#include <algorithm>

// Function to perform the Kolmogorov-Smirnov test
double kolmogorov_smirnov_test(const std::vector<double>& sample, const boost::math::uniform_distribution<>& dist) {
    std::vector<double> cdf_values;
    for (double x : sample) {
        cdf_values.push_back(boost::math::cdf(dist, x));
    }

    std::sort(cdf_values.begin(), cdf_values.end());

    double max_d = 0;
    for (size_t i = 0; i < cdf_values.size(); ++i) {
        double empirical_cdf = static_cast<double>(i + 1) / cdf_values.size();
        double d_plus = std::abs(empirical_cdf - cdf_values[i]);
        double d_minus = i == 0 ? cdf_values[i] : std::abs(cdf_values[i - 1] - empirical_cdf);
        max_d = std::max({max_d, d_plus, d_minus});
    }

    return max_d;
}

int main() {
    // Uniform distribution between 0 and 1
    boost::math::uniform_distribution<> my_uniform(0.0, 1.0);

    // Example sample data
    std::vector<double> data = {0.2, 0.5, 0.75, 0.1, 0.6, 0.8, 0.05, 0.7, 0.3};

    // Perform the KS test
    double ks_statistic = kolmogorov_smirnov_test(data, my_uniform);

    std::cout << "K-S Statistic: " << ks_statistic << std::endl;

    return 0;
}
