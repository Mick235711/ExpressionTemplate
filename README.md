# ExpressionTemplate

### a simple library uses expression templates

ExpressionTemplate is a simple library which provide usages like [Boost::Lambda](http://www.boost.org/doc/libs/1_64_0/doc/html/lambda.html)

## Usage
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

------------------
Output:

    2 4 1 6
    Even Even Odd Even
    
## Feature
`namespace arg_names`: provide arg1 to arg5 for argument

`namespace operators`: provide operator overloading for write nature syntax

`free functions`: `val`, `ref` and `cref`

    std::for_each(vec.begin(), vec.end(), cout << val("Value: ") << arg1 << "\n");
                                                  ^^^^^^^^^^^^^^            ^^^^
                                                  need val                  don't need val
###### They are all in `molly` namespace!
