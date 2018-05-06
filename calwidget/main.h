#ifndef MAIN_H
#define MAIN_H

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
#include "datetime.h"
#include "alarm.h"
#include "event.h"
#include "split.h"
#include "window.h"

using namespace std;

extern std::set <Alarm *> alarmSet;
extern std::set <Event *> eventSet;
extern int alarmCount;
extern int eventCount;
extern int eventAlarmCount;
extern int programEnd;

vector <string> openFile(const string &filename);

extern string alarmFilename;
extern string eventFilename;

extern vector <string> allAlarms;
extern vector <string> doneAlarms;

extern vector <string> allEvents;
extern vector <string> doneEvents;

void alarmCheck(); //thread to check for alarms.
void eventCheck();

void createAlarm(const string &n, int y, int mon, int d, int h, int min,
int s, int nr, int riD, int riH, int riM, int riS, const string &sound_file);

void createEvent(int y1, int mon1, int d1, int h1, int min1, int s1,
int y2, int mon2, int d2, int h2, int min2, int s2,
int y3, int mon3, int d3, int h3, int min3, int s3,
const string &n, const string &c, const string &descr);

#endif // MAIN_H
