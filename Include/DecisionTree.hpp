#ifndef DECISION_TREE_H
#define DECISION_TREE_H
#include<vector>
#include<string>
#include<map>
#include<algorithm>
#include<cmath>
#include<set>
#include "data.hpp"
class TreeNode
{
    File &data;
    std::vector<int> record_index;
    struct FeatureGain
    {
        std::string field;
        double gain;
    };

public:
    std::string attr;
    double gain;
    std::set<std::string> feature_sets;
    int label;
    TreeNode * lchild;
    TreeNode * rchild;
    TreeNode(File & file);
    TreeNode(File &file, std::vector<int> index, std::set<std::string> features);
    ~TreeNode()
    {
        if(rchild != nullptr) delete rchild;
        if(lchild != nullptr) delete lchild;
    }
    void divide_node();
    void show_tree();
    void set_label();
    double entropy();
    int test(Record &record);
    double test(File &test_file);
    double _entropy(std::vector<int> &index);
    double gain_of_attr(std::string attr, std::vector<int> &l_index, std::vector<int> &r_index);
    struct FeatureGain choose_attr(std::vector<int>&, std::vector<int> &);
};

#endif // DECISION_TREE_H
