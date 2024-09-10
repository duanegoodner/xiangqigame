#include <iostream>
#include <map>
#include <vector>

const int kNumRows = 3;
const int kNumCols = 5;

struct SearchResult {
  SearchResult()
      : result_counts{} {
    result_counts.reserve(kNumRows);
    for (auto idx = 0; idx < kNumRows; idx++) {
      result_counts.emplace_back(kNumCols, 0);
    }
  }

  void UpdateCounts(int row, int col) { result_counts[row][col]++; }

  std::vector<std::vector<int>> result_counts;
};

struct SearchResultCollections {
  std::vector<SearchResult> primary_searches;
  std::map<int, SearchResult> extra_searches;

  SearchResult &NewPrimarySearch() {
    primary_searches.emplace_back();
    return primary_searches.back();
  }

  SearchResult &NewExtraSearch(int primary_search_idx) {
    auto new_search_entry = extra_searches.emplace(primary_search_idx, SearchResult());
    return new_search_entry.first->second;
  }
};

int main() {
  SearchResultCollections my_results{};

  auto &first_primary_search = my_results.NewPrimarySearch();
  first_primary_search.UpdateCounts(2, 2);

  auto &first_extra_search = my_results.NewExtraSearch(1);
  first_extra_search.UpdateCounts(2, 2);

  std::cout << "first_primary_search counts element (2, 2) = "
            << my_results.primary_searches[0].result_counts[2][2] << std::endl;

  std::cout << "first_extra_search counts element (2, 2) = "
            << my_results.extra_searches[1].result_counts[2][2] << std::endl;

  return 0;
}