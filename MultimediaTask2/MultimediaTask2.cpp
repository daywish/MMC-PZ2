#include <iostream>
#include <utility>
#include <map>
#include <vector>
#include <bitset>
#include <sstream>
#include <Windows.h>

template <class T>
class Node
{
public:
    Node(T symbol) : symbol(symbol), last_code_symbol(-1) {};

    Node(Node<T>* first_child, Node<T>* second_child) 
        : last_code_symbol(-1), first_child(first_child),
        second_child(second_child) {};

    ~Node() {
        delete first_child;
        delete second_child;
    }

    std::string getCodeStringRepresentation() {
        std::stringstream result;
        for (short i = 0; i <= last_code_symbol; ++i) {
            result << (int)code[i];
        }

        return result.str();
    }

    T symbol;
    std::bitset<256> code;
    short last_code_symbol;
    Node* first_child;
    Node* second_child;
};

typedef Node<char> CharNode;
typedef Node<wchar_t> WCharNode;

std::vector<std::pair<char, float>> getAlphabetSymbolFrequences(
    std::string input
) {
    std::map<char, size_t> symbolCounts;

    for (const auto& character : input) {
        ++symbolCounts[character];
    }

    std::vector<std::pair<char, float>> alphabetSymbolFrequences;
    size_t input_length = input.length();
    for (const auto& symbolFrequencyPair : symbolCounts) {
        alphabetSymbolFrequences.push_back(
            std::make_pair(
                symbolFrequencyPair.first,
                (double)symbolFrequencyPair.second / input_length
            )
        );
    }

    return alphabetSymbolFrequences;
}

std::multimap<float, Node<char>*> getInitialTree(
    std::vector<std::pair<char, float>> alphabet_data
) {
    std::multimap<float, Node<char>*> initialTree;

    for (auto& current_symbol_data : alphabet_data) {
        Node<char>* currentNode = new Node<char>(
            current_symbol_data.first
        );
        initialTree.insert(std::make_pair(
            current_symbol_data.second,
            currentNode
        ));
    }

    return initialTree;
}

std::vector<Node<char>*> getSymbolNodes(
    std::multimap<float, Node<char>*>& initislTree
) {
    std::vector<Node<char>*> result;

    for (const auto& current_pair : initislTree) {
        result.push_back(current_pair.second);
    }

    return result;
}

Node<char>* buildTree(
    std::multimap<float, Node<char>*>& tree
) {
    while (tree.size() > 1) {
        Node<char>* last_node = tree.begin()->second;
        float last_node_frequency = tree.begin()->first;
        tree.erase(tree.begin());
        Node<char>* penult_node = tree.begin()->second;
        float penult_node_frequency = tree.begin()->first;
        tree.erase(tree.begin());

        Node<char>* newNode = new Node<char>(penult_node, last_node);
        tree.insert(std::make_pair(
            last_node_frequency + penult_node_frequency,
            newNode
        ));
    }

    return tree.begin()->second;
}

void initializeTreeLeafesCodes(
    Node<char>* tree_node,
    Node<char>* parent,
    bool value
) {
    for (short i = 0; i <= parent->last_code_symbol; ++i) {
        tree_node->code[i] = parent->code[i];
    }
    tree_node->code[parent->last_code_symbol + 1] = value;
    tree_node->last_code_symbol = parent->last_code_symbol + 1;

    if (tree_node->first_child) {
        initializeTreeLeafesCodes(tree_node->first_child, tree_node, false);
    }
    if (tree_node->second_child) {
        initializeTreeLeafesCodes(tree_node->second_child, tree_node, true);
    }
}

void initializeTreeNodesCodes(Node<char>* root) {

    initializeTreeLeafesCodes(
        root->first_child, root, false
    );
    initializeTreeLeafesCodes(
        root->second_child, root, true
    );
}

std::string encodeString(
    std::string input, 
    std::vector<Node<char>*> symbolNodes
) {
    std::string encodedString = "";

    for (const auto& symbol : input) {
        for (const auto& node : symbolNodes) {
            if (symbol == node->symbol) {
                encodedString += node->getCodeStringRepresentation();
                break;
            }
        }
    }

    return encodedString;
}

std::string decodeString(
    std::string encoded_string,
    std::vector<Node<char>*> symbolNodes
) {
    std::string decoded_string = "";

    std::string currentCode = "";
    for (const auto& symbol : encoded_string) {
        currentCode += symbol;

        for (const auto& node : symbolNodes) {
            if (currentCode == node->getCodeStringRepresentation()) {
                decoded_string += node->symbol;
                currentCode = "";
                break;
            }
        }
    }

    return decoded_string;
}

std::vector<std::vector<std::string>> completion_codes = {
    {"00110101", "00110101"},
    {"00111", "010"},
    {"0111", "11"},
    {"1000", "10"},
    {"1011", "011"},
    {"1100", "0011"},
    {"1110", "0010"},
    {"1111", "00011"},
    {"10011", "000101"},
    {"10100", "000100"},
    {"00111", "0000100"},
    {"01000", "0000101"},
    {"001000", "0000111"},
    {"000011", "00000100"},
    {"110100", "00000111"},
    {"110101", "000011000"},
    {"", ""},
    {"", ""},
    {"", ""},
    {"", ""},
    {"", ""},
    {"", ""},
    {"", ""},
    {"", ""},
    {"", ""},
    {"", ""},
    {"", ""},
    {"", ""},
    {"", ""},
    {"", ""},
    {"", ""},
    {"", ""},
    {"", ""},
    {"", ""},
    {"", ""},
    {"", ""},
    {"", ""},
    {"", ""},
    {"", ""},
    {"", ""},
    {"", ""},
    {"", ""},
    {"", ""},
    {"", ""},
    {"00101101", "000001010100"},
};
std::map<unsigned short, std::vector<std::string>> additional_codes = {
    {512, {"01100101", "0000001101100"}}
};
size_t completion_codes_max = 63;
size_t additional_codes_max = 2560;

std::vector<size_t> getRowSeries(const char* row, size_t columns) {
    std::vector<size_t> result;
    size_t current_series = 0;
    char current_color = '0';

    for (size_t i = 0; i < columns; ++i) {
        if (row[i] != current_color) {
            result.push_back(current_series);
            current_series = 0;
            current_color = current_color == '0' ? '1' : '0';
        }
        ++current_series;
    }
    result.push_back(current_series);

    return result;
}

std::vector<std::string> encode_by_ccitt_group_3(
    const char** input_image, size_t rows, size_t columns
) {
    std::vector<std::string> encoded_data;
    std::vector<size_t> row_series;
    const char* current_row;
    std::string row_code;
    size_t series_length;
    size_t code_overflow = completion_codes_max + additional_codes_max;
    size_t max_compound_code;

    for (size_t i = 0; i < rows; ++i) {
        current_row = input_image[i];
        row_series = getRowSeries(current_row, columns);
        row_code = "";

        for (size_t i = 0; i < row_series.size(); ++i) {
            series_length = row_series[i];
            while (series_length > code_overflow) {
                series_length -= additional_codes_max;
                row_code += additional_codes[additional_codes_max][i % 2];
            }

            if (series_length > completion_codes_max) {
                max_compound_code = (series_length >> 6) * 64;
                series_length -= max_compound_code;
                row_code += additional_codes[max_compound_code][i % 2];
            }
            row_code += completion_codes[series_length][i % 2];
        }

        encoded_data.push_back(row_code);
    }

    return encoded_data;
}

void print(std::vector<Node<char>*> symbolNodes) {
    for (const auto& k : symbolNodes) {
        std::cout << k->symbol << ' ';
        for (int i = 0; i <= k->last_code_symbol; ++i) {
            std::cout << k->code[i];
        }
        std::cout << std::endl;
    }
}

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "Ukrainian");

    std::vector<std::pair<char, float>> alphabet_data = {
        std::make_pair('a', 0.5),
        std::make_pair('b', 0.24),
        std::make_pair('c', 0.15),
        std::make_pair('d', 0.11)
    };

    std::string input_message = "TEXT THAT IS COMPRESSED";
    std::vector<std::pair<char, float>> a = 
        getAlphabetSymbolFrequences(input_message);
    std::multimap<float, Node<char>*> initial_tree = getInitialTree(
        a
    );
    std::vector<Node<char>*> symbolNodes = getSymbolNodes(initial_tree);

    Node<char>* b = buildTree(initial_tree);
    initializeTreeNodesCodes(b);

    std::cout << "Коди символів рядка: " << std::endl;
    print(symbolNodes);

    std::string encodedString = encodeString(input_message, symbolNodes);
    std::cout << "Закодоване значення: " << encodedString << std::endl;
    std::string decodedString = decodeString(encodedString, symbolNodes);
    std::cout << "Декодоване значення: " << decodedString << std::endl << std::endl;

    const char* input_image[] = {
        "111000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001111111111", 
    };

    std::vector<std::string> encoded_image = encode_by_ccitt_group_3(
        input_image, 1, 525
    );
    std::cout << "Compressed image rows:" << std::endl;
    for (const auto& row : encoded_image) {
        std::cout << row << std::endl;
    }
}
