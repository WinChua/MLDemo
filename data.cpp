#include "data.hpp"

void File::show_features() const
{
    for(auto it=featureset.begin(); it != featureset.end(); ++it)
    {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    return;
}

void File::show_statistic() const
{
    std::cout << "the filename is " << filename << std::endl;
    std::cout << "the number of positive case is " << positive_case << std::endl;
    std::cout << "the number of negative case is " << negative_case << std::endl;
}

File::File(std::string f): filename(f), positive_case(0), negative_case(0)
{
    std::ifstream fin(filename.c_str());
    std::string line;
    while(std::getline(fin, line))
    {
        records.emplace(records.end(), line, featureset);
        if((records.end()-1)->label == 1)
            positive_case += 1;
        else
            negative_case += 1;
    }
}
