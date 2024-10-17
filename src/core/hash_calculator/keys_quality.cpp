#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>

// Function to calculate the Mean, Standard Deviation, and Coefficient of Variation
template <typename KeyType>
void analyzeKeyDistribution(const std::vector<KeyType>& keys) {
    if (keys.empty()) {
        std::cout << "No keys to analyze." << std::endl;
        return;
    }

    double sum = std::accumulate(keys.begin(), keys.end(), 0.0);
    double mean = sum / keys.size();
    double sq_sum = std::accumulate(keys.begin(), keys.end(), 0.0, [mean](double acc, KeyType key) {
        return acc + (key - mean) * (key - mean);
    });
    double variance = sq_sum / keys.size();
    double std_dev = std::sqrt(variance);
    double coefficient_of_variation = std_dev / mean;

    std::cout << "Mean: " << mean << std::endl;
    std::cout << "Standard Deviation: " << std_dev << std::endl;
    std::cout << "Coefficient of Variation: " << coefficient_of_variation << std::endl;
}

// Function to perform the Kolmogorov-Smirnov test against a uniform distribution
template <typename KeyType>
double kolmogorovSmirnovTest(const std::vector<KeyType>& keys) {
    if (keys.empty()) return -1;  // Handle empty vector case

    std::vector<KeyType> sorted_keys = keys;
    std::sort(sorted_keys.begin(), sorted_keys.end());

    double D = 0.0;
    double theoretical_cdf;
    double n = static_cast<double>(sorted_keys.size());
    KeyType min_key = sorted_keys.front();
    KeyType max_key = sorted_keys.back();

    for (KeyType key : sorted_keys) {
        theoretical_cdf = static_cast<double>(key - min_key) / (max_key - min_key);
        double ecdf_val = (std::upper_bound(sorted_keys.begin(), sorted_keys.end(), key) - sorted_keys.begin()) / n;
        D = std::max(D, std::abs(ecdf_val - theoretical_cdf));
    }

    return D;
}

template <typename KeyType>
double kolmogorovSmirnovFullRangeTest(const std::vector<KeyType>& keys) {
    if (keys.empty()) return -1;  // Handle empty vector case

    std::vector<KeyType> sorted_keys = keys;
    std::sort(sorted_keys.begin(), sorted_keys.end());

    double D = 0.0;
    double n = static_cast<double>(sorted_keys.size());

    // Use the full range of the data type
    KeyType min_key = 0;  // Start of the range
    KeyType max_key = std::numeric_limits<KeyType>::max();  // End of the range

    for (KeyType key : sorted_keys) {
        double theoretical_cdf = static_cast<double>(key - min_key) / (max_key - min_key);
        double ecdf_val = (std::upper_bound(sorted_keys.begin(), sorted_keys.end(), key) - sorted_keys.begin()) / n;
        D = std::max(D, std::abs(ecdf_val - theoretical_cdf));
    }

    return D;
}

// Function to calculate the Nearest Neighbor Index
template <typename KeyType>
double nearestNeighborIndex(const std::vector<KeyType>& keys) {
    if (keys.size() < 2) return -1;

    std::vector<KeyType> sorted_keys = keys;
    std::sort(sorted_keys.begin(), sorted_keys.end());

    std::vector<double> distances;
    for (size_t i = 1; i < sorted_keys.size(); ++i) {
        distances.push_back(static_cast<double>(sorted_keys[i] - sorted_keys[i - 1]));
    }

    double mean_distance = std::accumulate(distances.begin(), distances.end(), 0.0) / distances.size();
    double range = static_cast<double>(sorted_keys.back() - sorted_keys.front());
    double expected_distance = range / (keys.size() + 1);

    return mean_distance / expected_distance;
}

int main() {
    std::vector<uint64_t> keys = {10, 20, 30, 40, 50, 60, 70, 80, 90, 00};

    std::cout << "Analyzing Key Distribution:\n";
    analyzeKeyDistribution(keys);

    double D = kolmogorovSmirnovTest(keys);
    std::cout << "Kolmogorov-Smirnov Statistic (D): " << D << std::endl;

    double D_full = kolmogorovSmirnovFullRangeTest(keys);
    std::cout << "Kolmogorov-Smirnov Statistic with full range (D): " << D << std::endl;

    double NNI = nearestNeighborIndex(keys);
    std::cout << "Nearest Neighbor Index (NNI): " << NNI << std::endl;

    return 0;
}
