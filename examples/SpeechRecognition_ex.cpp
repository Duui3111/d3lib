#include "../d3lib/SpeechRecognition.h"
#include <iostream>

int main()
{
	d3lib::SpeechRecognition sr;
	d3lib::SpeechRecognition sri;

	sr.loadSpeak(d3lib::SpeechRecognition::Gender::MALE);
	sr.Speak("Say time to get the current time");

	char* input;
	sri.Input(input);
	
	if(std::string(input) == "Time") sr.Speak((std::string("the time is ") + std::string(__TIME__)).c_str());
	else sr.Speak((const char*)input);

	std::cout << input << std::endl;
}