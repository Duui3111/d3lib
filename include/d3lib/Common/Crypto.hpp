#ifndef __CRYPTO_H__
#define __CRYPTO_H__

#include <string>
#include <iomanip>
#include <algorithm>
#include <vector>
#include <bitset>
#include <sstream>

namespace d3lib
{
    namespace Cryto
    {
       struct Xor
       {
            std::string XorCrpet(std::string Crpet, char *map)
            {
                std::string str = Crpet;
                for (size_t i = 0; i < str.size(); i++)
                    str[i] ^= map[i % (sizeof(str) + i) / sizeof(int)];
                return str;
            }

            std::string XorCrpetChar(std::string Crpet, char map)
            {
                std::string str = Crpet;
                for (size_t i = 0; i < str.size(); i++)
                    str[i] ^= map;
                return str;
            }
       };

        struct Binary
        {
            static std::string encode(const std::string &binary)
            {
                std::string binaryString = "";
                for (char _char : binary)
                    binaryString += std::bitset<8>(_char).to_string();
                return binaryString;
            }

            static std::string decode(const std::string &binary)
            {
                std::stringstream sstream(binary);
                std::string output;
                std::bitset<8> bits;
                while (sstream >> bits)
                    output += char(bits.to_ulong());
                return output;
            }
        };

        class Base64
        {
        public:
            static std::string encode(const std::string &in)
            {
                std::string out;

                int val = 0, valb = -6;
                for (unsigned char c : in)
                {
                    val = (val << 8) + c;
                    valb += 8;
                    while (valb >= 0)
                    {
                        out.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(val >> valb) & 0x3F]);
                        valb -= 6;
                    }
                }
                if (valb > -6)
                    out.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[((val << 8) >> (valb + 8)) & 0x3F]);
                while (out.size() % 4)
                    out.push_back('=');
                return out;
            }

            static std::string decode(const std::string &in)
            {
                std::string out;

                std::vector<int> T(256, -1);
                for (int i = 0; i < 64; i++)
                    T["ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[i]] = i;

                int val = 0, valb = -8;
                for (unsigned char c : in)
                {
                    if (T[c] == -1)
                        break;
                    val = (val << 6) + T[c];
                    valb += 6;
                    if (valb >= 0)
                    {
                        out.push_back(char((val >> valb) & 0xFF));
                        valb -= 8;
                    }
                }

                return out;
            }
        };

        class URIComponent
        {
        public:
            static std::string urlDecode(std::string str)
            {
                std::string result = "";

                for (unsigned int i = 0; i < str.length(); i++)
                {
                    if (str[i] == '%')
                    {
                        char ret = static_cast<char>(strtoul(str.substr(i + 1, 2).c_str(), NULL, 16));
                        result.append(&ret, 1);
                        i += 2;
                    }
                    else result.append(&str[i], 1);
                }

                return result;
            }

            static std::string urlEncode(std::string str)
            {
                const std::string url_letters("-.0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz~");
                std::stringstream ss;

                for (std::string::const_iterator p = str.begin(); p != str.end(); ++p)
                {
                    if (std::binary_search(url_letters.begin(), url_letters.end(), *p)) ss << *p;
                    else ss << "%" << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(*p);
                }

                return ss.str();
            }
        };
    }
}

#endif