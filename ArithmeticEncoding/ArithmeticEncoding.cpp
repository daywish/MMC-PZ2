#include <iostream>
#include <Windows.h>
#include <utility>
#include <map>
#include <vector>
#include <tuple>

std::string string_to_compress = "КОВ.КОРОВА";
std::map<char, std::pair<double, double>> intervals = {
    {'О', {0, 0.3}}, {'К', {0.3, 0.5}}, {'В', {0.5, 0.7}}, 
    {'Р', {0.7, 0.8}}, {'А', {0.8, 0.9}}, {'.', {0.9, 1}}
};

float randomDouble(double min, double max)
{
    double res = (double)rand() / (double)RAND_MAX;
    return min + res * (max - min);
}

double encodeByArithmeticEncoding(std::string input) {
    double left = 0;
    double right = 1;
    double previous_left = 0;
    double previous_range = 0;

    for (const auto& symbol : input) {
        previous_left = left;
        previous_range = right - left;
        left = previous_left + intervals[symbol].first * previous_range;
        right = previous_left + intervals[symbol].second * previous_range;
    }

    return randomDouble(left, right);
}

std::string decodeByArithmeticEncoding(
    double encoded_value, size_t input_length
) {
    std::string decoded_value = "";
    double left = 0;
    double right = 1;
    double symbol_left = 0;
    double symbol_right = 0;
    double previous_left = 0;
    double previous_range = 0;

    for (size_t i = 0; i < input_length; ++i) {
        for (const auto& interval : intervals) {
            previous_left = left;
            previous_range = right - left;
            symbol_left = previous_left + interval.second.first * previous_range;
            symbol_right = previous_left + interval.second.second * previous_range;

            if (symbol_left <= encoded_value && symbol_right > encoded_value) {
                decoded_value += interval.first;
                left = symbol_left;
                right = symbol_right;
                break;
            }
        }
    }

    return decoded_value;
}

void bitsPlusFollow(
    std::string& encoded_string, int bit, 
    int bits_to_follow
) {
    encoded_string += bit;
    for (; bits_to_follow > 0; bits_to_follow--) {
        encoded_string += bit;
    }
}

size_t getSymbolIndex(
    std::vector<std::pair<char, unsigned char>> table,
    char symbol
) {
    for (int i = 0; i < table.size(); ++i) {
        if (table[i].first == symbol) {
            return i;
        }
    }

    return -1;
}

void advancedEncodeByArithmeticEncoding(
    std::string input, 
    std::vector<std::pair<char, unsigned char>> table
) {
    std::string result = "";
    int left = 0;
    int right = 65535;
    int delitel = table[table.size() - 1].second;
    int first_qtr = (right + 1) / 4;
    int half = first_qtr * 2;
    int third_qtr = first_qtr * 3;
    int bits_to_follow = 0; 
    int previous_left = 0;
    int previous_range = 0;

    for (const auto& symbol : input) {
        size_t j = getSymbolIndex(table, symbol);
        previous_left = left;
        previous_range = right - previous_left + 1;

        left = previous_left + 
            table[j - 1].second * previous_range / delitel;
        right = previous_left + 
            table[j - 1].second * previous_range / delitel - 1;
        for (;;) {
            if (right < half) {
                bitsPlusFollow(result, bits_to_follow, 0);
            }
            else if (left >= half) {
                bitsPlusFollow(result, bits_to_follow, 1);
                left -= half;
                right -= half;
            } else if ((left >= first_qtr) && (right < third_qtr)) {
                ++bits_to_follow;
                left -= first_qtr;
                right -= first_qtr;
            } else {
                break;
            }
            left += left;
            right += right + 1;
        }
    }
}

std::string advancedDecodeByArithmeticEncoding(
    std::string encodedData,
    std::vector<std::pair<char, unsigned char>> table
) {
    std::string result = "";
    int left = 0;
    int right = 65535;
    int delitel = table[table.size() - 1].second;
    int first_qtr = (right + 1) / 4;
    int half = first_qtr * 2;
    int third_qtr = first_qtr * 3;
    int previous_left = 0;
    int previous_range = 0;
    int freq = 0;

    for (const auto& symbol : encodedData) {
        char value = symbol;
        freq = ((symbol - left + 1) * delitel - 1) / (right - left + 1);
        size_t j = getSymbolIndex(table, symbol);
        previous_left = left;
        previous_range = right - previous_left + 1;

        left = previous_left +
            table[j - 1].second * previous_range / delitel;
        right = previous_left +
            table[j - 1].second * previous_range / delitel - 1;
        for (;;) {
            if (right < half)
                ;
            else if (left >= half) {
                left -= half;
                right -= half;
                value -= half;
            }
            else if ((left >= first_qtr) && (right < third_qtr)) {
                value -= first_qtr;
                left -= first_qtr;
                right -= first_qtr;
            }
            else {
                break;
            }
            left += left;
            right += right + 1;
            value += value;
        }
        result += value;
    }

    return result;
}

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "Ukrainian");
    double encoded_data = encodeByArithmeticEncoding(string_to_compress);
    std::cout << "Вхідне значення: " << string_to_compress << std::endl;
    std::cout << "Закодоване значення: " << encoded_data << std::endl;
    std::cout << "Декодоване значення: " << decodeByArithmeticEncoding(
        encoded_data, 
        string_to_compress.length()
    );
}

