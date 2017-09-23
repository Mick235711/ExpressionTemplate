# ExpressionTemplate

### a simple library uses expression templates

ExpressionTemplate is a simple library which provide usages like [Boost::Phoenix](http://www.boost.org/doc/libs/1_65_0/doc/html/phoenix.html)

## Usage
    #include <iostream>
    #include <vector>
    #include "Expression.hpp"
    
    using namespace std;
    
    int main()
    {
        using namespace molly;
        using namespace tags;
        using namespace expressions;
        using namespace arg_names;
        using namespace operators;
        vector<int> vec1{2, 4, 1, -5, 2, 0, -1};
        for_each(vec1.begin(), vec1.end(), cout << arg1 << ' ');
        cout << endl;
        for_each(vec1.begin(), vec1.end(), (
            cout << constant("Before: ") << arg1 << '\n',
            arg1 = arg1 + 5,
            cout << constant("After: ") << arg1 << '\n'
        ));
        for_each(vec1.begin(), vec1.end(), cout << arg1 << ' ');
        cout << endl;
        return 0;
    }

------------------
Output:

    2 4 1 -5 2 0 -1 
    Before: 2
    After: 7
    Before: 4
    After: 9
    Before: 1
    After: 6
    Before: -5
    After: 0
    Before: 2
    After: 7
    Before: 0
    After: 5
    Before: -1
    After: 4
    7 9 6 0 7 5 4  
    
## Feature
`namespace arg_names`: provide arg1 to arg5 for argument

`namespace operators`: provide operator overloading for write nature syntax such as `+`, `<<`

`namespace expression`: `constant`, `ref_constant` and `cref_constant`

    std::for_each(vec.begin(), vec.end(), cout << constant("Value: ") << arg1 << "\n");
                                                  ^^^^^^^^^^^^^^                 ^^^^
                                                  need constant                  don't need constant
###### They are all in `molly` namespace!

#### Note: this header require `C++14` support such as passing `-std=c++14` to your compiler

## TODO
Implement statement support
