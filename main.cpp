#include <iostream>
#include "data.hpp"
#include "DecisionTree.hpp"
using namespace std;

int main()
{
    File train("data/libsvm.txt");
    File test("data/libsvm_test.txt");
    //train.show_features();
    TreeNode root(train);
    train.show_statistic();
    test.show_statistic();
    root.test(test);
    system("pause");
    return 0;
}
