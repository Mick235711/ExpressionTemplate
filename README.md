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

------------------
Output:

    Value: 2
    Value: 4
    Value: 1
    Value: 6
    
    2 is even!
    4 is even!
    1 is odd!
    6 is even!
    
    1
    
## Feature
`namespace arg_names`: provide arg1 to arg5 for argument

`namespace operators`: provide operator overloading for write nature syntax such as `+`, `<<`

`namespace expression`: `val`, `ref` and `cref`

    std::for_each(vec.begin(), vec.end(), cout << val("Value: ") << arg1 << "\n");
                                                  ^^^^^^^^^^^^^^            ^^^^
                                                  need val                  don't need val
`namespace statement`: provide statement support such as `if_`, `while_`
###### They are all in `molly` namespace!

#### Note: this header require `C++14` support such as passing `-std=c++14` to your compiler
