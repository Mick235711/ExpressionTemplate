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
