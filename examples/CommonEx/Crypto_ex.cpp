#include "../include/d3lib/Common/Crypto.hpp"
#include <iostream>

int main()
{
    std::cout << d3lib::Cryto::Binary::encode("hi") << std::endl; //01101000 01101001 
    std::cout << d3lib::Cryto::Binary::decode("01101000 01101001") << std::endl; // hi

    std::cout << d3lib::Cryto::Base64::encode("hi") << std::endl; //aGk= 
    std::cout << d3lib::Cryto::Base64::decode("aGk=") << std::endl; // hi

    std::cout << d3lib::Cryto::URIComponent::urlEncode("https://www.google.com/") << std::endl; //https%3A%2F%2Fwww.google.com%2F 
    std::cout << d3lib::Cryto::URIComponent::urlDecode("https%3A%2F%2Fwww.google.com%2F") << std::endl; //https://www.google.com/
}