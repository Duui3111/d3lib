#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <cctype>
#include <ctime>
#include <map>

#ifdef _WIN32
	#include <Windows.h>

	enum struct ColorCode { 
		COLORCODE_RED = 4, 
		COLORCODE_GREEN = 2, 
		COLORCODE_BLUE = 1,
		COLORCODE_YELLOW = 14,
		COLORCODE_WHITE = 15,
		COLORCODE_DEFAULT  = 7,
	};

	#define PrintColorConsole(color) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WORD)color);
	#define CreateDir(path) CreateDirectoryA((LPCSTR)path.substr(0, path.rfind('/', path.length())).c_str(), NULL);
#else
	#include <sys/types.h>
	#include <sys/stat.h>
	#include <unistd.h>

	enum struct ColorCode {
		COLORCODE_RED  = 31,
		COLORCODE_GREEN = 32,
		COLORCODE_BLUE = 34,
		COLORCODE_YELLOW = 33,
		COLORCODE_WHITE = 37,
		COLORCODE_DEFAULT = 39,
	};

	#define PrintColorConsole(color) std::cout << "\033[" << (int)color << "m";
	#define CreateDir(path) mkdir(path.c_str(), 0777);
#endif

namespace d3lib
{
	class Logger
	{
	private:
		class Log
		{
		private:
			std::string Logmessage;
			int LogLevel;

			std::map<int, std::string> clevel = {{1, "INFO"}, {2, "SUCCSES"}, {4, "ERROR"}, {14, "WARN"}, {15, "TRACE"}, {7, "DEBUG"}};

			static void LoggerFile(std::string path, const std::string &type, const std::string &message, std::string timestamp = "%Y-%m-%d %H:%M:%S")
			{
				std::time_t t = std::time(nullptr);
				CreateDir(path);
				std::ofstream ofs(path, std::ios::app);
				ofs << "[" << type << "] " << "[" << std::put_time(std::localtime(&t), timestamp.c_str()) << "] - " << message << std::endl;
			}

			static void LogMessage(std::string message, std::string type, int color, std::string timestamp = "%Y-%m-%d %H:%M:%S")
			{
				std::time_t t = std::time(nullptr);
				PrintColorConsole(color);
				std::cout << "[" << type << "]" << "[" << std::put_time(std::localtime(&t), timestamp.c_str()) << "] - " << message << std::endl;
				PrintColorConsole(ColorCode::COLORCODE_DEFAULT);
			}

			std::string ToLowerCase(std::string text) 
			{				
				std::transform(text.begin(), text.end(), text.begin(), [](unsigned char c){ return std::tolower(c); });			
				return text;
			}

		public:
			Log(std::string message, ColorCode color) : Logmessage(message), LogLevel((int)color) {}

			Log File() { LoggerFile("./Logs/" + ToLowerCase(clevel[LogLevel]) + ".log", clevel[LogLevel], Logmessage); return *this; }
			Log File(std::string path) { LoggerFile(path, clevel[LogLevel], Logmessage); return *this; }
			Log File(std::string path, std::string type) { LoggerFile(path, type, Logmessage); return *this; }
			Log File(std::string path, std::string type, std::string format) { LoggerFile(path, type, Logmessage, format); return *this; }

			Log Console() { LogMessage(Logmessage, clevel[LogLevel], LogLevel); return *this; }
			Log Console(std::string type) { LogMessage(Logmessage, type, LogLevel); return *this; }
			Log Console(std::string type, std::string format) { LogMessage(Logmessage, type, LogLevel, format); return *this; }
		};

	public:
		static Log Debug(std::string message) { return Log(message, ColorCode::COLORCODE_DEFAULT); }
		static Log Info(std::string message) { return Log(message, ColorCode::COLORCODE_BLUE); }
		static Log Succses(std::string message) { return Log(message, ColorCode::COLORCODE_GREEN); }
		static Log Warn(std::string message) { return Log(message, ColorCode::COLORCODE_YELLOW); }
		static Log Trace(std::string message) { return Log(message, ColorCode::COLORCODE_WHITE); }
		static Log Error(std::string message) { return Log(message, ColorCode::COLORCODE_RED); }
		static Log Custom(std::string message, int color) { return Log(message, (ColorCode)color); }
	};
}

#endif