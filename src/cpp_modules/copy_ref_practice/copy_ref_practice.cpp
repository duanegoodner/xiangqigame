#include <array>
#include <cstdlib>
#include <iostream>
#include <string>

using namespace std;

const int array_size = 3;

typedef array< array <int, array_size>, array_size> MyArray_t;

const MyArray_t kStartingArray = {{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}};

struct TestItem {
    string name;
    MyArray_t &item_data;
};


class DataAnalyzer {
    public:
    DataAnalyzer(MyArray_t &data) : data_{data} {};
    MyArray_t CreateCopyOfData() {
        MyArray_t data_copy;
        for (auto row = 0; row < array_size; row++) {
            copy(data_[row].begin(), data_[row].end(), data_copy[row].begin());
        }
        return data_copy;
    }

    int RunCalcThatModifiesArray() {
        auto scratchwork_array = CreateCopyOfData();
        scratchwork_array[1][1] = rand();
        return scratchwork_array[1][1];

    }
    private:
    MyArray_t data_;
};


int main() {

    auto my_array = kStartingArray;
    auto my_item = TestItem{"something", my_array};
    auto my_analyzer = DataAnalyzer(my_item.item_data);
    auto result = my_analyzer.RunCalcThatModifiesArray();

    cout << result << endl;
    cout << my_array[1][1] << endl;

    return 0;
}



