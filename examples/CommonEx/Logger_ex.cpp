#include "../include/d3lib/Common/Logger.hpp"

int main()
{
    d3lib::Logger::Succses("hi this is a log").Console();
    d3lib::Logger::Error("hi this is a log").Console();
    d3lib::Logger::Info("hi this is a log").Console();
    d3lib::Logger::Warn("hi this is a log").Console();
    d3lib::Logger::Trace("hi this is a log").Console();
    d3lib::Logger::Debug("hi this is a log").Console();

    d3lib::Logger::Succses("hi this is a log").File();
    d3lib::Logger::Error("hi this is a log").File();
    d3lib::Logger::Info("hi this is a log").File();
    d3lib::Logger::Warn("hi this is a log").File();
    d3lib::Logger::Trace("hi this is a log").File();
    d3lib::Logger::Debug("hi this is a log").File();

    d3lib::Logger::Succses("hi this is a log 1").Console().File();
    d3lib::Logger::Error("hi this is a log 1").Console().File();
    d3lib::Logger::Info("hi this is a log 1").Console().File();
    d3lib::Logger::Warn("hi this is a log 1").Console().File();
    d3lib::Logger::Trace("hi this is a log 1").Console().File();
    d3lib::Logger::Debug("hi this is a log 1").Console().File();

    d3lib::Logger::Succses("hi this is a log 2").Console().File("Logs/logfile1.log");
    d3lib::Logger::Error("hi this is a log 2").Console().File("Logs/logfile1.log");
    d3lib::Logger::Info("hi this is a log 2").Console().File("Logs/logfile1.log");
    d3lib::Logger::Warn("hi this is a log 2").Console().File("Logs/logfile1.log");
    d3lib::Logger::Trace("hi this is a log 2").Console().File("Logs/logfile1.log");
    d3lib::Logger::Debug("hi this is a log 2").Console().File("Logs/logfile1.log");

    d3lib::Logger::Succses("hi this is a log 3").Console("CUSTOMTYPE").File("Logs/logfile1.log");
    d3lib::Logger::Error("hi this is a log 3").Console("CUSTOMTYPE").File("Logs/logfile1.log");
    d3lib::Logger::Info("hi this is a log 3").Console("CUSTOMTYPE").File("Logs/logfile1.log");
    d3lib::Logger::Warn("hi this is a log 3").Console("CUSTOMTYPE").File("Logs/logfile1.log");
    d3lib::Logger::Trace("hi this is a log 3").Console("CUSTOMTYPE").File("Logs/logfile1.log");
    d3lib::Logger::Debug("hi this is a log 3").Console("CUSTOMTYPE").File("Logs/logfile1.log");

    d3lib::Logger::Succses("hi this is a log 4").Console().File("Logs/logfile1.log", "CUSTOMTYPE");
    d3lib::Logger::Error("hi this is a log 4").Console().File("Logs/logfile1.log", "CUSTOMTYPE");
    d3lib::Logger::Info("hi this is a log 4").Console().File("Logs/logfile1.log", "CUSTOMTYPE");
    d3lib::Logger::Warn("hi this is a log 4").Console().File("Logs/logfile1.log", "CUSTOMTYPE");
    d3lib::Logger::Trace("hi this is a log 4").Console().File("Logs/logfile1.log", "CUSTOMTYPE");
    d3lib::Logger::Debug("hi this is a log 4").Console().File("Logs/logfile1.log", "CUSTOMTYPE");

    d3lib::Logger::Succses("hi this is a log 5").Console("CUSTOMTYPE").File("Logs/logfile1.log", "CUSTOMTYPE");
    d3lib::Logger::Error("hi this is a log 5").Console("CUSTOMTYPE").File("Logs/logfile1.log", "CUSTOMTYPE");
    d3lib::Logger::Info("hi this is a log 5").Console("CUSTOMTYPE").File("Logs/logfile1.log", "CUSTOMTYPE");
    d3lib::Logger::Warn("hi this is a log 5").Console("CUSTOMTYPE").File("Logs/logfile1.log", "CUSTOMTYPE");
    d3lib::Logger::Trace("hi this is a log 5").Console("CUSTOMTYPE").File("Logs/logfile1.log", "CUSTOMTYPE");
    d3lib::Logger::Debug("hi this is a log 5").Console("CUSTOMTYPE").File("Logs/logfile1.log", "CUSTOMTYPE");
    
    d3lib::Logger::Succses("hi this is a log 6").Console().File("Logs/logfile1.log", "CUSTOMTYPE", "%H:%M:%S");
    d3lib::Logger::Error("hi this is a log 6").Console().File("Logs/logfile1.log", "CUSTOMTYPE","%H:%M:%S");
    d3lib::Logger::Info("hi this is a log 6").Console().File("Logs/logfile1.log", "CUSTOMTYPE", "%H:%M:%S");
    d3lib::Logger::Warn("hi this is a log 6").Console().File("Logs/logfile1.log", "CUSTOMTYPE", "%H:%M:%S");
    d3lib::Logger::Trace("hi this is a log 6").Console().File("Logs/logfile1.log", "CUSTOMTYPE", "%H:%M:%S");
    d3lib::Logger::Debug("hi this is a log 6").Console().File("Logs/logfile1.log", "CUSTOMTYPE", "%H:%M:%S");

    d3lib::Logger::Succses("hi this is a log 7").Console("CUSTOMTYPE", "%H:%M:%S").File("Logs/logfile1.log", "CUSTOMTYPE");
    d3lib::Logger::Error("hi this is a log 7").Console("CUSTOMTYPE", "%H:%M:%S").File("Logs/logfile1.log", "CUSTOMTYPE");
    d3lib::Logger::Info("hi this is a log 7").Console("CUSTOMTYPE", "%H:%M:%S").File("Logs/logfile1.log", "CUSTOMTYPE");
    d3lib::Logger::Warn("hi this is a log 7").Console("CUSTOMTYPE", "%H:%M:%S").File("Logs/logfile1.log", "CUSTOMTYPE");
    d3lib::Logger::Trace("hi this is a log 7").Console("CUSTOMTYPE", "%H:%M:%S").File("Logs/logfile1.log", "CUSTOMTYPE");
    d3lib::Logger::Debug("hi this is a log 7").Console("CUSTOMTYPE", "%H:%M:%S").File("Logs/logfile1.log", "CUSTOMTYPE");

    d3lib::Logger::Custom("hi this is a log 8", 2).Console("Custom Log Type").File("Logs/logfile2.log", "My Custom Type");

    return 0;
}