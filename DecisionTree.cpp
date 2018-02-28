#include "DecisionTree.hpp"
TreeNode::TreeNode(File & file):data(file), feature_sets(file.featureset), lchild(nullptr), rchild(nullptr)
{
    for(int i=0; i < data.records.size(); ++i)
    {
        record_index.push_back(i);
    }
    set_label();
    divide_node();
}

TreeNode::TreeNode(File &file, std::vector<int> index, std::set<std::string> features):data(file), record_index(index),feature_sets(features), lchild(nullptr), rchild(nullptr)
{
    set_label();
    divide_node();
}


void TreeNode::divide_node() {
    std::vector<int> index_of_lchild, index_of_rchild;
    std::set<std::string> feature_of_lchild(feature_sets);
    std::set<std::string> feature_of_rchild(feature_sets);
    struct FeatureGain feature_gain = choose_attr(index_of_lchild, index_of_rchild);
    attr = feature_gain.field;
    gain = feature_gain.gain;
    if(attr != "no divided")
    {
        feature_of_lchild.erase(attr);
        feature_of_rchild.erase(attr);
        if(index_of_lchild.size() > 0)
            lchild = new TreeNode(data, index_of_lchild, feature_of_lchild);
        if(index_of_rchild.size() > 0)
            rchild = new TreeNode(data, index_of_rchild, feature_of_rchild);
    }
}


void TreeNode::set_label()
{
    int  pos=0, neg=0;
    for(auto index : record_index)
    {
        if(data.records[index].label == 1) pos += 1;
        else neg += 1;
    }
    label = pos > neg ? 1 : -1;
}

int TreeNode::test(Record &record)
{
    TreeNode * node = this;
    std::string feature = node->attr;
    while(feature != "no divided")
    {
        if(record.data.find(feature) == record.data.end())
        {
            node = node->rchild;
        }
        else node = node->lchild;
        feature = node->attr;
    }
    return node->label;
}

double TreeNode::test(File &test_file)
{
    std::cout << "the size of test set is " << test_file.records.size() << std::endl;
    float tp=0, tn=0, fp=0, fn=0;
    for(auto record : test_file.records) {
        if(record.label == test(record))
            if(record.label == 1)
                tp += 1;
            else
                tn += 1;
        else
            if(record.label == 1)
                fp += 1;
            else
                fn += 1;
    }
    std::cout << "the true_positive is " << tp / (tp+fp) << std::endl;
    std::cout << "the false_positive is " << tn / (tn + fn) << std::endl;
    std::cout << "the recall is " << tp / (tp + fn) << std::endl;
    return tp / (tp+tn);
}

void TreeNode::show_tree()
{
    std::vector<TreeNode *> current_level = {this};
    while(!current_level.empty())
    {
        std::cout << std::endl;
        std::vector<TreeNode *> next_level;
        for(auto it : current_level)
        {
            std::cout << it->attr << ":" << it->gain <<":" << it->label << " ";
//            if(it->lchild == nullptr && it->rchild == nullptr) {
//                std::cout << "the size of the leaf is " << it->record_index.size() << std::endl;
//            }
            if(it->lchild != nullptr)
            {
                next_level.push_back(it->lchild);
            }
            if(it->rchild != nullptr)
            {
                next_level.push_back(it->rchild);
            }
        }
        swap(current_level, next_level);
        next_level.clear();
    }
    return;
}

struct TreeNode::FeatureGain TreeNode::choose_attr(std::vector<int> &max_l_index, std::vector<int> &max_r_index)
{
    std::string max_attr("no");
    double max_entropy=0.0;
    std::vector<int> l_index, r_index;
    for(auto feature : data.featureset)
    {
        double gain = gain_of_attr(feature, l_index, r_index);
        if(gain > max_entropy)
        {
            max_attr = feature;
            swap(max_l_index, l_index);
            swap(max_r_index, r_index);
            max_entropy = gain;
        }
        r_index.clear();
        l_index.clear();
    }
    if(max_entropy <  0.005)
    {
        return {"no divided", 0.0};
    }
    return {max_attr, max_entropy};
}


double get_entropy(double pos, double neg)
{
    if(pos == 0 || neg == 0) return 0.0;
    pos = pos/(pos+neg);
    neg = 1 - pos;
    return -pos*std::log2(pos)-neg*std::log2(neg);
}


double TreeNode::gain_of_attr(std::string attr, std::vector<int> &l_index, std::vector<int> &r_index)
{
    double en = entropy();
    float index_size = record_index.size();
    for(auto ri : record_index)
    {
        if(data.records[ri].data.find(attr) == data.records[ri].data.end())
        {
            r_index.push_back(ri);
        }
        else l_index.push_back(ri);
    }
    double r_entropy = _entropy(r_index);
    double l_entropy = _entropy(l_index);
    double div = r_index.size()/index_size* r_entropy + l_index.size() / index_size* l_entropy;
    double gain = en - div;
    return gain;
}

double TreeNode::_entropy(std::vector<int> &index)
{
    double entropy=0.0;
    double pos=0.0, neg=0.0;
    for(auto it=index.begin(); it != index.end(); ++it)
    {
        if(data.records[*it].label == 1)
        {
            pos += 1;
        }
        else
        {
            neg += 1;
        }
    }
    entropy = get_entropy(pos, neg);
    return entropy;
}

double TreeNode::entropy()
{
    return _entropy(record_index);
}
