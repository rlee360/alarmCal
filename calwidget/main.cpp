#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <fstream>  //file stream
#include <sstream>
#include <time.h>   //For cross compatibility
#include <ctime>    // struct std::tm
#include <chrono>   //for the thread sleep
#include <thread>   //for threading. Be sure to compile with the -pthread or -lpthread option (os specific afaik)
#include <unistd.h> //for sleep
#include <cmath>
#include <QThread>
#include <QtConcurrent/QtConcurrent>
#include "datetime.h"
#include "alarm.h"
#include "event.h"
#include "split.h"
#include "window.h"
#include "main.h"

#define NOW "now" //would put this in datetime, but didn't want to obfuscate

using namespace std;

std::set <Alarm *> alarmSet;
std::set <Event *> eventSet;

vector <string> allAlarms;
vector <string> doneAlarms; //these are the alarms we don't really care about

vector <string> allEvents;
vector <string> doneEvents;

string alarmFilename = "./alarms.csv";
string eventFilename = "./events.csv"; //home/sysuser1404/Downloads/alarmCalFinal-gui/build-calwidget-Desktop-Release

int alarmCount = 0;
int eventCount = 0;
int eventAlarmCount = 0;
int programEnd = 0;

void alarmCheck() {
    cout << "Alarm Thread Initialized\n";
    //start by opening file. openFile returns a vector of strings, with each element being a line
    allAlarms = openFile(alarmFilename);

    //for every element in allAlarms vector, splitter it into a new vector via comments
    for(int i = 0; i < (int)allAlarms.size() ; i++) {
        vector <string> tmp = split(allAlarms[i], ','); //utilize a splitter function located below.
        if(tmp[0] == "0") { //check if alarm has yet to ring
            alarmSet.insert(new Alarm(allAlarms[i], i)); //if so, insert into alarm set
            alarmCount++; //make a note of how many alarms we have
            cout << "New alarm added, new alarm count: " << alarmCount << endl;
        }
        else if(tmp[0] == "1") { //If alarm is done, then just push it into the doneAlarms vector
            doneAlarms.push_back(allAlarms[i]);
        }
    }


    while(1) { //check forever, or at least until all events are DONE
        for(auto it=alarmSet.begin(); it!=alarmSet.end(); ++it) {
            //checkRing returns 1
            if(((*it)->checkRing()) > 0) {
                alarmCount--;

            }
            else {
            }
        }
        if(programEnd > 0) {
            break;
        }
        this_thread::sleep_for(std::chrono::seconds(2));  //sleep(2);
    }

    ofstream outFile(alarmFilename, ofstream::out);
    for(auto it=doneAlarms.begin(); it!=doneAlarms.end(); ++it) {
        outFile << *it << endl;
    }

    for(auto it=alarmSet.begin(); it!=alarmSet.end(); ++it) {
        cout << (*it)->saveAlarm();
        outFile << (*it)->saveAlarm();
        delete *it;
    }
    outFile.close();


}

void eventCheck() {
    cout << "Event Thread Initialized\n";
    //start by opening file. openFile returns a vector of strings, with each element being a line
    allEvents = openFile(eventFilename);

    //for every element in allAlarms vector, split it into a new vector via commas
    for(int i = 0; i < allEvents.size() ; i++) {
        vector <string> tmp = split(allEvents[i], ','); //utilize a split function
        if(tmp[0] == "0") { //check if alarm has yet to ring
            eventSet.insert(new Event(allEvents[i], i)); //if so, insert into event set
            eventCount++; //make a note of how many events we have
            eventAlarmCount++;
            cout << "New event added, new event count: " << eventCount << endl; /////////////////////////
        }
        else if(tmp[0] == "1") { //If alarm is done, then just push it into the doneAlarms vector
            doneEvents.push_back(allEvents[i]);
        }
    }

    while(1) { //check forever, or at least until all events are DONE
        for(auto it=eventSet.begin(); it!=eventSet.end(); ++it) {
            //checkRing returns 1
            if(((*it)->ringNotification()) > 0) {
                eventAlarmCount--;
            }
            else {}


            if(((*it)->checkEvent()) > 0) {
                eventCount--;
            }

        }
        if(programEnd > 0) {
            break;
        }
        this_thread::sleep_for(std::chrono::seconds(2));  //sleep(2);
    }
    ofstream outFile(eventFilename, ofstream::out);
    for(auto it=doneEvents.begin(); it!=doneEvents.end(); ++it) {
        outFile << *it << endl;
    }

    for(auto it=eventSet.begin(); it!=eventSet.end(); ++it) {
        cout << (*it)->saveEvent();
        outFile << (*it)->saveEvent();
        delete *it;
    }
    outFile.close();
}


vector <string> openFile(const string &filename) {
    string tmp;
    vector <string> inText;
    ifstream infile(filename); //open up an input file stream using the file string
    if(infile.is_open()) { //some error checking
        while(getline(infile, tmp)) {
            if(tmp.length() == 0) {
                continue;
            }
            inText.push_back(tmp);
        }
    }
    infile.close();
    return inText;
}

void createAlarm(const string &n, int y, int mon, int d, int h, int min,
int s, int nr, int riD, int riH, int riM, int riS, const string &sound_file)
{
    alarmSet.insert(new Alarm(n, y, mon, d, h, min, s, nr, riD, riH, riM, riS, sound_file));
}

void createEvent(int y1, int mon1, int d1, int h1, int min1, int s1,
int y2, int mon2, int d2, int h2, int min2, int s2,
int y3, int mon3, int d3, int h3, int min3, int s3,
const string &n, const string &c, const string &descr)
{
    eventSet.insert(new Event(y1, mon1, d1, h1, min1, s1, y2, mon2, d2, h2, min2, s2,
y3, mon3, d3, h3, min3, s3, n, c, descr));
}

int main(int argc, char *argv[])
{
    QFuture<void> alarmThread = QtConcurrent::run(alarmCheck);
    QFuture<void> eventThread = QtConcurrent::run(eventCheck);

    QApplication app(argc, argv);
    Window window;
    window.show();
    return app.exec();
}

