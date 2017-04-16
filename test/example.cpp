#include "MessageBus.hpp"
#include <log4cplus/logger.h>
//#include <log4cplus/consoleappender.h>
#include <log4cplus/fileappender.h>
#include <log4cplus/layout.h>
//#include <log4cplus/ndc.h>
//#include <log4cplus/mdc.h>
#include <log4cplus/helpers/loglog.h>
#include <log4cplus/thread/threads.h>
//#include <log4cplus/helpers/sleep.h>
#include <log4cplus/loggingmacros.h>


using namespace std;
using namespace log4cplus;
using namespace log4cplus::helpers;
Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("Max:"));


MessageBus g_bus;
const string Topic = "Drive";
const string CallBackTopic = "DriveOK";

struct Subject
{
    Subject()
    {
        g_bus.Attach([this]{DriveOK();},CallBackTopic);
    }
    void SendReq(const string& topic)
    {
        g_bus.SendReq<void, int>(50, topic);
    }
    void DriveOK()
    {
		LOG4CPLUS_DEBUG(logger, "drive OK");
    }
};

struct Car
{
    Car()
    {
        g_bus.Attach([this](int speed){Drive(speed);}, Topic);
    }

    void Drive(int speed)
    {
        LOG4CPLUS_DEBUG(logger, "Car drives : "<<speed);
        g_bus.SendReq<void>(CallBackTopic);
    }
};
struct Bus
{
    Bus()
    {
        g_bus.Attach([this](int speed){Drive(speed);});
    }

    void Drive(int speed)
    {
		LOG4CPLUS_DEBUG(logger, "Bus drives : "<<speed);
        g_bus.SendReq<void>(CallBackTopic);
    }
};
struct Truck
{
    Truck()
    {
        g_bus.Attach([this](int speed){Drive(speed);});
    }

    void Drive(int speed)
    {
		LOG4CPLUS_DEBUG(logger, "Truck drives : "<<speed);
        g_bus.SendReq<void>(CallBackTopic);
    }
};

void TestBus()
{
    Subject subject;
    Car car;
    Bus bus;
    Truck truck;
    //subject.SendReq(Topic);
	for (int i = 0; i < 10000; i++)
	{
		subject.SendReq("");
	}
    
}
int main()
{
    log4cplus::initialize ();
    try {
        SharedObjectPtr<Appender> append_1(new FileAppender("Test.log"));
        append_1->setName(LOG4CPLUS_TEXT("First"));

        log4cplus::tstring pattern = LOG4CPLUS_TEXT("[%d{%m/%d/%y %H:%M:%S,%Q}] %c %-5p - %m [%l]%n");
        //  std::tstring pattern = LOG4CPLUS_TEXT("%d{%c} [%t] %-5p [%.15c{3}] %%%x%% - %m [%l]%n");
        append_1->setLayout( std::auto_ptr<Layout>(new PatternLayout(pattern)) );
        Logger::getRoot().addAppender(append_1);

        logger.setLogLevel(DEBUG_LOG_LEVEL);

    }
    catch(...) {
        Logger::getRoot().log(FATAL_LOG_LEVEL, LOG4CPLUS_TEXT("Exception occured..."));
    }

    LOG4CPLUS_DEBUG(logger, "set logger done!"<<"\nhello log4cplus\n");

    TestBus();
    return 0;

}