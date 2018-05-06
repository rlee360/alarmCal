#ifndef ALARM_H
#define ALARM_H

#include <iostream>
#include <string>

using namespace std;

extern const int numAlarmMembers;

class Alarm {
    private:
        int doneFlag;
        string name;
        Datetime ringTime;
        int numRepeats;
        int repeatInterval;
        string sound;

    public:
        Alarm() {} //blank constructor if needed

        //stop watch constructor
        Alarm(int minsUntil, int nr, int riM, const string &sound_file);

        //use a datetime class to construct an alarm
        Alarm(const Datetime &t, const string &s, int nr, int riD, int riH, int riM, int riS, const string &sound_file);

        //alarm constructor for events
        Alarm(time_t t);

        //read from file alarm constructor
        Alarm(const string &s, long lineNum);

        //standard alarm constructor
        Alarm(const string &n, int y, int mon, int d, int h, int min, int s, int nr, int riD, int riH, int riM, int riS, const string &sound_file);
        /*  For human reading:
            riD = repeat Interval Days
            riH = repeat Interval Hours
            riM = repeat Interval Minutes
            riS = repeat Interval Seconds
        */

        string saveAlarm(); //return a string with all alarm parameters

        void print(); //print some debugging info, backend primarily

        int checkRing(); //check if it should ring. Ring the alarm if it should. returns 1 if all alarms are done. Returns 0 more to come.

        int ring(); //actually force ring an alarm

        //return the datetime, used for some comparisons
        Datetime get_Datetime() {
            return ringTime;
        }

        //return the cpu time
        time_t get_alarm_cpuTime() {
            return ringTime.get_cpuTime();
        }

        //get parameters for gui to display
        string getParams();

        //a function that takes the soundname string
        //this is employed in the direct text replacement done in alarm.cpp
        void linuxSound(const string &soundName);

        //comparator overload based on datetimes
        friend bool operator < (Alarm &s, Alarm &t);
};


#endif //ALARM_H
