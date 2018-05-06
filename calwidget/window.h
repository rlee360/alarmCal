#ifndef WINDOW_H
#define WINDOW_H

#include <QtWidgets>
#include <QString>
#include <QWindow>
#include <QSpinBox>

class QCalendarWidget;
class QCheckBox;
class QComboBox;
class QDate;
class QDateEdit;
class QGridLayout;
class QGroupBox;
class QLabel;
class QTimeEdit;
class QDateTimeEdit;
class QTextEdit;
class QPushButton;
class QLineEdit;
class QSpinBox;
class QString;

class Window : public QWidget
{
    Q_OBJECT


public:
    Window();
    void changeDisplayEventNameLabel(QString &q);


private slots:
    void localeChanged(int index);
    void firstDayChanged(int index);
    void selectionModeChanged(int index);
    void horizontalHeaderChanged(int index);
    void verticalHeaderChanged(int index);
    void selectedDateChanged();
    void minimumDateChanged(const QDate &date);
    void maximumDateChanged(const QDate &date);
    void weekdayFormatChanged();
    void weekendFormatChanged();
    void reformatHeaders();
    void reformatCalendarPage();
    void setEventEndDateEditMinimum();
    void setEventEndTimeEditMinimum();
    void viewEventsAndAlarmsSelectedDateChanged();
    void addEventConfirmationPushButtonClicked();
    void addAlarmConfirmationPushButtonClicked();
    void stopwatchConfirmationPushButtonClicked();
    void viewNextEventPushButtonClicked();
    void viewPreviousEventPushButtonClicked();
    void viewNextAlarmPushButtonClicked();
    void viewPreviousAlarmPushButtonClicked();


private:
    void createPreviewGroupBox();
    void createGeneralOptionsGroupBox();
    void createDatesGroupBox();
    void createTextFormatsGroupBox();
    void createAddEventGroupBox();
    void createViewEventGroupBox();
    void createAddAlarmGroupBox();
    void createViewAlarmGroupBox();
    void createStopwatchGroupBox();
    void displayViewEventLabels();
    void displayViewAlarmLabels();
    void viewEvents();
    void viewAlarms();
    void closeEvent(QCloseEvent *event);


    //Calendar preview section beginning
    QGroupBox *previewGroupBox;
    QGridLayout *previewLayout;
    QCalendarWidget *calendar;
    //Calendar preview section end


    //General options section beginning
    QGroupBox *generalOptionsGroupBox;
    QLabel *localeLabel;
    QLabel *firstDayLabel;
    QLabel *selectionModeLabel;
    QLabel *horizontalHeaderLabel;
    QLabel *verticalHeaderLabel;
    QComboBox *localeCombo;
    QComboBox *firstDayCombo;
    QComboBox *selectionModeCombo;
    QCheckBox *gridCheckBox;
    QCheckBox *navigationCheckBox;
    QComboBox *horizontalHeaderCombo;
    QComboBox *verticalHeaderCombo;
    QComboBox *createColorComboBox();
    //General options section end


    //Dates section beginning
    QGroupBox *datesGroupBox;
    QLabel *currentDateLabel;
    QLabel *minimumDateLabel;
    QLabel *maximumDateLabel;
    QDateEdit *currentDateEdit;
    QDateEdit *minimumDateEdit;
    QDateEdit *maximumDateEdit;
    //Dates section end


    //Text formats section beginning
    QGroupBox *textFormatsGroupBox;
    QLabel *weekdayColorLabel;
    QLabel *weekendColorLabel;
    QLabel *headerTextFormatLabel;
    QComboBox *weekdayColorCombo;
    QComboBox *weekendColorCombo;
    QComboBox *headerTextFormatCombo;
    QCheckBox *FridayCheckBox;
    QCheckBox *FirstCheckBox;
    //Text formats section end


    //View event AND alarm section beginning
    //Will view and display everything as QLabels
    //Parameters will be converted into QStrings with functions to display different data types
    QGroupBox *viewEventGroupBox;
    QGroupBox *viewAlarmGroupBox;
    QLabel *viewEventNameLabel;
        QLabel *displayEventNameLabel; //Display
    QLabel *viewEventStartDateTimeLabel;
        QLabel *displayEventStartDateTimeLabel; //Display
    QLabel *viewEventEndDateTimeLabel;
        QLabel *displayEventEndDateTimeLabel; //Display
    QLabel *viewEventDescriptionLabel;
        QLabel *displayEventDescriptionLabel; //Display
    QLabel *viewEventReminderLabel;
        QLabel *displayEventReminderLabel; //Display
    QLabel *viewEventColorLabel;
        QLabel *displayEventColorLabel; //Display
    QLabel *viewEventAlarmNameLabel;
        QLabel *displayEventAlarmNameLabel; //Display
    QLabel *viewEventAlarmTimeLabel;
        QLabel *displayEventAlarmTimeLabel; //Display
    QLabel *viewEventNumberOfRepeatsLabel;
        QLabel *displayEventNumberOfRepeatsLabel; //Display
    QLabel *viewEventIntervalLabel;
        QLabel *displayEventIntervalLabel; //Display
    QLabel *viewEventSoundLabel;
        QLabel *displayEventSoundLabel; //Display
    QPushButton *viewNextEventPushButton;
    QPushButton *viewPreviousEventPushButton;
    QPushButton *viewNextAlarmPushButton;
    QPushButton *viewPreviousAlarmPushButton;
    //View event AND alarm section end


    //Add event AND alarm section beginning
    QGroupBox *addEventGroupBox;
    QGroupBox *addAlarmGroupBox;
    QLabel *addEventNameLabel;
        QLineEdit *addEventNameLineEdit;
    QLabel *addEventStartDateTimeLabel;
        QDateEdit *addEventStartDateEdit;
        QTimeEdit *addEventStartTimeEdit;
    QLabel *addEventEndDateTimeLabel;
        QDateEdit *addEventEndDateEdit;
        QTimeEdit *addEventEndTimeEdit;
    QLabel *addEventDescriptionLabel;
        QLineEdit *addEventDescriptionLineEdit;
    QLabel *addEventReminderLabel;
        QDateEdit *addEventReminderDateEdit;
        QTimeEdit *addEventReminderTimeEdit;
    QLabel *addEventColorLabel;
        QComboBox *addEventColorCombo;
    QLabel *addEventAlarmNameLabel;
        QLineEdit *addEventAlarmNameLineEdit;
    QLabel *addEventAlarmTimeLabel;
        QDateEdit *addEventAlarmDateEdit;
        QTimeEdit *addEventAlarmTimeEdit;
    QLabel *addEventNumberOfRepeatsLabel;
        QSpinBox *addEventNumberOfRepeatsSpinBox;
    QLabel *addEventIntervalLabel;
        QTimeEdit *addEventIntervalTimeEdit;
        QSpinBox *addEventIntervalSpinBox;
    QLabel *addEventSoundLabel;
        QComboBox *addEventSoundComboBox;
    QPushButton *addEventConfirmationPushButton;
    QPushButton *addAlarmConfirmationPushButton;
    //Save all the event properties into strings when button is pushed
    //Add event AND alarm section end


    //Stopwatch section beginning
    QGroupBox *viewStopwatchGroupBox;
    QLabel *stopwatchRingInLabel;
        QSpinBox *stopwatchRingInSpinBox;
    QLabel *stopwatchRepeatLabel;
        QSpinBox *stopwatchRepeatSpinBox;
    QLabel *stopwatchEveryLabel;
        QSpinBox *stopwatchEverySpinBox;
    QPushButton *stopwatchConfirmationPushButton;
    //Stopwatch section end


    //Setting default Date so it is more practical to input a Date
    QTime currentTime = QTime::currentTime();
    QDate currentDate = QDate::currentDate();


    //Temporary variables for passing data into functions below
    //Temporary variables for events
    std::string tempEventName;
    std::string tempEventDescription;
    std::string tempEventColor;
    int tempEventStartDateYear;
    int tempEventStartDateMonth;
    int tempEventStartDateDay;
    int tempEventStartTimeHour;
    int tempEventStartTimeMinute;
    int tempEventStartTimeSecond;
    int tempEventEndDateYear;
    int tempEventEndDateMonth;
    int tempEventEndDateDay;
    int tempEventEndTimeHour;
    int tempEventEndTimeMinute;
    int tempEventEndTimeSecond;
    int tempEventReminderDateYear;
    int tempEventReminderDateMonth;
    int tempEventReminderDateDay;
    int tempEventReminderTimeHour;
    int tempEventReminderTimeMinute;
    int tempEventReminderTimeSecond;


    //Temporary variables for alarms
    std::string tempAlarmName;
    std::string tempAlarmSound;
    std::string tempAlarmSound2;
    int tempAlarmStartDateYear;
    int tempAlarmStartDateMonth;
    int tempAlarmStartDateDay;
    int tempAlarmStartTimeHour;
    int tempAlarmStartTimeMinute;
    int tempAlarmStartTimeSecond;
    int tempAlarmNumerOfRepeats;
    int tempIntervalBetweenRepeats;
    int tempIntervalHours;
    int tempIntervalMinutes;
    int tempIntervalSeconds;


    //Temporary variables for stopwatches
    int tempStopwatchRingIn;
    int tempStopwatchRepeat;
    int tempEvery;


    //Temporary variables for converting
    QString tempQStr1;
    QString tempQStr2;
    QString tempQStr3;
    QString tempQStr4;
    QString tempQStr5;
    QString tempQStr6;
    QString tempQStr7;
    QString tempQStr8;
    QString tempQStr9;
    QString tempQStr10;
    QString tempQStr11;
    QString tempQStr12;
    QString tempQStr13;
    QString tempQStr14;
    QString tempQStr15;
    QString tempQStr16;
    QString tempQStr17;
    QString tempQStr18;
    QString tempQStr19;
    QString tempQStr20;
    QString tempQStr21;
    QString tempQStr22;
    QString tempQStr23;
    QString tempQStr24;
    QString tempQStr25;
};

#endif
