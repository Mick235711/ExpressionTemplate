# ExpressionTemplate

### a simple library uses expression templates

ExpressionTemplate is a simple library which provide usages like [Boost::Phoenix](http://www.boost.org/doc/libs/1_65_0/doc/html/phoenix.html)

## Usage
    #include <iostream>
    #include <vector>
    #include <algorithm>
    #include "Expression.hpp"
    
    using namespace std;
    
    int main()
    {
        using namespace molly;
        using namespace tags;
        using namespace expressions;
        using namespace arg_names;
        using namespace operators;
        using namespace statements;
        vector<int> vec1{2, 4, 1, -5, 2, -6, 0, -1};
        for_each(vec1.begin(), vec1.end(), cout << arg1 << ' ');
        cout << endl;
        for_each(vec1.begin(), vec1.end(), (
            cout << constant("Before: ") << arg1 << '\n',
            if_(arg1 % 2 == 0)
            [
                arg1 = arg1 + 5
            ].
            else_
            [
                arg1 = arg1 - 5
            ],
            cout << constant("After: ") << arg1 << '\n'
        ));
        for_each(vec1.begin(), vec1.end(), cout << arg1 << ' ');
        cout << endl;
        vector<int> vec2{4, 2, 26, 27, 1, 7, 3, 8, 10, 15, 20, 30, 40, 35, 29, 46};
        for_each(vec2.begin(), vec2.end(), (
            cout << constant("Sequence for ") << arg1 << ':',
            while_(arg1 != 1)
            [
                cout << constant(' ') << arg1,
                if_(arg1 % 2 == 0)
                [
                    arg1 /= 2
                ].
                else_
                [
                    arg1 = arg1 * 3 + 1
                ]
            ],
            cout << constant(' ') << arg1 << '\n'
        ));
        return 0;
    }


------------------
Output:

    2 4 1 -5 2 -6 0 -1 
    Before: 2
    After: 7
    Before: 4
    After: 9
    Before: 1
    After: -4
    Before: -5
    After: -10
    Before: 2
    After: 7
    Before: -6
    After: -1
    Before: 0
    After: 5
    Before: -1
    After: -6
    7 9 -4 -10 7 -1 5 -6 
    Sequence for 4: 4 2 1
    Sequence for 2: 2 1
    Sequence for 26: 26 13 40 20 10 5 16 8 4 2 1
    Sequence for 27: 27 82 41 124 62 31 94 47 142 71 214 107 322 161 484 242 121 364 182 91 274 137 412 206 103 310 155 466 233 700 350 175 526 263 790 395 1186 593 1780 890 445 1336 668 334 167 502 251 754 377 1132 566 283 850 425 1276 638 319 958 479 1438 719 2158 1079 3238 1619 4858 2429 7288 3644 1822 911 2734 1367 4102 2051 6154 3077 9232 4616 2308 1154 577 1732 866 433 1300 650 325 976 488 244 122 61 184 92 46 23 70 35 106 53 160 80 40 20 10 5 16 8 4 2 1
    Sequence for 1: 1
    Sequence for 7: 7 22 11 34 17 52 26 13 40 20 10 5 16 8 4 2 1
    Sequence for 3: 3 10 5 16 8 4 2 1
    Sequence for 8: 8 4 2 1
    Sequence for 10: 10 5 16 8 4 2 1
    Sequence for 15: 15 46 23 70 35 106 53 160 80 40 20 10 5 16 8 4 2 1
    Sequence for 20: 20 10 5 16 8 4 2 1
    Sequence for 30: 30 15 46 23 70 35 106 53 160 80 40 20 10 5 16 8 4 2 1
    Sequence for 40: 40 20 10 5 16 8 4 2 1
    Sequence for 35: 35 106 53 160 80 40 20 10 5 16 8 4 2 1
    Sequence for 29: 29 88 44 22 11 34 17 52 26 13 40 20 10 5 16 8 4 2 1
    Sequence for 46: 46 23 70 35 106 53 160 80 40 20 10 5 16 8 4 2 1
    
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
