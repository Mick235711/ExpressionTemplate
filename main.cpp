#include <iostream>
#include <vector>
#include "Expression.hpp"

int main()
{
    using namespace molly;
    using namespace arg_names;
    using namespace operators;
    int i = 4, j = 5, k = 6;
    std::vector<int*> vec{&k, &i, &j};
    std::vector<int> vec2;
    std::transform(vec.begin(), vec.end(), std::back_inserter(vec2), *arg1);
    std::for_each(vec2.begin(), vec2.end(), std::cout << arg1 << ' ');
    int count = 0;
    std::for_each(vec2.begin(), vec2.end(), ++ref(count));
    std::cout << count << std::endl;
    return 0;
}
