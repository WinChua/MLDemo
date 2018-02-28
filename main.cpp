#include <iostream>
#include "data.hpp"
#include "DecisionTree.hpp"
using namespace std;

int main(int argc, char* argv[])
{
    if(argc < 3)
    {
        std::cout << "you should specify the train and test file like:" << std::endl;
        std::cout << "MLDemo train.file test.file" << std::endl;
        return 1;
    }
    std::string trainfile(argv[1]), testfile(argv[2]);
    File train(trainfile);
    File test(testfile);
    //train.show_features();
    TreeNode root(train);
    train.show_statistic();
    test.show_statistic();
    root.test(test);
    return 0;
}
