#include "Expression.hpp"
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>

using std::cout;
using std::vector;

int main()
{
    using namespace molly;
    using namespace molly::operators;
    using namespace molly::arg_names;
    
    vector<int> vec{2, 4, 1, 6};
    std::for_each(vec.begin(), vec.end(), (cout << val("Value: "), cout << arg1 << ' '));
    cout << std::endl;
    
    vector<std::string> vec2;
    std::transform(vec.begin(), vec.end(), std::back_inserter(vec2),
                   if_else(arg1 % 2 == 0, "Even", "Odd"));
    std::for_each(vec2.begin(), vec2.end(), cout << arg1 << ' ');
    cout << std::endl;
    return 0;
}
