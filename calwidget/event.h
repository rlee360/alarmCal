//Event.h
#ifndef EVENT_H
#define EVENT_H

#include <iostream>
#include <string>
#include <vector>
#include "datetime.h"
#include "alarm.h"

using namespace std;

extern const int numEventMembers;

class Event{
private:
    Datetime startDatetime;
    Datetime endDatetime;
    //Alarm notification;
    Alarm alarm;
    string name;
    string color; //hex? nah
    string description;
    int doneFlagEvent;

public:
    Event(){} //blank constructor for instantiating blank events

    Event(int y1, int mon1, int d1, int h1, int min1, int s1,
          int y2, int mon2, int d2, int h2, int min2, int s2,
          int y3, int mon3, int d3, int h3, int min3, int s3,
          const string &n, const string &c, const string &descr); //full blown constructor

    Event(const string &s, long lineNum); //read from file constructor

    string getParams(); //returns all the parameters of an event for gui

    string saveEvent(); //create a string to write to file

    int ringNotification(); //should it ring? Has the alarm been reached? Return 1 if so. 1 and done.

    int checkEvent(); //check whether and event is due.

    Datetime get_startDatetime() {
        return startDatetime; //return the startDatetime class
    }

    time_t get_event_start_cpuTime() {
        return startDatetime.get_cpuTime();
    }

    //operator < override to compare Events, return true if e1 is earlier than e2
    friend bool operator < (const Event &e1, const Event &e2);
    friend bool operator - (const Event &e1, const Event &e2);
};



#endif // EVENT_H
