#include <iostream>
#include <set>
#include <string>
#include <QThread>
#include <sstream>
#include <cmath>
#include "datetime.h"
#include "event.h"
#include "alarm.h"
#include "split.h"

using namespace std;

const int numEventMembers = 12;
#define EVENTSOUND "Timer.wav" //default event alarm sound

/****************************** Constructors ********************************/

//default event constructor
Event::Event(int y1, int mon1, int d1, int h1, int min1, int s1,
             int y2, int mon2, int d2, int h2, int min2, int s2,
             int y3, int mon3 ,int d3, int h3, int min3, int s3,
             const string &n, const string &c, const string &descr){

    Datetime sdt(y1, mon1, d1, h1, min1, s1);
    Datetime edt(y2, mon2, d2, h2, min2, s2);
    Datetime forAlarm(y3, mon3, d3, h3, min3, s3);
    Alarm tempAlarm(forAlarm, "__Event Alarm__", 0, 0, 0, 0, 0, EVENTSOUND); //use the Alarm constructor that takes a datetime object
    startDatetime = sdt;
    endDatetime = edt;
    alarm = tempAlarm;
    description = descr;
    name = n;
    color = c;
    doneFlagEvent = 0;
    cout << saveEvent() << endl;
}

//read from file event constructor
Event::Event(const string &s, long lineNum) {
    vector <string> in = split(s, ',');
    if(in.size() != numEventMembers) {
        cout << "Event data corrupted on line number: " << lineNum + 1 << endl; //error handling
    }
    else {
        int i = 0; //counter
        cout << "Constructing Event\n";
        stringstream dF(in[i++]);
        dF >> doneFlagEvent;

        name = in[i++];

        time_t temp_time; //create a dump variable that we overwrite

        stringstream sdt(in[i++]);
        sdt >> temp_time;
        Datetime temp_sdt(temp_time);
        startDatetime = temp_sdt;

        stringstream edt(in[i++]);
        edt >> temp_time;
        Datetime temp_edt(temp_time);
        endDatetime = temp_edt;

        stringstream d(in[i++]);
        d >> description;

        stringstream c(in[i++]);
        c >> color;

        string forAlarm = in[i++];
        int j = i; //j is a count of where we are on this line
        for(i; i < numAlarmMembers + j; ) {
            forAlarm = forAlarm + "," + in[i++];
        }
        cout << forAlarm << endl;
        Alarm temp_alarm(forAlarm, 0); //make an alarm
        alarm = temp_alarm;
        cout << saveEvent() << endl;
    }

}

/*************************** Operator Overloads ******************************/

//operator < override to compare Events, return true if e1 is earlier than e2
bool operator < (const Event &e1, const Event &e2){

    if (e1.startDatetime < e2.startDatetime)
        return true;
    else return false;
}

bool operator - (const Event &e1, const Event &e2){
    if (fabs(e1.startDatetime - e2.startDatetime) <= 10) {};
}

/******************* Display/String Creator Functions ***********************/
string Event::saveEvent(){

    return to_string(doneFlagEvent) + "," + name + "," + to_string(startDatetime.get_cpuTime()) + "," +
            to_string(endDatetime.get_cpuTime()) + "," + description + "," + color + "," + alarm.saveAlarm();
}

string Event::getParams() {
    Datetime temp = alarm.get_Datetime();
    return name + "," + description + "," + color + "," + startDatetime.getTimeString() + ","
            + endDatetime.getTimeString() + "," + temp.getTimeString();
}

//Check if Event is done
int Event::checkEvent(){
    Datetime temp("now");
    long diff = endDatetime - temp;
    //cout << "diff: " << diff << endl; //debugging info if needed
    if (diff < 0){
        doneFlagEvent = 1;
        return 1; // 1 = Event done
    }

    else {
        return 0; //keep on waiting...waiting on the world to change
    }

}

/***************************** Ring functions *******************************/

//Should it ring?
int Event::ringNotification(){
    int eventRinger = alarm.checkRing();
    if (eventRinger > 0) {
        eventRinger = 0;
        return 1; //1 = ring
    }
    return 0;
}
