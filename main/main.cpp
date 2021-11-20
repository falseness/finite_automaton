#include <source/user_interface.h>
#include <source/practice1/parser.h>
#include <iostream>

int main() {

    int res = ReversePolishNotationParser::Parse("acb..bab.c. * .ab.ba. + . + *a.", 3, 0);
    std::cout << res;
    return 0;
    UserInterface u;
    u.Run();
}
