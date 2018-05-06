//datetime.h

#ifndef DATETIME_H
#define DATETIME_H

#include <iostream>
#include <string>
#include <time.h> //For cross compatibility

using namespace std;

//BE SURE TO ALWAYS SET isdst FLAG, bad things happen otherwise.

class Datetime{
    private:
        //datetime parameters
        time_t cpuTime; //linux time equivalent
        string dayOfWeek;
        string timeString; //a string in YYYY/MM/DD HH:MM:SS format
        tm wholeTime; //time struct, not sure if the struct is needed...

        //functions
        void setDayOfWeek(int year, int month, int day); //actually determines what to set the day of the week to be.
        void setCurrentTime(); //used with the "now" constructor
        void setTimeString(); //sets the timestring...not sure if we actually need it
        int checkDST(int year, int mon, int day); //check if daylight savings is active. Returns 1 if between march and november
        int calcDayOfWeek(int year, int month, int day); //calc day of week just returns a number w/ the day of the week

    public:
        Datetime() {} //blank constructor needed for object creation in classes

        Datetime(int y, int mon, int d, int h, int min, int s); //takes a date and time and formats into object

        Datetime(string n); //takes a "now" keyword to set the current time

        Datetime(time_t t); //take a linux time and populate datetime object.

        ~Datetime(); //standard destructor

        Datetime(const Datetime &t); //copy constructor

        Datetime& operator = (const Datetime &t);

        void printDatetime(); //prints some parameters

        time_t get_cpuTime() { //current time in linux time
            return cpuTime;
        }

        string getTimeString() { //returns time strings for the gui
            return timeString;
        }

        void changeTime(long t); //pass in the number of seconds to change by. alarm/event will manage this.
        friend bool operator <= (const Datetime &s, const Datetime &t);
        friend bool operator < (const Datetime &s, const Datetime &t);
        friend bool operator == (const Datetime &s, const Datetime &t);
        friend long operator - (const Datetime &s, const Datetime &t);
};

#endif //DATETIME_H
