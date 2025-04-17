#include "my_utils.h"

namespace my_utils
{
    string generate_uuid()
    {
        boost::uuids::random_generator gen;
        return boost::uuids::to_string(gen());
    }

    bool is_all_digit(const string &str)
    {
        // 遍历字符串的每个字符
        for (char ch : str)
        {
            // 如果字符不是数字，返回 false
            if (!std::isdigit(ch))
            {
                return false;
            }
        }
        // 如果遍历完没有遇到非数字字符，返回 true
        return true;
    }
}
