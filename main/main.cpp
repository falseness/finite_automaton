#include <source/practice1/parser.h>
#include <string>
#include <iostream>

int main() {
    std::string s;
    size_t k, l;
    std::cin >> s >> k >> l;
    int8_t res = ReversePolishNotationParser::Parse(s, k, l);
    if (res == -1) {
        std::cout << "ERROR";
    }
    else if (res == 1) {
        std::cout << "YES\n";
    }
    else
        std::cout << "NO\n";
    return 0;
}
