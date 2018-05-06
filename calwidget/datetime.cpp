//datetime.cpp

#include <iostream>
#include <string>
#include <QDebug>
#include <cmath>
#include <time.h> //For cross compatibility
#include "datetime.h"

using namespace std;

#define MARGIN_OF_ERROR 15 //up to n seconds error on checking for == operator
#define PRINTER cout

/****************************** Constructors ********************************/
Datetime::Datetime(time_t t) {
    cpuTime = t;
    changeTime(0); //clever use of changeTime to set the datetime object clock: change the clock by zero seconds
}

Datetime::Datetime(string n) {
    if(n == "now") {
        setCurrentTime();
    }
}

Datetime::Datetime(int y, int mon, int d, int h, int min, int s) {
    wholeTime.tm_year = y - 1900;
    wholeTime.tm_mon = mon - 1; //this is because months are zero indexed
    wholeTime.tm_mday = d;
    wholeTime.tm_hour = h;
    wholeTime.tm_min = min;
    wholeTime.tm_sec = s;

    setDayOfWeek(y, mon, d); //setDayOfWeek sets wholeTime.tm_wday for us :)

    wholeTime.tm_isdst = checkDST(y,mon,d); //check if the provided date is in daylight savings

    cpuTime = mktime(&wholeTime);
//	struct tm *test = localtime(&cpuTime); //make a time struct using linux time...
//	cout << "DST Flag: " << test->tm_isdst << endl; // debugging code

    wholeTime.tm_year = y;
    wholeTime.tm_mon = mon;

    setTimeString(); //configure the time string to say the right time

}

Datetime::Datetime(const Datetime &t) { //copy constructor; sets everything equal
    cpuTime = t.cpuTime;
    dayOfWeek = t.dayOfWeek;
    timeString = t.timeString;
    wholeTime = t.wholeTime;
}

Datetime::~Datetime() {}

/*************************** Operator Overloads ******************************/

Datetime& Datetime::operator = (const Datetime &t) { //operator overload; also sets everything equal
    cpuTime = t.cpuTime;
    dayOfWeek = t.dayOfWeek;
    timeString = t.timeString;
    wholeTime = t.wholeTime;
//	cout << wholeTime.tm_hour << endl; //debug
}

bool operator < (const Datetime &s, const Datetime &t) {
    if(s.cpuTime < t.cpuTime) {
        return true;
    }
    else if( s.cpuTime >= t.cpuTime) {
        return false;
    }
}

bool operator <= (const Datetime &s, const Datetime &t) {
    if(s.cpuTime <= t.cpuTime) {
        return true;
    }
    else if( s.cpuTime > t.cpuTime) {
        return false;
    }
}

bool operator == (const Datetime &s, const Datetime &t) {
    if(abs(t.cpuTime - s.cpuTime) < MARGIN_OF_ERROR) {
        return true;
    }
    else {
        return false;
    }
}

long operator - (const Datetime &s, const Datetime &t) {
    time_t t1 = s.cpuTime;
    time_t t2 = t.cpuTime;
    return t1-t2; //return the time diff in seconds.
}

/****************************** Display params *******************************/
void Datetime::printDatetime() {
    PRINTER << "Datetime: " << timeString << endl;
    PRINTER << "Day of Week: " << dayOfWeek << endl;
    PRINTER << "Linux Time: " << cpuTime << endl;
    PRINTER << "Days since Sunday: " << wholeTime.tm_wday << endl;
}

/****************************** Time Modifiers *******************************/
void Datetime::setCurrentTime() {
    // current date/time based on current system time
    time_t now = time(0);
    tm *ltm = localtime(&now);
    cpuTime = now; //make the cpu time now
    wholeTime.tm_year = 1900 + ltm->tm_year;
    wholeTime.tm_mon =  1 + ltm->tm_mon;
    wholeTime.tm_mday =  ltm->tm_mday;
    wholeTime.tm_hour = ltm->tm_hour;
    wholeTime.tm_min =  ltm->tm_min;
    wholeTime.tm_sec = ltm->tm_sec;
    wholeTime.tm_wday =  ltm->tm_wday; //Day of week is implicitly built-in to tm struct :)
    wholeTime.tm_isdst = ltm->tm_isdst; //it seems that localtime handles dst for us, so we don't need to; still good to have.

    if (wholeTime.tm_wday == 0) {
        dayOfWeek = "Sunday";
    }
    if (wholeTime.tm_wday == 1) {
        dayOfWeek = "Monday";
    }
    if (wholeTime.tm_wday == 2) {
        dayOfWeek = "Tuesday";
    }
    if (wholeTime.tm_wday == 3) {
        dayOfWeek = "Wednesday";
    }
    if (wholeTime.tm_wday == 4) {
        dayOfWeek = "Thursday";
    }
    if (wholeTime.tm_wday == 5) {
        dayOfWeek = "Friday";
    }
    if (wholeTime.tm_wday == 6) {
        dayOfWeek = "Saturday";
    }
    setTimeString(); //configure the time string to say the right time
}


void Datetime::changeTime(long t) { //takes t seconds and adds to cpuTime, also resetting wholeTime based on new cpuTime
    cout << "Advancing by: " << t << " seconds." << endl;
    cpuTime += t; //advance the cpu time by t seconds.
    tm *ltm = localtime(&cpuTime); //need to change to the wholeTime struct for closure
    wholeTime.tm_year = 1900 + ltm->tm_year;
    wholeTime.tm_mon =  1 + ltm->tm_mon;
    wholeTime.tm_mday =  ltm->tm_mday;
    wholeTime.tm_hour = ltm->tm_hour;
    wholeTime.tm_min =  ltm->tm_min;
    wholeTime.tm_sec = ltm->tm_sec;
    wholeTime.tm_wday =  ltm->tm_wday; //Day of week is implicitly built-in to tm struct :)
    wholeTime.tm_isdst = ltm->tm_isdst; //it seems that localtime handles dst for us, so we don't need to; still good to have.

    if (wholeTime.tm_wday == 0) {
        dayOfWeek = "Sunday";
    }
    if (wholeTime.tm_wday == 1) {
        dayOfWeek = "Monday";
    }
    if (wholeTime.tm_wday == 2) {
        dayOfWeek = "Tuesday";
    }
    if (wholeTime.tm_wday == 3) {
        dayOfWeek = "Wednesday";
    }
    if (wholeTime.tm_wday == 4) {
        dayOfWeek = "Thursday";
    }
    if (wholeTime.tm_wday == 5) {
        dayOfWeek = "Friday";
    }
    if (wholeTime.tm_wday == 6) {
        dayOfWeek = "Saturday";
    }
    setTimeString();
}

void Datetime::setTimeString() {
    string time;
    int arr[] = {wholeTime.tm_hour, wholeTime.tm_min, wholeTime.tm_sec};
    int arrSize = sizeof(arr)/sizeof(arr[0]);
    for(int i = 0; i < arrSize; i++) {
        if(arr[i] < 10) {
            if(i != arrSize-1) {
                time = time + "0" + to_string(arr[i]) + ":";
            }
            else {
                time = time + "0" + to_string(arr[i]);
            }
        }
        else {
            if(i != arrSize-1) {
                time = time + to_string(arr[i]) + ":";
            }
            else {
                time = time + to_string(arr[i]);
            }
        }

    }
    //cout << "setTimeString: " << time << endl;
    timeString = to_string(wholeTime.tm_year) + "/" + to_string(wholeTime.tm_mon) + "/" + to_string(wholeTime.tm_mday) + " " + time;
}

int Datetime::checkDST(int year, int mon, int day) {
    int marchSunday,novSunday;
    if(mon < 3 || mon > 11) {
        return 0;
    }
    else if(mon >= 4 && mon <= 10) {
        return 1;
    }
    else {
        for(int i = 8; i <= 14; i++) {
            if(calcDayOfWeek(year, 11, i) == 0) {
                marchSunday = i;
                cout << "marchSunday: " << marchSunday << endl;
                break;
            }
        }
        for(int i = 1; i <= 7; i++) {
            if(calcDayOfWeek(year, 11, i) == 0) {
                novSunday = i;
                cout << "novSunday: " << novSunday << endl;
                break;
            }
        }

        if(mon == 3) {
            if(day >= marchSunday) {
                return 1;
            }
            else {
                return 0;
            }
        }

        else if(mon == 11) {
            if(day >= novSunday) {
                return 0;
            }
            else {
                return 1;
            }
        }
    }
}

/********************* Day of Week Calculators/Setters **********************/
int Datetime::calcDayOfWeek(int year, int month, int day){

    int num = (((year) % 100) / 4) + day;
    int leap; //1 if it is a leap year, 0 if it is not a leap year
    int century = (year / 100)*100;

    if (month == 1 || month == 10)
        num++;
    else if (month == 2 || month == 3 || month == 11)
        num += 4;
    else if (month == 5)
        num += 2;
    else if (month == 6)
        num += 5;
    else if (month == 8)
        num += 3;
    else if (month == 9 || month == 12)
        num += 6;

    if (year % 4 == 0){
        leap = 1;
    if (year % 100 == 0 && year % 400 == 0)
        leap = 1;
    else if (year % 100 == 0 && year % 400 != 0)
        leap = 0;
    }

    if (leap == 1 && (month == 1 || month == 2))
        num --;
    while(century != 1700 && century && century != 1800 && century != 1900 && century != 2000){
        if (century > 2000){
            century -= 400;
        }
        if (century < 1700){
            century += 400;
        }

    }

    if (century == 1700){
        num += 4;
    }
    else if (century == 1800){
        num += 2;
    }
    else if(century == 2000){
        num += 6;
    }

    num += year % 100;
    num--;
    num %= 7;
    return num;
}

void Datetime::setDayOfWeek(int year, int month, int day) {
    wholeTime.tm_wday = calcDayOfWeek(year, month, day);
    if (wholeTime.tm_wday == 0) {
        dayOfWeek = "Sunday";
    }
    if (wholeTime.tm_wday == 1) {
        dayOfWeek = "Monday";
    }
    if (wholeTime.tm_wday == 2) {
        dayOfWeek = "Tuesday";
    }
    if (wholeTime.tm_wday == 3) {
        dayOfWeek = "Wednesday";
    }
    if (wholeTime.tm_wday == 4) {
        dayOfWeek = "Thursday";
    }
    if (wholeTime.tm_wday == 5) {
        dayOfWeek = "Friday";
    }
    if (wholeTime.tm_wday == 6) {
        dayOfWeek = "Saturday";
    }
}
