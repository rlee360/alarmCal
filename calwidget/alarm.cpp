//alarm.cpp

#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <time.h>
#include <QDebug>
#include "datetime.h"
#include "alarm.h"
#include "split.h"

//conform with POSIX standard
#if defined(_WIN32)
    #define RINGSOUND(sound) cout << "\a";
#elif defined(__linux__)
    #define RINGSOUND linuxSound //linux function
#elif defined(__APPLE__)
    #define RINGSOUND(sound) cout << "\a";
#endif //defined

#define PRINTER qDebug()

#define MARGIN_OF_ERROR 15


using namespace std;

const int numAlarmMembers = 6;

/****************************** Constructors ********************************/

//alarm constructor for events
Alarm::Alarm(time_t t) {
    Datetime temp(t);
    ringTime = temp;
    numRepeats = 0;
    repeatInterval = 0;
    doneFlag = 0;
}

//stop watch constructor
Alarm::Alarm(int minsUntil, int nr, int riM, const string &sound_file) { //stopwatch code
    minsUntil = minsUntil * 60; //turn it into seconds until...
    Datetime temp("now");
    temp.changeTime(minsUntil);
    ringTime = temp;

    numRepeats = nr;
    repeatInterval = riM * 60; //repeat interval resolution is in minutes
    doneFlag = 0;
    sound = sound_file;
}

//use a datetime class to construct an alarm
Alarm::Alarm(const Datetime &t, const string &s, int nr, int riD, int riH, int riM, int riS, const string &sound_file) {
    name = s;
    ringTime = t; //invoke assignment operator here
    numRepeats = nr;
    repeatInterval = 0;
    repeatInterval += riD * 86400;
    repeatInterval += riH * 3600;
    repeatInterval += riM * 60;
    repeatInterval += riS;
    sound = sound_file;
    doneFlag = 0;
}

 //Constructor for read from file
Alarm::Alarm(const string &s, long lineNum) {
    vector <string> in = split(s, ',');
    if(in.size() != numAlarmMembers) {
        cout << "Alarm data corrupted on line number: " << lineNum + 1 << endl; //error handling
    }
    else {
        int i = 0; //counter
        cout << "Constructing alarm\n";
        stringstream dF(in[i++]);
        dF >> doneFlag;

        name = in[i++];

        time_t temp_time;
        stringstream dt(in[i++]);
        dt >> temp_time;
        Datetime temp(temp_time);
        ringTime = temp;

        stringstream nr(in[i++]);
        nr >> numRepeats;

        stringstream ri(in[i++]);
        ri >> repeatInterval;

        stringstream s(in[i++]);
        s >> sound;

        print();
    }

}

//default alarm constructor: set all the parameters
Alarm::Alarm(const string &n, int y, int mon, int d, int h, int min, int s, int nr, int riD, int riH, int riM, int riS, const string &sound_file) {
    name = n;
    Datetime temp(y, mon, d, h, min, s); //make a temp datetime class
    ringTime = temp; //assignment operator overload
    numRepeats = nr;
    repeatInterval = 0;
    repeatInterval += riD * 86400;
    repeatInterval += riH * 3600;
    repeatInterval += riM * 60;
    repeatInterval += riS;
    doneFlag = 0;
    sound = sound_file;
    print();
}


/*************************** Operator Overloads ******************************/

bool operator < (Alarm &s, Alarm &t) {
    if(s.ringTime < t.ringTime) {
        return true;
    }
    else {
        return false;
    }
}

/******************* Display/String Creator Functions ***********************/

string Alarm::saveAlarm() {
    // return all the parameters of an alarm as string.
    return to_string(doneFlag) + "," + name + "," + to_string(ringTime.get_cpuTime()) + "," +
           to_string(numRepeats) + "," + to_string(repeatInterval) + "," + sound + "\n";
    }

void Alarm::print() {
    ringTime.printDatetime();
    PRINTER << "Repeat the alarm " << numRepeats << " times every " << repeatInterval << " " << endl;
}

string Alarm::getParams() {
   return name + "," + ringTime.getTimeString() + "," + sound + "," + to_string(numRepeats)
               + "," + to_string(repeatInterval);
}

/***************************** Ring functions *******************************/

int Alarm::checkRing() {

    //instantiate a temp object with now time
    Datetime temp("now");
    long diff = ringTime - temp; //find the difference between the times.

    if(abs(diff) < MARGIN_OF_ERROR && doneFlag == 0) { //if the time is within range and the alarm hasn't been done, then ring it and return the result
        return ring(); //return the ring result. 0 if more alarms to come. 1 and done :)
    }
    else if(diff < -10) { //if we're really behind, just forget it and leave.
        doneFlag = 1; //set the flag to say we're not going to bother
        return 1;
    }

    return 0;
}

int Alarm::ring() {
    RINGSOUND(sound);
    if(numRepeats > 0) {
        numRepeats--;
        ringTime.changeTime(repeatInterval); //pass in the number of seconds we want to advance the datetime clock by.
        ringTime.printDatetime();
        return 0;
    }

    doneFlag = 1; //1 and done :P
    return 1;
}

void Alarm::linuxSound(const string &soundName) {
    string tmpString = "aplay ./alarmCal_sounds/" + soundName;
    const char *cmd = tmpString.c_str();
    system(cmd);
}
