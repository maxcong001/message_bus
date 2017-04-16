#include "MessageBus.hpp"
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
        cout<<"drive OK"<<endl;
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
        cout<<"car drives "<<speed<<endl;
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
        cout<<"Bus drives "<<speed<<endl;
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
        cout<<"Truck drives "<<speed<<endl;
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
    subject.SendReq("");
}
int main()
{
    TestBus();
    return 0;

}