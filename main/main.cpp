#include <iostream>
#include <source/regular_expression/transformer.h>

int main() {
    auto transformer = RegularTransformer("(a*b)*");
    auto automat = transformer.Parse();
    automat->Output();
    return 0;
}
