#include <iostream>
#include "Expression.hpp"

using std::cout;
using std::endl;
using std::ostream;

int main()
{
    using namespace molly;
    using namespace tags;
    using namespace expressions;
    using namespace arg_names;
    auto e = expr<shift_left_tag, literal<ostream&>, expr<binary_plus_tag, argument<1>, literal<int>>>
        (ref_constant(cout), expr<binary_plus_tag, argument<1>, literal<int>>(_1, constant(2)));
    e(3);
    return 0;
}
