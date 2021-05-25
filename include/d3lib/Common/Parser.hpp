#ifndef __PARSER_H__
#define __PARSER_H__

#include <vector>
#include <string>
#include <regex>

namespace d3lib
{
    class JsonParser
    {
        std::string str;
        std::string strstack;

    public:
        JsonParser(std::string input_json) : str(input_json) {}

        JsonParser operator[](std::string sstr)
        {
            size_t pos = str.find(sstr);
            pos = str.find(":", pos) + 1;
            size_t last_pos = pos;

            while (true)
            {
                if (str[pos] == '}' && strstack.empty()) break;
                if (str[pos] == ',' && strstack.empty()) break;
                if (str[pos] == '{') strstack.push_back('{');
                if (str[pos] == '}') strstack.pop_back();
                if (str[pos] == '[') strstack.push_back('[');
                if (str[pos] == ']') strstack.pop_back();
                pos++;
            }

            return JsonParser(str.substr(last_pos, (pos - last_pos)));
        }

        JsonParser operator[](size_t index)
        {
            std::vector<std::string> v;
            size_t pos = str.find_first_of("[");
            size_t lpos = ++pos;

            while (true)
            {
                if (str[pos] == L',' && strstack.empty()) { v.push_back(str.substr(lpos, pos - lpos));  lpos = pos + 1; }
                if (str[pos] == '{') strstack.push_back('{');
                if (str[pos] == '}') strstack.pop_back();
                if (str[pos] == '[') strstack.push_back('[');
                if (str[pos] == ']' && strstack.empty()) { v.push_back(str.substr(lpos, pos - lpos)); break; }
                if (str[pos] == ']') strstack.pop_back();
                pos++;
            }

            return JsonParser(v[index]);
        }


        std::string ToStr() { return str; }
        int ToInt() { return std::stoi(str); }
        double ToDouble() { return std::stod(str); }
        float ToFloat() { return std::stof(str); }

    };

}

#endif