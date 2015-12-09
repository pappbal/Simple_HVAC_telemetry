#ifndef GUI_H
#define GUI_H

#include <application.h>
#include <statehistory.h>
#include <QQmlProperty>
#include <QObject>
#include <QQuickItem>
#include <QTimer>
#include <QString>
#include <QQmlContext>
#include <QList>
/*! \mainpage Overview
 *
 * \section Introduction
 * This is the introduction.
 * \image html image_full.png
 * 
 *
 * \section HVAC
 * This is the section of the embedded HVAC node
 * 
 * \section Communication
 * This is the overview of the Communication mechanism
 * \image html image_communication.png
 *
 *
 
 * \section Representation
 * This is the overview of the robot representation
 * The actual state of the robot is contained in the State class.
 * The state contains Temperatures, Speeds and Actuators classes for the values sent by the node in the currentState.
 * Whenever a State is full (all values are collected) or deficient (for some reason the node have not sent any of them)
 * it is appended to its container, the StateHistory. When a new State is appended the Proxy signals the GUI via newData().
 * 
 * On the sending side, the Proxy provides one funcion - sendCommand(ID,data) to send an integer data to the node with predefined IDs.
 * For details on the communication mechanism, refer to the previous sections.
 *
 * The Proxy is also responsible for handling connection and disconnection. Whenever it is set to be disconnected, it discards any data
 * coming from the node, and also any command coming from the GUI (except for Connect command).
 * The Proxy also signals disconnection to the GUI via signalDisconnected. It has a 5 sec timeout counter, if no data is incoming, it disconnects
 * and sends the signal. (If the node is stopped, it is not handled as disconnection, since the node is sending NodeStopped messages.)
 *
 * The class diagram of the node representation is as follows:
 * \image html image_robot_representation.png
 *
 *
 *
 
 * \section Sequences
 * TODO: Sequence diagrams!!!!!!!
 * etc...
 */
 
class GUITester : public QObject
{
    Q_OBJECT
public:
    GUITester(StateHistory &stateHistory);

    void Start(float intervalSec);
    void Stop();

private:
    /** Időzítő a tick() metódus periodikus hívására. */
    QTimer timer;

    StateHistory &stateHistory;

    Temperatures temps;
    Speeds speeds;
    Actuators acts;

private slots:
    /** A timer hívja meg, meghatározza a robot
     * állapotát a következő időpillanatban. */
    void tick();

signals:
    void sendMessage(QString message);
    void stateHistoryUpdateSimulatorSignal();
};



class GUI : public QObject
{
    Q_OBJECT
private:
    const StateHistory& stateHistory;

    QQuickItem* findItemByName(const QString& name);
    QQuickItem* findItemByName(QObject *rootObject, const QString& name);
    QQuickItem* findItemByName(QList<QObject*> nodes, const QString& name);
    QQuickItem* mainWindowObject;

    QQmlContext &qmlContext;

    //GUITester tester;

    const unsigned showStateNumber = 40;
    QList<double> graphTemperatures1;
    QList<double> graphTemperatures2;
    QList<double> graphTemperatures3;
    QList<double> graphTemperatures4;

    QList<int> graphSpeeds1;
    QList<int> graphSpeeds2;

    int getValueFromQML(const QString& itemName, const char *invokeMethodName);

public:
    GUI(QObject *rootObject, QQmlContext& qmlContext, StateHistory& stateHistory);

    void ConnectQmlSignals(QObject *rootObject);

    void sendSignal(qint8 pid_ID, qint32 data);

signals:
    void signalCommand(qint8 pid_ID, qint32 data);
    void historyContextUpdated();

public slots:
    // From StateHistory
    void plotData();
    void stateHistoryUpdated();

    // From Proxy
    void stoppedSlot();
    void disconnectedSlot();

    // From QML
    void receiveConnectSignal();
    void receiveDisconnectSignal();
    void receiveStartSignal();
    void receiveStopSignal();
    void receiveSetPSignal();
    void receiveSetISignal();
    void receiveSetDSignal();
    void receiveSetTemp1Signal();
    void receiveSetTemp2Signal();
    void receiveSetTemp3Signal();
    void receiveSetTemp4Signal();

    // From GUITester
    void testMessage(QString message);


};

#endif // GUI

