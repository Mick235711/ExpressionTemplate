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
    using namespace molly::expression;
    using namespace molly::statement;
    
    vector<int> vec{2, 4, 1, 6};
    std::for_each(vec.begin(), vec.end(), (cout << val("Value: "), cout << arg1 << '\n'));
    cout << std::endl;
    
    vector<std::string> vec2;
    int count = 0;
    std::transform(vec.begin(), vec.end(), std::back_inserter(vec2), (
                   if_(arg1 % 2 == 0)
                   [
                       cout << arg1 << " is even!\n"
                   ].else_
                   [
                       cout << arg1 << " is odd!\n",
                       ref(count)++
                   ],
                   if_else(arg1 % 3 == 0, "Dividable by 3", "Not dividable by 3")));
    cout << std::endl << count << std::endl;
    return 0;
}
