#include <QtWidgets>
#include <QString>
#include <QWindow>
#include <QSpinBox>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QIODevice>
#include <vector>
#include <iostream>
#include <fstream>
#include <chrono>   //for the thread sleep
#include <thread>   //for threading. Be sure to compile with the -pthread or -lpthread option (os specific afaik)
#include "window.h"
#include "main.h"
#include "split.h"
#include "alarm.h"
#include "event.h"

#define STOPWATCH "Chime.wav" //stop watch code
#define SOUNDFILETYPE ".wav"

Window::Window()
{
    createPreviewGroupBox();
    createGeneralOptionsGroupBox();
    createDatesGroupBox();  // Overwritten by createDatesGroupBox()
    createTextFormatsGroupBox();
    createAddEventGroupBox();
    createViewEventGroupBox();
    createAddAlarmGroupBox();
    createViewAlarmGroupBox();
    createStopwatchGroupBox(); // Replaces createDatesGroupBox()

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(viewEventGroupBox, 0, 0);
    layout->addWidget(viewAlarmGroupBox, 1, 0);
    layout->addWidget(previewGroupBox, 0, 1);
    //layout->addWidget(datesGroupBox, 1, 1); // Replaced by createDatesGroupBox()
    layout->addWidget(viewStopwatchGroupBox, 1, 1);
    layout->addWidget(generalOptionsGroupBox, 0, 2);
    layout->addWidget(textFormatsGroupBox, 1, 2);
    layout->addWidget(addEventGroupBox, 0, 3);
    layout->addWidget(addAlarmGroupBox, 1, 3);
    layout->setSizeConstraint(QLayout::SetFixedSize);
    setLayout(layout);

    previewLayout->setRowMinimumHeight(0, calendar->sizeHint().height());
    previewLayout->setColumnMinimumWidth(0, calendar->sizeHint().width());

    setWindowTitle(tr("alarmCal"));
}

void Window::closeEvent(QCloseEvent *event)
{
    //tell the threads it's time to end
    programEnd = 1;
}

//Once the Create Stopwatch Button is clicked, save all the input fields
void Window::stopwatchConfirmationPushButtonClicked()
{
    tempStopwatchRingIn = stopwatchRingInSpinBox->value();
    tempStopwatchRepeat = stopwatchRepeatSpinBox->value();
    tempEvery = stopwatchEverySpinBox->value();

    // Pass the data that we saved to temporary variables to a pointer to an alarm object that will act as a stopwatch

    alarmSet.insert(new Alarm(tempStopwatchRingIn, tempStopwatchRepeat, tempEvery, STOPWATCH));
    alarmCount++;

    // Clear the input fields

    stopwatchRingInSpinBox->setValue(0);
    stopwatchRepeatSpinBox->setValue(0);
    stopwatchEverySpinBox->setValue(0);
}

//Once the Create Event Button is clicked, save all the input fields
void Window::addEventConfirmationPushButtonClicked()
{
    tempEventName = addEventNameLineEdit->text().toStdString();
    tempEventDescription = addEventDescriptionLineEdit->text().toStdString();

    tempEventStartDateYear = addEventStartDateEdit->date().year();
    tempEventStartDateMonth = addEventStartDateEdit->date().month();
    tempEventStartDateDay = addEventStartDateEdit->date().day();

    tempEventStartTimeHour = addEventStartTimeEdit->time().hour();
    tempEventStartTimeMinute = addEventStartTimeEdit->time().minute();
    tempEventStartTimeSecond = addEventStartTimeEdit->time().second();

    tempEventEndDateYear = addEventEndDateEdit->date().year();
    tempEventEndDateMonth = addEventEndDateEdit->date().month();
    tempEventEndDateDay = addEventEndDateEdit->date().day();

    tempEventEndTimeHour = addEventEndTimeEdit->time().hour();
    tempEventEndTimeMinute = addEventEndTimeEdit->time().minute();
    tempEventEndTimeSecond = addEventEndTimeEdit->time().second();

    tempEventReminderDateYear = addEventReminderDateEdit->date().year();
    tempEventReminderDateMonth = addEventReminderDateEdit->date().month();
    tempEventReminderDateDay = addEventReminderDateEdit->date().day();

    tempEventReminderTimeHour = addEventReminderTimeEdit->time().hour();
    tempEventReminderTimeMinute = addEventReminderTimeEdit->time().minute();
    tempEventReminderTimeSecond = addEventReminderTimeEdit->time().second();

    tempEventColor = addEventColorCombo->currentText().toStdString();

    // Pass the data that we saved to temporary variables to a pointer to an event object

    eventSet.insert(new Event(tempEventStartDateYear, tempEventStartDateMonth, tempEventStartDateDay,
                tempEventStartTimeHour, tempEventStartTimeMinute, tempEventStartTimeSecond,
                tempEventEndDateYear, tempEventEndDateMonth, tempEventEndDateDay,
                tempEventEndTimeHour, tempEventEndTimeMinute, tempEventEndTimeSecond,
                tempEventReminderDateYear, tempEventReminderDateMonth, tempEventReminderDateDay,
                tempEventReminderTimeHour, tempEventReminderTimeMinute, tempEventReminderTimeSecond,
                tempEventName, tempEventColor, tempEventDescription));
    eventCount++;

    // Display the data for the event that was just created

    tempQStr1 = QString::fromStdString(tempEventName);
    displayEventNameLabel->setText(tempQStr1);
    tempQStr2 = QString::fromStdString(tempEventDescription);
    displayEventDescriptionLabel->setText(tempQStr2);
    tempQStr3 = QString::number(tempEventStartDateMonth) + QStringLiteral("/") + QString::number(tempEventStartDateDay)
            + QStringLiteral("/") + QString::number(tempEventStartDateYear) + QStringLiteral(" ")
            + QString::number(tempEventStartTimeHour) + QStringLiteral(":") + QString::number(tempEventStartTimeMinute)
            + QStringLiteral(":") + QString::number(tempEventStartTimeSecond);
    displayEventStartDateTimeLabel->setText(tempQStr3);
    tempQStr4 = QString::number(tempEventEndDateMonth) + QStringLiteral("/") + QString::number(tempEventEndDateDay)
            + QStringLiteral("/") + QString::number(tempEventEndDateYear) + QStringLiteral(" ")
            + QString::number(tempEventEndTimeHour) + QStringLiteral(":") + QString::number(tempEventEndTimeMinute)
            + QStringLiteral(":") + QString::number(tempEventEndTimeSecond);
    displayEventEndDateTimeLabel->setText(tempQStr4);
    tempQStr5 = QString::number(tempEventReminderDateMonth) + QStringLiteral("/") + QString::number(tempEventReminderDateDay)
            + QStringLiteral("/") + QString::number(tempEventReminderDateYear) + QStringLiteral(" ")
            + QString::number(tempEventReminderTimeHour) + QStringLiteral(":") + QString::number(tempEventReminderTimeMinute)
            + QStringLiteral(":") + QString::number(tempEventReminderTimeSecond);
    displayEventReminderLabel->setText(tempQStr5);
    tempQStr6 = QString::fromStdString(tempEventColor);
    displayEventColorLabel->setText(tempQStr6);

    // Clear the input fields

    QTime time(0, 0, 0, 0);
    addEventNameLineEdit->clear();
    addEventDescriptionLineEdit->clear();
    addEventStartDateEdit->setDate(currentDate);
    addEventStartTimeEdit->setTime(time);
    addEventEndDateEdit->setDate(currentDate);
    addEventEndTimeEdit->setTime(time);
    addEventReminderDateEdit->setDate(currentDate);
    addEventReminderTimeEdit->setTime(time);
    addEventColorCombo->setCurrentIndex(3);
    QCoreApplication::processEvents(); //refresh
}


//Once the Create Alarm Button is clicked, save all the filled out info
void Window::addAlarmConfirmationPushButtonClicked()
{
    tempAlarmName = addEventAlarmNameLineEdit->text().toStdString();

    tempAlarmStartDateYear = addEventAlarmDateEdit->date().year();
    tempAlarmStartDateMonth = addEventAlarmDateEdit->date().month();
    tempAlarmStartDateDay = addEventAlarmDateEdit->date().day();

    tempAlarmStartTimeHour = addEventAlarmTimeEdit->time().hour();
    tempAlarmStartTimeMinute = addEventAlarmTimeEdit->time().minute();
    tempAlarmStartTimeSecond = addEventAlarmTimeEdit->time().second();

    tempAlarmNumerOfRepeats = addEventNumberOfRepeatsSpinBox->value();

    tempIntervalBetweenRepeats = addEventIntervalSpinBox->value();

    tempIntervalHours = addEventIntervalTimeEdit->time().hour();
    tempIntervalMinutes = addEventIntervalTimeEdit->time().minute();
    tempIntervalSeconds = addEventIntervalTimeEdit->time().second();

    tempAlarmSound = addEventSoundComboBox->currentText().toStdString() + SOUNDFILETYPE;
    tempAlarmSound2 = addEventSoundComboBox->currentText().toStdString();

    // Pass the data that we saved to temporary variables to a pointer to an alarm object

    alarmSet.insert(new Alarm(tempAlarmName, tempAlarmStartDateYear, tempAlarmStartDateMonth,
                tempAlarmStartDateDay, tempAlarmStartTimeHour, tempAlarmStartTimeMinute,
                tempAlarmStartTimeSecond, tempAlarmNumerOfRepeats, tempIntervalBetweenRepeats,
                tempIntervalHours, tempIntervalMinutes, tempIntervalSeconds, tempAlarmSound));
    alarmCount++;

    // Display the data for the event that was just created

    tempQStr7 = QString::fromStdString(tempAlarmName);
    displayEventAlarmNameLabel->setText(tempQStr7);
    tempQStr8 = QString::number(tempAlarmStartDateMonth) + QStringLiteral("/") + QString::number(tempAlarmStartDateDay)
            + QStringLiteral("/") + QString::number(tempAlarmStartDateYear) + QStringLiteral(" ")
            + QString::number(tempAlarmStartTimeHour) + QStringLiteral(":") + QString::number(tempAlarmStartTimeMinute)
            + QStringLiteral(":") + QString::number(tempAlarmStartTimeSecond);
    displayEventAlarmTimeLabel->setText(tempQStr8);
    tempQStr9 = QString::number(tempAlarmNumerOfRepeats);
    displayEventNumberOfRepeatsLabel->setText(tempQStr9);
    tempQStr10 = QString::number(tempIntervalBetweenRepeats) + QStringLiteral(" ") + QString::number(tempIntervalHours) +
            QStringLiteral(":") + QString::number(tempIntervalMinutes) + QStringLiteral(":") + QString::number(tempIntervalSeconds);
    displayEventIntervalLabel->setText(tempQStr10);
    tempQStr11 = QString::fromStdString(tempAlarmSound2);
    displayEventSoundLabel->setText(tempQStr11);

    // Clear the input fields

    QTime time(0, 0, 0, 0);
    addEventAlarmNameLineEdit->clear();
    addEventAlarmDateEdit->setDate(currentDate);
    addEventAlarmTimeEdit->setTime(time);
    addEventNumberOfRepeatsSpinBox->setValue(0);
    addEventIntervalSpinBox->setValue(0);
    addEventIntervalTimeEdit->setTime(time);
    addEventSoundComboBox->setCurrentIndex(0);

    QCoreApplication::processEvents(); //refresh
}

//we declare these down here so we can see them right above the code that needs them
int eventViewer = 0;
int alarmViewer = 0;

//When a user presses a previous or next button
void Window::viewNextEventPushButtonClicked()
{
    eventViewer++;
    viewEvents();
    viewAlarms();
}

void Window::viewPreviousEventPushButtonClicked()
{
    eventViewer--;
    viewEvents();
    viewAlarms();
}

void Window::viewNextAlarmPushButtonClicked()
{
    alarmViewer++;
    viewEvents();
    viewAlarms();
}

void Window::viewPreviousAlarmPushButtonClicked()
{
    alarmViewer--;
    viewEvents();
    viewAlarms();
}

//Once the Selected Date is changed, display events for that day
void Window::viewEventsAndAlarmsSelectedDateChanged()
{
    alarmViewer = 0;
    eventViewer = 0;
    viewEvents();
    viewAlarms();
}

//Additional Event comparator
typedef struct {
    bool operator () (Event* &s, Event* &t) {
        if(s->get_event_start_cpuTime() < t->get_event_start_cpuTime()) {
            return true;
        }
        else {
            return false;
        }
    }
}eventSorter;

void Window::viewEvents()
{
    using namespace std;

    int y = currentDateEdit->date().year();
    int m = currentDateEdit->date().month();
    int d = currentDateEdit->date().day();


    Datetime beginning(y,m,d,0,0,0);
    Datetime end(y,m,d,23,59,59);

    vector <Event *> displayEvents;

    for(auto it = eventSet.begin(); it != eventSet.end(); ++it) {
        Datetime tempEvent = (*it)->get_startDatetime(); //it is an event with a function called getDatetime()
        if( beginning.get_cpuTime() <= tempEvent.get_cpuTime() && tempEvent.get_cpuTime() <= end.get_cpuTime()) {
            displayEvents.push_back(*it); //append to vector
        }
    }

    sort(displayEvents.begin(), displayEvents.end(), eventSorter());

    if(displayEvents.size() == 0){
        return;
    }
    if(eventViewer >= displayEvents.size()){
        eventViewer = 0;
    }
    else if(eventViewer < 0){
        eventViewer = displayEvents.size() - 1;
    }
    vector <string> eventString = split(displayEvents[eventViewer]->getParams(), ',');
    int counter = 0;
    tempQStr12 = QString::fromStdString(eventString[counter++]);
    displayEventNameLabel->setText(tempQStr12);
    tempQStr13 = QString::fromStdString(eventString[counter++]);
    displayEventDescriptionLabel->setText(tempQStr13);
    tempQStr14 = QString::fromStdString(eventString[counter++]);
    displayEventColorLabel->setText(tempQStr14);
    tempQStr15 = QString::fromStdString(eventString[counter++]);
    displayEventStartDateTimeLabel->setText(tempQStr15);
    tempQStr16 = QString::fromStdString(eventString[counter++]);
    displayEventEndDateTimeLabel->setText(tempQStr16);
    tempQStr17 = QString::fromStdString(eventString[counter++]);
    displayEventReminderLabel->setText(tempQStr17);

    QCoreApplication::processEvents(); //refresh
}

//Additional Alarm comparator
typedef struct {
    bool operator () (Alarm* &s, Alarm* &t) {
        if(s->get_alarm_cpuTime() < t->get_alarm_cpuTime()) {
            return true;
        }
        else {
            return false;
        }
    }
}alarmSorter;

void Window::viewAlarms()
{
    using namespace std;

    int y = currentDateEdit->date().year();
    int m = currentDateEdit->date().month();
    int d = currentDateEdit->date().day();


    Datetime beginning(y,m,d,0,0,0);
    Datetime end(y,m,d,23,59,59);

    vector <Alarm *> displayAlarms;

    for(auto it = alarmSet.begin(); it != alarmSet.end(); ++it) {
        //Datetime tempAlarm = (*it)->get_Datetime(); //it is an event with a function called getDatetime()
        if( beginning.get_cpuTime() <= (*it)->get_alarm_cpuTime() && (*it)->get_alarm_cpuTime() <= end.get_cpuTime()) {
            displayAlarms.push_back(*it); //append to vector
        }
    }
    //sort alarms here
    sort(displayAlarms.begin(), displayAlarms.end(), alarmSorter());
    if(displayAlarms.size() == 0){
        return;
    }
    if(alarmViewer >= displayAlarms.size()){
        alarmViewer = 0;
    }
    else if(alarmViewer < 0){
        alarmViewer = displayAlarms.size() - 1;
    }

    vector <string> alarmString = split(displayAlarms[alarmViewer]->getParams(), ',');
    int counter2 = 0;
    tempQStr18 = QString::fromStdString(alarmString[counter2++]);
    displayEventAlarmNameLabel->setText(tempQStr18);
    tempQStr19 = QString::fromStdString(alarmString[counter2++]);
    displayEventAlarmTimeLabel->setText(tempQStr19);
    vector<string> tempSound = split(alarmString[counter2++], '/');
    tempQStr20 = QString::fromStdString(tempSound.back());
    displayEventSoundLabel->setText(tempQStr20);
    tempQStr21 = QString::fromStdString(alarmString[counter2++]);
    displayEventNumberOfRepeatsLabel->setText(tempQStr21);
    tempQStr22 = QString::fromStdString(alarmString[counter2++]);
    displayEventIntervalLabel->setText(tempQStr22);

    QCoreApplication::processEvents(); //refresh
}

//everything above here is a SLOT or FUNCTION whose purpose is
//condensing code from a createGroupBox function

//Call this in two cases:
        // When viewEventsAndAlarmsSelectedDateChanged() is called
        // When addEventConfirmationPushButtonClicked() is called
void Window::displayViewEventLabels()
{
    displayEventNameLabel = new QLabel(tr(""));
    displayEventStartDateTimeLabel = new QLabel(tr(""));
    displayEventEndDateTimeLabel = new QLabel(tr(""));
    displayEventDescriptionLabel = new QLabel(tr(""));
    displayEventReminderLabel = new QLabel(tr(""));
    displayEventColorLabel = new QLabel(tr(""));
}

//Call this in two cases:
        // When viewEventsAndAlarmsSelectedDateChanged() is called
        // When addAlarmConfirmationPushButtonClicked() is called
void Window::displayViewAlarmLabels()
{
    displayEventAlarmNameLabel = new QLabel(tr(""));
    displayEventAlarmTimeLabel = new QLabel(tr(""));
    displayEventNumberOfRepeatsLabel = new QLabel(tr(""));
    displayEventIntervalLabel = new QLabel(tr(""));
    displayEventSoundLabel = new QLabel(tr(""));
}

void Window::localeChanged(int index)
{
    const QLocale newLocale(localeCombo->itemData(index).toLocale());
    calendar->setLocale(newLocale);
    int newLocaleFirstDayIndex = firstDayCombo->findData(newLocale.firstDayOfWeek());
    firstDayCombo->setCurrentIndex(newLocaleFirstDayIndex);
}

void Window::firstDayChanged(int index)
{
    calendar->setFirstDayOfWeek(Qt::DayOfWeek(
                                firstDayCombo->itemData(index).toInt()));
}

void Window::selectionModeChanged(int index)
{
    calendar->setSelectionMode(QCalendarWidget::SelectionMode(
                               selectionModeCombo->itemData(index).toInt()));
}

void Window::horizontalHeaderChanged(int index)
{
    calendar->setHorizontalHeaderFormat(QCalendarWidget::HorizontalHeaderFormat(
        horizontalHeaderCombo->itemData(index).toInt()));
}

void Window::verticalHeaderChanged(int index)
{
    calendar->setVerticalHeaderFormat(QCalendarWidget::VerticalHeaderFormat(
        verticalHeaderCombo->itemData(index).toInt()));
}

void Window::selectedDateChanged()
{
    currentDateEdit->setDate(calendar->selectedDate());
}

void Window::minimumDateChanged(const QDate &date)
{
    calendar->setMinimumDate(date);
    maximumDateEdit->setDate(calendar->maximumDate());
}

void Window::maximumDateChanged(const QDate &date)
{
    calendar->setMaximumDate(date);
    minimumDateEdit->setDate(calendar->minimumDate());
}

void Window::setEventEndDateEditMinimum()
{
    addEventEndDateEdit->setMinimumDate(addEventStartDateEdit->date());
    addEventEndDateEdit->setDate(addEventStartDateEdit->date());
}

void Window::setEventEndTimeEditMinimum()
{
    addEventEndTimeEdit->setMinimumTime(addEventStartTimeEdit->time());
    addEventEndTimeEdit->setTime(addEventStartTimeEdit->time());
}

void Window::weekdayFormatChanged()
{
    QTextCharFormat format;

    format.setForeground(qvariant_cast<QColor>(
        weekdayColorCombo->itemData(weekdayColorCombo->currentIndex())));
    calendar->setWeekdayTextFormat(Qt::Monday, format);
    calendar->setWeekdayTextFormat(Qt::Tuesday, format);
    calendar->setWeekdayTextFormat(Qt::Wednesday, format);
    calendar->setWeekdayTextFormat(Qt::Thursday, format);
    calendar->setWeekdayTextFormat(Qt::Friday, format);
}

void Window::weekendFormatChanged()
{
    QTextCharFormat format;

    format.setForeground(qvariant_cast<QColor>(
        weekendColorCombo->itemData(weekendColorCombo->currentIndex())));
    calendar->setWeekdayTextFormat(Qt::Saturday, format);
    calendar->setWeekdayTextFormat(Qt::Sunday, format);
}

void Window::reformatHeaders()
{
    QString text = headerTextFormatCombo->currentText();
    QTextCharFormat format;

    if (text == tr("Bold")) {
        format.setFontWeight(QFont::Bold);
    } else if (text == tr("Italic")) {
        format.setFontItalic(true);
    } else if (text == tr("Green")) {
        format.setForeground(Qt::green);
    }
    calendar->setHeaderTextFormat(format);
}


void Window::reformatCalendarPage()
{
    QTextCharFormat FirstFormat;
    const QDate First(calendar->yearShown(), 5, 1);

    QTextCharFormat FridayFormat;
    QDate Friday(calendar->yearShown(), calendar->monthShown(), 1);
    while (Friday.dayOfWeek() != Qt::Friday)
        Friday = Friday.addDays(1);

    if (FridayCheckBox->isChecked()) {
        FridayFormat.setForeground(Qt::blue);
    } else { // Revert to regular colour for this day of the week.
        Qt::DayOfWeek dayOfWeek(static_cast<Qt::DayOfWeek>(Friday.dayOfWeek()));
        FridayFormat.setForeground(calendar->weekdayTextFormat(dayOfWeek).foreground());
    }

    calendar->setDateTextFormat(Friday, FridayFormat);

    // When it is checked, "First in Red" always takes precedence over "Friday in Blue".
    if (FirstCheckBox->isChecked()) {
        FirstFormat.setForeground(Qt::red);
    } else if (!FridayCheckBox->isChecked() || Friday != First) {
        // We can now be certain we won't be resetting "First in Red" when we restore
        // the firsts regular colour for this day of the week.
        Qt::DayOfWeek dayOfWeek(static_cast<Qt::DayOfWeek>(First.dayOfWeek()));
        calendar->setDateTextFormat(First, calendar->weekdayTextFormat(dayOfWeek));
    }

    calendar->setDateTextFormat(First, FirstFormat);
}


void Window::changeDisplayEventNameLabel(QString &q)
{
    displayEventNameLabel->setText(q);
    QApplication::processEvents();
}

//everything above here is a SLOT or FUNCTION whose purpose is
//condensing code from a createGroupBox function

//everything below here is a createGroupBox function

void Window::createPreviewGroupBox()
{
    previewGroupBox = new QGroupBox(tr("Preview"));

    calendar = new QCalendarWidget;
    calendar->setMinimumDate(QDate(999, 1, 1));
    calendar->setMaximumDate(QDate(9999, 1, 1));
    calendar->setGridVisible(true);

    connect(calendar, SIGNAL(currentPageChanged(int,int)),
            this, SLOT(reformatCalendarPage()));

    previewLayout = new QGridLayout;
    previewLayout->addWidget(calendar, 0, 0, Qt::AlignCenter);
    previewGroupBox->setLayout(previewLayout);
}

void Window::createGeneralOptionsGroupBox()
{
    generalOptionsGroupBox = new QGroupBox(tr("General Options"));

    //Bonus Option for changing language, took out so there'd be more space for other things
    /*
    localeCombo = new QComboBox;
    int curLocaleIndex = -1;
    int index = 0;
    for (int _lang = QLocale::C; _lang <= QLocale::LastLanguage; ++_lang) {
        QLocale::Language lang = static_cast<QLocale::Language>(_lang);
        QList<QLocale::Country> countries = QLocale::countriesForLanguage(lang);
        for (int i = 0; i < countries.count(); ++i) {
            QLocale::Country country = countries.at(i);
            QString label = QLocale::languageToString(lang);
            label += QLatin1Char('/');
            label += QLocale::countryToString(country);
            QLocale locale(lang, country);
            if (this->locale().language() == lang && this->locale().country() == country)
                curLocaleIndex = index;
            localeCombo->addItem(label, locale);
            ++index;
        }
    }
    if (curLocaleIndex != -1)
        localeCombo->setCurrentIndex(curLocaleIndex);
    localeLabel = new QLabel(tr("&Locale:"));
    localeLabel->setBuddy(localeCombo);
    */

    firstDayCombo = new QComboBox;
    firstDayCombo->addItem(tr("Sunday"), Qt::Sunday);
    firstDayCombo->addItem(tr("Monday"), Qt::Monday);
    firstDayCombo->addItem(tr("Tuesday"), Qt::Tuesday);
    firstDayCombo->addItem(tr("Wednesday"), Qt::Wednesday);
    firstDayCombo->addItem(tr("Thursday"), Qt::Thursday);
    firstDayCombo->addItem(tr("Friday"), Qt::Friday);
    firstDayCombo->addItem(tr("Saturday"), Qt::Saturday);

    firstDayLabel = new QLabel(tr("Wee&k starts on:"));
    firstDayLabel->setBuddy(firstDayCombo);

    selectionModeCombo = new QComboBox;
    selectionModeCombo->addItem(tr("Single selection"),
                                QCalendarWidget::SingleSelection);
    selectionModeCombo->addItem(tr("None"), QCalendarWidget::NoSelection);

    selectionModeLabel = new QLabel(tr("&Selection mode:"));
    selectionModeLabel->setBuddy(selectionModeCombo);

    gridCheckBox = new QCheckBox(tr("&Grid"));
    gridCheckBox->setChecked(calendar->isGridVisible());

    navigationCheckBox = new QCheckBox(tr("&Navigation bar"));
    navigationCheckBox->setChecked(true);

    horizontalHeaderCombo = new QComboBox;
    horizontalHeaderCombo->addItem(tr("Single letter day names"),
                                   QCalendarWidget::SingleLetterDayNames);
    horizontalHeaderCombo->addItem(tr("Short day names"),
                                   QCalendarWidget::ShortDayNames);
    horizontalHeaderCombo->addItem(tr("None"),
                                   QCalendarWidget::NoHorizontalHeader);
    horizontalHeaderCombo->setCurrentIndex(1);

    horizontalHeaderLabel = new QLabel(tr("&Horizontal header:"));
    horizontalHeaderLabel->setBuddy(horizontalHeaderCombo);

    verticalHeaderCombo = new QComboBox;
    verticalHeaderCombo->addItem(tr("ISO week numbers"),
                                 QCalendarWidget::ISOWeekNumbers);
    verticalHeaderCombo->addItem(tr("None"), QCalendarWidget::NoVerticalHeader);

    verticalHeaderLabel = new QLabel(tr("&Vertical header:"));
    verticalHeaderLabel->setBuddy(verticalHeaderCombo);

    //connect(localeCombo, SIGNAL(currentIndexChanged(int)),
    //        this, SLOT(localeChanged(int)));
    connect(firstDayCombo, SIGNAL(currentIndexChanged(int)),
            this, SLOT(firstDayChanged(int)));
    connect(selectionModeCombo, SIGNAL(currentIndexChanged(int)),
            this, SLOT(selectionModeChanged(int)));
    connect(gridCheckBox, SIGNAL(toggled(bool)),
            calendar, SLOT(setGridVisible(bool)));
    connect(navigationCheckBox, SIGNAL(toggled(bool)),
            calendar, SLOT(setNavigationBarVisible(bool)));
    connect(horizontalHeaderCombo, SIGNAL(currentIndexChanged(int)),
            this, SLOT(horizontalHeaderChanged(int)));
    connect(verticalHeaderCombo, SIGNAL(currentIndexChanged(int)),
            this, SLOT(verticalHeaderChanged(int)));

    QHBoxLayout *checkBoxLayout = new QHBoxLayout;
    checkBoxLayout->addWidget(gridCheckBox);
    checkBoxLayout->addStretch();
    checkBoxLayout->addWidget(navigationCheckBox);

    QGridLayout *outerLayout = new QGridLayout;
    //outerLayout->addWidget(localeLabel, 0, 0); // taken out for spacing reasons, it works
    //outerLayout->addWidget(localeCombo, 0, 1); // taken out for spacing reasons, it works
    outerLayout->addWidget(firstDayLabel, 1, 0);
    outerLayout->addWidget(firstDayCombo, 1, 1);
    outerLayout->addWidget(selectionModeLabel, 2, 0);
    outerLayout->addWidget(selectionModeCombo, 2, 1);
    outerLayout->addLayout(checkBoxLayout, 3, 0, 1, 2);
    outerLayout->addWidget(horizontalHeaderLabel, 4, 0);
    outerLayout->addWidget(horizontalHeaderCombo, 4, 1);
    outerLayout->addWidget(verticalHeaderLabel, 5, 0);
    outerLayout->addWidget(verticalHeaderCombo, 5, 1);
    generalOptionsGroupBox->setLayout(outerLayout);

    firstDayChanged(firstDayCombo->currentIndex());
    selectionModeChanged(selectionModeCombo->currentIndex());
    horizontalHeaderChanged(horizontalHeaderCombo->currentIndex());
    verticalHeaderChanged(verticalHeaderCombo->currentIndex());
}

void Window::createDatesGroupBox()
{
    datesGroupBox = new QGroupBox(tr("Dates"));

    minimumDateEdit = new QDateEdit;
    minimumDateEdit->setDisplayFormat("MMMM d yyyy");
    minimumDateEdit->setDateRange(calendar->minimumDate(),
                                  calendar->maximumDate());
    minimumDateEdit->setDate(calendar->minimumDate());
    minimumDateEdit->setToolTip("Set Earliest Calendar Date");

    minimumDateLabel = new QLabel(tr("&Minimum Date:"));
    minimumDateLabel->setBuddy(minimumDateEdit);

    currentDateEdit = new QDateEdit;
    currentDateEdit->setDisplayFormat("MMMM d yyyy");
    currentDateEdit->setDate(calendar->selectedDate());
    currentDateEdit->setDateRange(calendar->minimumDate(),
                                  calendar->maximumDate());

    currentDateLabel = new QLabel(tr("&Current Date:"));
    currentDateLabel->setBuddy(currentDateEdit);

    maximumDateEdit = new QDateEdit;
    maximumDateEdit->setDisplayFormat("MMMM d yyyy");
    maximumDateEdit->setDateRange(calendar->minimumDate(),
                                  calendar->maximumDate());
    maximumDateEdit->setDate(calendar->maximumDate());
    maximumDateEdit->setToolTip("Set Latest Calendar Date");

    maximumDateLabel = new QLabel(tr("Ma&ximum Date:"));
    maximumDateLabel->setBuddy(maximumDateEdit);

    connect(currentDateEdit, SIGNAL(dateChanged(QDate)),
            calendar, SLOT(setSelectedDate(QDate)));
    connect(calendar, SIGNAL(selectionChanged()),
            this, SLOT(selectedDateChanged()));
    connect(calendar, SIGNAL(selectionChanged()),
            this, SLOT(viewEventsAndAlarmsSelectedDateChanged()));
    connect(minimumDateEdit, SIGNAL(dateChanged(QDate)),
            this, SLOT(minimumDateChanged(QDate)));
    connect(maximumDateEdit, SIGNAL(dateChanged(QDate)),
            this, SLOT(maximumDateChanged(QDate)));

    QGridLayout *dateBoxLayout = new QGridLayout;
    dateBoxLayout->addWidget(currentDateLabel, 1, 0);
    dateBoxLayout->addWidget(currentDateEdit, 1, 1);
    dateBoxLayout->addWidget(minimumDateLabel, 0, 0);
    dateBoxLayout->addWidget(minimumDateEdit, 0, 1);
    dateBoxLayout->addWidget(maximumDateLabel, 2, 0);
    dateBoxLayout->addWidget(maximumDateEdit, 2, 1);
    dateBoxLayout->setRowStretch(3, 1);

    datesGroupBox->setLayout(dateBoxLayout);
}

void Window::createTextFormatsGroupBox()
{
    textFormatsGroupBox = new QGroupBox(tr("Text Formats"));

    weekdayColorCombo = createColorComboBox();
    weekdayColorCombo->setCurrentIndex(
            weekdayColorCombo->findText(tr("Black")));

    weekdayColorLabel = new QLabel(tr("&Weekday color:"));
    weekdayColorLabel->setBuddy(weekdayColorCombo);

    weekendColorCombo = createColorComboBox();
    weekendColorCombo->setCurrentIndex(
            weekendColorCombo->findText(tr("Red")));

    weekendColorLabel = new QLabel(tr("Week&end color:"));
    weekendColorLabel->setBuddy(weekendColorCombo);

    headerTextFormatCombo = new QComboBox;
    headerTextFormatCombo->addItem(tr("Bold"));
    headerTextFormatCombo->addItem(tr("Italic"));
    headerTextFormatCombo->addItem(tr("Plain"));
    headerTextFormatLabel = new QLabel(tr("&Header text:"));
    headerTextFormatLabel->setBuddy(headerTextFormatCombo);

    FridayCheckBox = new QCheckBox(tr("&Friday in blue"));

    FirstCheckBox = new QCheckBox(tr("&First in red"));

    connect(weekdayColorCombo, SIGNAL(currentIndexChanged(int)),
            this, SLOT(weekdayFormatChanged()));
    connect(weekdayColorCombo, SIGNAL(currentIndexChanged(int)),
            this, SLOT(reformatCalendarPage()));
    connect(weekendColorCombo, SIGNAL(currentIndexChanged(int)),
            this, SLOT(weekendFormatChanged()));
    connect(weekendColorCombo, SIGNAL(currentIndexChanged(int)),
            this, SLOT(reformatCalendarPage()));
    connect(headerTextFormatCombo, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(reformatHeaders()));
    connect(FridayCheckBox, SIGNAL(toggled(bool)),
            this, SLOT(reformatCalendarPage()));
    connect(FirstCheckBox, SIGNAL(toggled(bool)),
            this, SLOT(reformatCalendarPage()));

    QHBoxLayout *checkBoxLayout = new QHBoxLayout;
    checkBoxLayout->addWidget(FridayCheckBox);
    checkBoxLayout->addStretch();
    checkBoxLayout->addWidget(FirstCheckBox);

    QGridLayout *outerLayout = new QGridLayout;
    outerLayout->addWidget(weekdayColorLabel, 0, 0);
    outerLayout->addWidget(weekdayColorCombo, 0, 1);
    outerLayout->addWidget(weekendColorLabel, 1, 0);
    outerLayout->addWidget(weekendColorCombo, 1, 1);
    outerLayout->addWidget(headerTextFormatLabel, 2, 0);
    outerLayout->addWidget(headerTextFormatCombo, 2, 1);
    outerLayout->addLayout(checkBoxLayout, 3, 0, 1, 2);
    textFormatsGroupBox->setLayout(outerLayout);

    weekdayFormatChanged();
    weekendFormatChanged();
    reformatHeaders();
    reformatCalendarPage();
}

QComboBox *Window::createColorComboBox()
{
    QComboBox *comboBox = new QComboBox;
    comboBox->addItem(tr("Red"), QColor(Qt::red));
    comboBox->addItem(tr("Blue"), QColor(Qt::blue));
    comboBox->addItem(tr("Green"), QColor(Qt::green));
    comboBox->addItem(tr("Black"), QColor(Qt::black));
    comboBox->addItem(tr("Yellow"), QColor(Qt::yellow));
    comboBox->addItem(tr("Magenta"), QColor(Qt::magenta));
    return comboBox;
}

void Window::createAddEventGroupBox()
{
    addEventGroupBox = new QGroupBox(tr("Add Event"));
    addEventNameLabel = new QLabel(tr("Name:"));
    addEventStartDateTimeLabel = new QLabel(tr("Event Start:"));
    addEventEndDateTimeLabel = new QLabel(tr("Event End:"));
    addEventDescriptionLabel = new QLabel(tr("Description:"));
    addEventReminderLabel = new QLabel(tr("Reminder:"));
    addEventColorLabel = new QLabel(tr("Color:"));

    addEventStartDateEdit = new QDateEdit;
        addEventStartDateEdit->setDisplayFormat("MMMM d yyyy");
        addEventStartDateEdit->setDate(currentDate);
        addEventStartDateEdit->setMinimumDate(currentDate);
    addEventEndDateEdit = new QDateEdit;
        addEventEndDateEdit->setDisplayFormat("MMMM d yyyy");
        addEventEndDateEdit->setDate(currentDate);
        addEventEndDateEdit->setMinimumDate(currentDate);
    addEventReminderDateEdit = new QDateEdit;
        addEventReminderDateEdit->setDisplayFormat("MMMM d yyyy");
        addEventReminderDateEdit->setDate(currentDate);
        addEventReminderDateEdit->setMinimumDate(currentDate);

    addEventColorCombo = createColorComboBox();
        addEventColorCombo->setCurrentIndex(
        addEventColorCombo->findText(tr("Black")));

    addEventStartTimeEdit = new QTimeEdit;
        addEventStartTimeEdit->setDisplayFormat("hh:mm:ss ap");
        addEventStartTimeEdit->setTime(currentTime);
        addEventStartTimeEdit->setMinimumTime(currentTime);
    addEventEndTimeEdit = new QTimeEdit;
        addEventEndTimeEdit->setDisplayFormat("hh:mm:ss ap");
        addEventEndTimeEdit->setTime(currentTime);
        addEventEndTimeEdit->setMinimumTime(currentTime);
    addEventReminderTimeEdit = new QTimeEdit;
        addEventReminderTimeEdit->setDisplayFormat("hh:mm:ss ap");
        addEventReminderTimeEdit->setTime(currentTime);
        addEventReminderTimeEdit->setMinimumTime(currentTime);

    addEventNameLineEdit = new QLineEdit;
        addEventNameLineEdit->setPlaceholderText("Event Name Here");
    addEventDescriptionLineEdit = new QLineEdit;
        addEventDescriptionLineEdit->setPlaceholderText("Event Description Here");

    addEventConfirmationPushButton = new QPushButton;
        addEventConfirmationPushButton->setText("Create Event");
        addEventConfirmationPushButton->setToolTip("Add Event to Calendar");

    QGridLayout *addEventLayout = new QGridLayout;
        addEventLayout->addWidget(addEventNameLabel, 0, 0);
        addEventLayout->addWidget(addEventNameLineEdit, 0, 1); // event name input
        addEventLayout->addWidget(addEventStartDateTimeLabel, 1, 0);
        addEventLayout->addWidget(addEventStartDateEdit, 1, 1); // start date input
        addEventLayout->addWidget(addEventStartTimeEdit, 1, 2); // start time input
        addEventLayout->addWidget(addEventEndDateTimeLabel, 2, 0);
        addEventLayout->addWidget(addEventEndDateEdit, 2, 1); // end date input
        addEventLayout->addWidget(addEventEndTimeEdit, 2, 2); // end time input
        addEventLayout->addWidget(addEventDescriptionLabel, 3, 0);
        addEventLayout->addWidget(addEventDescriptionLineEdit, 3, 1); // description input
        addEventLayout->addWidget(addEventReminderLabel, 4, 0);
        addEventLayout->addWidget(addEventReminderDateEdit, 4, 1); // reminder date input
        addEventLayout->addWidget(addEventReminderTimeEdit, 4, 2); // reminder time input
        addEventLayout->addWidget(addEventColorLabel, 5, 0);
        addEventLayout->addWidget(addEventColorCombo, 5, 1); // color input
        addEventLayout->addWidget(addEventConfirmationPushButton, 6, 0, 6, 3);
        addEventGroupBox->setLayout(addEventLayout);

        connect(addEventConfirmationPushButton, SIGNAL(clicked(bool)),
                this, SLOT(addEventConfirmationPushButtonClicked()));
        connect(addEventStartDateEdit, SIGNAL(dateChanged(QDate)),
                this, SLOT(setEventEndDateEditMinimum()));
        connect(addEventStartTimeEdit, SIGNAL(timeChanged(QTime)),
                this, SLOT(setEventEndTimeEditMinimum()));
}

void Window::createViewEventGroupBox()
{
    viewEventGroupBox = new QGroupBox(tr("Selected Day's Events"));
    viewEventNameLabel = new QLabel(tr("Name:"));
    viewEventStartDateTimeLabel = new QLabel(tr("Event Start:"));
    viewEventEndDateTimeLabel = new QLabel(tr("Event End:"));
    viewEventDescriptionLabel = new QLabel(tr("Description:"));
    viewEventReminderLabel = new QLabel(tr("Reminder:"));
    viewEventColorLabel = new QLabel(tr("Color:"));

    Window::displayViewEventLabels();

    viewNextEventPushButton = new QPushButton;
    viewNextEventPushButton->setText("Next Event Today");
    viewNextEventPushButton->setToolTip("Display Next Event Today");
    viewPreviousEventPushButton = new QPushButton;
    viewPreviousEventPushButton->setText("Previous Event Today");
    viewPreviousEventPushButton->setToolTip("Display Previous Event Today");

    connect(viewNextEventPushButton, SIGNAL(clicked(bool)),
            this, SLOT(viewNextEventPushButtonClicked()));

    connect(viewPreviousEventPushButton,SIGNAL(clicked(bool)),
            this, SLOT(viewPreviousEventPushButtonClicked()));

    QGridLayout *viewEventLayout = new QGridLayout;
    viewEventLayout->addWidget(viewEventNameLabel, 0, 0);
    viewEventLayout->addWidget(displayEventNameLabel, 0, 1); // event display
    viewEventLayout->addWidget(viewEventStartDateTimeLabel, 1, 0);
    viewEventLayout->addWidget(displayEventStartDateTimeLabel, 1, 1); // start date display
    viewEventLayout->addWidget(viewEventEndDateTimeLabel, 2, 0);
    viewEventLayout->addWidget(displayEventEndDateTimeLabel, 2, 1); // end date display
    viewEventLayout->addWidget(viewEventDescriptionLabel, 3, 0);
    viewEventLayout->addWidget(displayEventDescriptionLabel, 3, 1); // description display
    viewEventLayout->addWidget(viewEventReminderLabel, 4, 0);
    viewEventLayout->addWidget(displayEventReminderLabel, 4, 1); // reminder display
    viewEventLayout->addWidget(viewEventColorLabel, 5, 0);
    viewEventLayout->addWidget(displayEventColorLabel, 5, 1); // color display
    viewEventLayout->addWidget(viewPreviousEventPushButton, 6, 0);
    viewEventLayout->addWidget(viewNextEventPushButton, 6, 1);
    viewEventGroupBox->setLayout(viewEventLayout);
}

void Window::createAddAlarmGroupBox()
{
    addAlarmGroupBox = new QGroupBox(tr("Add Alarm"));
    addEventAlarmNameLabel = new QLabel(tr("Alarm Name:"));
    addEventAlarmTimeLabel = new QLabel(tr("Alarm Time:"));
    addEventSoundLabel = new QLabel(tr("Alarm Sound:"));
    addEventNumberOfRepeatsLabel = new QLabel(tr("Number of Repeats:"));
    addEventIntervalLabel = new QLabel(tr("Time Between Repeats:"));

    addEventAlarmDateEdit = new QDateEdit;
        addEventAlarmDateEdit->setDisplayFormat("MMMM d yyyy");
        addEventAlarmDateEdit->setMinimumDate(currentDate);
        addEventAlarmDateEdit->setDate(currentDate);
    addEventAlarmTimeEdit = new QTimeEdit;
        addEventAlarmTimeEdit->setDisplayFormat("hh:mm:ss ap");
        addEventAlarmTimeEdit->setMinimumTime(currentTime);
        addEventAlarmTimeEdit->setTime(currentTime);
    addEventIntervalTimeEdit = new QTimeEdit;
        addEventIntervalTimeEdit->setDisplayFormat("hh:mm:ss");
        addEventIntervalTimeEdit->setToolTip("Time Frequency");

    addEventAlarmNameLineEdit = new QLineEdit;
        addEventAlarmNameLineEdit->setPlaceholderText("Alarm Name Here");

    addEventSoundComboBox = new QComboBox;
        addEventSoundComboBox->addItem(tr("Blossom"));
        addEventSoundComboBox->addItem(tr("Chime"));
        addEventSoundComboBox->addItem(tr("Wind"));
        addEventSoundComboBox->addItem(tr("Morning"));
        addEventSoundComboBox->addItem(tr("Timer"));

    addEventNumberOfRepeatsSpinBox = new QSpinBox;
        addEventNumberOfRepeatsSpinBox->setRange(0, 100);
        addEventNumberOfRepeatsSpinBox->setSingleStep(1);
        addEventNumberOfRepeatsSpinBox->setValue(0);

    addAlarmConfirmationPushButton = new QPushButton;
        addAlarmConfirmationPushButton->setText("Create Alarm");
        addAlarmConfirmationPushButton->setToolTip("Add Alarm to Calendar");

    addEventIntervalSpinBox = new QSpinBox;
        addEventIntervalSpinBox->setRange(0, 365);
        addEventIntervalSpinBox->setSingleStep(1);
        addEventIntervalSpinBox->setValue(0);
        addEventIntervalSpinBox->setToolTip("Day Frequency");

    QGridLayout *addAlarmLayout = new QGridLayout;
    addAlarmLayout->addWidget(addEventAlarmNameLabel, 0, 0);
    addAlarmLayout->addWidget(addEventAlarmNameLineEdit, 0, 1); // alarm name input
    addAlarmLayout->addWidget(addEventAlarmTimeLabel, 1, 0);
    addAlarmLayout->addWidget(addEventAlarmDateEdit, 1, 1); // alarm date input
    addAlarmLayout->addWidget(addEventAlarmTimeEdit, 1, 2); // alarm time imput
    addAlarmLayout->addWidget(addEventSoundLabel, 2, 0);
    addAlarmLayout->addWidget(addEventSoundComboBox, 2, 1); // sound input
    addAlarmLayout->addWidget(addEventNumberOfRepeatsLabel, 3, 0);
    addAlarmLayout->addWidget(addEventNumberOfRepeatsSpinBox, 3, 1); // number of repeats input
    addAlarmLayout->addWidget(addEventIntervalLabel, 4, 0);
    addAlarmLayout->addWidget(addEventIntervalSpinBox, 4, 1); // length of intervals input (days)
    addAlarmLayout->addWidget(addEventIntervalTimeEdit, 4, 2); // length of intervals input (hh:mm:ss)
    addAlarmLayout->addWidget(addAlarmConfirmationPushButton, 5, 0, 5, 3);
    addAlarmGroupBox->setLayout(addAlarmLayout);

    connect(addAlarmConfirmationPushButton, SIGNAL(clicked(bool)),
            this, SLOT(addAlarmConfirmationPushButtonClicked()));
}

void Window::createViewAlarmGroupBox()
{
    viewAlarmGroupBox = new QGroupBox(tr("Selected Day's Alarms"));
    viewEventAlarmNameLabel = new QLabel(tr("Alarm Name:"));
    viewEventAlarmTimeLabel = new QLabel(tr("Alarm Time:"));
    viewEventSoundLabel = new QLabel(tr("Alarm Sound:"));
    viewEventNumberOfRepeatsLabel = new QLabel(tr("Number of Repeats:"));
    viewEventIntervalLabel = new QLabel(tr("Interval of Repeats:"));

    Window::displayViewAlarmLabels();

    viewNextAlarmPushButton = new QPushButton;
    viewNextAlarmPushButton->setText("Next Alarm Today");
    viewNextAlarmPushButton->setToolTip("Display Next Alarm Today");
    viewPreviousAlarmPushButton = new QPushButton;
    viewPreviousAlarmPushButton->setText("Previous Alarm Today");
    viewPreviousAlarmPushButton->setToolTip("Display Alarm Event Today");

    connect(viewNextAlarmPushButton, SIGNAL(clicked(bool)),
            this, SLOT(viewNextAlarmPushButtonClicked()));

    connect(viewPreviousAlarmPushButton,SIGNAL(clicked(bool)),
            this, SLOT(viewPreviousAlarmPushButtonClicked()));

    QGridLayout *viewAlarmLayout = new QGridLayout;
    viewAlarmLayout->addWidget(viewEventAlarmNameLabel, 0, 0);
    viewAlarmLayout->addWidget(displayEventAlarmNameLabel, 0, 1); // alarm name display
    viewAlarmLayout->addWidget(viewEventAlarmTimeLabel, 1, 0);
    viewAlarmLayout->addWidget(displayEventAlarmTimeLabel, 1 , 1); // alarm time display
    viewAlarmLayout->addWidget(viewEventSoundLabel, 2, 0);
    viewAlarmLayout->addWidget(displayEventSoundLabel, 2, 1); // sound display
    viewAlarmLayout->addWidget(viewEventNumberOfRepeatsLabel, 3, 0);
    viewAlarmLayout->addWidget(displayEventNumberOfRepeatsLabel, 3, 1); // repeats display
    viewAlarmLayout->addWidget(viewEventIntervalLabel, 4, 0);
    viewAlarmLayout->addWidget(displayEventIntervalLabel, 4, 1); // interval display
    viewAlarmLayout->addWidget(viewPreviousAlarmPushButton, 5, 0);
    viewAlarmLayout->addWidget(viewNextAlarmPushButton, 5, 1);
    viewAlarmGroupBox->setLayout(viewAlarmLayout);
}

void Window::createStopwatchGroupBox()
{
    viewStopwatchGroupBox = new QGroupBox(tr("Create Stopwatch"));
    stopwatchRingInLabel = new QLabel(tr("Ring In:"));
    stopwatchRepeatLabel = new QLabel(tr("Times to Repeat:"));
    stopwatchEveryLabel = new QLabel(tr("Interval Between Repeats:"));

    stopwatchRingInSpinBox = new QSpinBox;
        stopwatchRingInSpinBox->setRange(0, 999);
        stopwatchRingInSpinBox->setSingleStep(1);
        stopwatchRingInSpinBox->setValue(0);
        stopwatchRingInSpinBox->setToolTip("Minutes");

    stopwatchRepeatSpinBox = new QSpinBox;
        stopwatchRepeatSpinBox->setRange(0, 999);
        stopwatchRepeatSpinBox->setSingleStep(1);
        stopwatchRepeatSpinBox->setValue(0);

    stopwatchEverySpinBox = new QSpinBox;
        stopwatchEverySpinBox->setRange(0, 999);
        stopwatchEverySpinBox->setSingleStep(1);
        stopwatchEverySpinBox->setValue(0);
        stopwatchEverySpinBox->setToolTip("Minutes");

    stopwatchConfirmationPushButton = new QPushButton;
        stopwatchConfirmationPushButton->setText("Create Stopwatch");
        stopwatchConfirmationPushButton->setToolTip("Add Stopwatch to Calendar");

     QGridLayout *stopwatchLayout = new QGridLayout;
     stopwatchLayout->addWidget(stopwatchRingInLabel, 0, 0);
     stopwatchLayout->addWidget(stopwatchRingInSpinBox, 0, 1);
     stopwatchLayout->addWidget(stopwatchRepeatLabel, 1, 0);
     stopwatchLayout->addWidget(stopwatchRepeatSpinBox, 1, 1);
     stopwatchLayout->addWidget(stopwatchEveryLabel, 2, 0);
     stopwatchLayout->addWidget(stopwatchEverySpinBox, 2, 1);
     stopwatchLayout->addWidget(stopwatchConfirmationPushButton, 3, 0, 3, 2);
     viewStopwatchGroupBox->setLayout(stopwatchLayout);

     connect(stopwatchConfirmationPushButton, SIGNAL(clicked(bool)),
             this, SLOT(stopwatchConfirmationPushButtonClicked()));
}
