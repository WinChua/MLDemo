#ifndef DATA_H
#define DATA_H
#include<map>
#include<iostream>
#include<sstream>
#include<fstream>
#include<utility>
#include<string>
#include<set>
#include<vector>
#include<cmath>

// libsvm format: -1 3:1 11:1 14:1 19:1 39:1 42:1 55:1 64:1 67:1 73:1 75:1 76:1 80:1 83:1
class Record;
class File
{
public:
    typedef std::set<std::string> FeatureSet;
    std::string filename;
    std::vector<Record> records;
    FeatureSet featureset;
    int positive_case;
    int negative_case;

    File(std::string f);
    void show_statistic() const;
    void show_features() const;

};

class Record
{
typedef std::map<std::string, float> Fields;
public:
    int label;
    Fields data;
    Record(std::string field_values, File::FeatureSet & featuresets)
    {
        std::stringstream ss(field_values);
        ss >> label;
        std::string field_value;
        while(ss)
        {
            ss >> field_value;
            std::string field = field_value.substr(0, field_value.find(":"));
            featuresets.insert(field);
            float value = atof(field_value.substr(field_value.find(":")+1,
                                                  field_value.size()).c_str());
            data.emplace(field, value);
        }
    }
};


#endif // DATA_H
