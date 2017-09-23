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
