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
 *
 * This is a diagnostic application for the embedded HVAC node. Requirements for this application are the follows: show
 * measured temperature values, measured fan speed values and actuator values for diagnostic purposes, and let the user define the controller parameters
 * (P, I, D) and temperature setpoints, as well as start and stop the temperature controlling.
 *
 * Video about the working system (in hungarian): https://drive.google.com/open?id=0BzYGHIbT62_UdGRQUUR6dnBtRTQ
 *
 * \section GUI
 * This is the overview of the GUI.
 *
 * All current values (temperatures, speeds, actuators) are visible on the user interface in text format. In case of current temperature values
 * a column diagram is shown as well to make it more expressive. Previous values of temperatures, speeds and actuators displayed on three
 * history graphs: one for the temperatures, one for the speeds, and one for the actuator values. Display can be switched on / switched off for
 * each value by checking / unchecking radio buttons. The user interface also informs the user about the state of the connection to the device
 * (which can be "Connected" or "Unconnected") and about the state of the device itself (which can be "Running" or "Stopped").
 *
 * File menu serves to connect or unconnect the device. Start and Stop buttons placed on the graphical user interface to start and stop the teperature controlling.
 * Textfields are provided to set P, I, D parameters and temperature setpoints. The new values will be sent to the device only if the corresponding Set button is pushed.
 *
 * \image html image_full.png
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

 * \section Communication
 * This is the overview of the Communication mechanism.
 *
 * The communcation happens through a virtual COM port which is handled as a normal serial port, the OS does the translation between USB and serial protocols.
 * When a full message received, the communication modul sends a signal to the proxy indication a new message is waiting for being parsed.
 * When a package is ready to be sent the proxy passes a package to the communication module which then composes the frames from the packet and sends them out to the HVAC.
 *
 * \image html image_communication.png
 *

 * \section HVAC
 * Heating Ventillation Air Conditioning system
 * This is the overview of the embedded software and hardware.
 *
 * The embedded system is controlled by an STM32F4 Discovery board. 4 fans can be connected to the controller through an H-bridge, each of them can be controlled individually with PWM signals.
 * 4 temperature sensors can be used, each of them belongs to a fan, communicate through SPI with the microprocessor. One PID controller is implemented which controls all fans based on their thermometer seperatelly,
 * but with the same PID parameters, thus 4 different setpoints can be applied on the system at the same time.
 * The system communicates through virtual com port with the PC. The messages are the follows with the appropiate header ID:
 *
 * NAME                             ID
 * Thermometer 1 measured value     1
 * Thermometer 2 measured value     2
 * Thermometer 3 measured value     3
 * Thermometer 4 measured value     4
 * Fan 1 speed                      5
 * Fan 2 speed                      6
 * Actuator 1                       7
 * Actuator 2                       8
 * P parameter                      9
 * I parameter                      10
 * D parameter                      11
 * Requested temperature 1          12
 * Requested temperature 2          13
 * Requested temperature 3          14
 * Requested temperature 4          15
 * START                            16
 * STOP                             17
 * HVAC is in stopped state         21
 *
 *It is seen that the embedded system sends out temperature values, fans's actual frequencies, the PID's actuator values and a signal indicating the HVAC is in stopped state.
 *The system receives START/STOP messages, the P,I,D and setpoints values.
 *
 *Currently 2 fans are installed in the system, that is why only 2 fan speed and 2 actuator message are defined.
 *

 * \section Sequences
 * The following fugire shows the receive mechanism
 * \image html image_receive.png
 * The following figure shows the send mechanism
 * \image html image_send.png
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

/**
 * @brief Represents a link between QML and other parts of the program.
 */

class GUI : public QObject
{
    Q_OBJECT
private:
    /** Constant reference to a StateHistory object */
    const StateHistory& stateHistory;

    /** @brief Finds a QML item in a recursive way
     *  @param name Item's objectName (QML property)
     *  @return Pointer to the item, nullptr if not found
     */
    QQuickItem* findItemByName(const QString& name);

    /** @brief Finds a QML item in a recursive way
     *  @param rootObject Starting element of the search
     *  @param name Item's objectName (QML property)
     *  @return Pointer to the item, nullptr if not found
     */
    QQuickItem* findItemByName(QObject *rootObject, const QString& name);

    /** @brief Finds a QML item in a recursive way
     *
     *  Used by FindItemByName(QObject *rootObject, const QString& name)
     *
     *  @param nodes List of nodes in which the search will be done
     *  @param name Item's objectName (QML property)
     *  @param rootObject Starting element of the searching
     *  @return Pointer to the item, nullptr if not found
     */
    QQuickItem* findItemByName(QList<QObject*> nodes, const QString& name);

    /** Main window object */
    QQuickItem* mainWindowObject;

    /** QML context object */
    QQmlContext &qmlContext;

    // TODO: remove
    GUITester tester;

    /** Number of shown values on history graphs
     *  Last showStateNumber number of values (temperatures, speeds and actuators) will be send to the history graphs to show them
     */
    const unsigned showStateNumber = 80;

    /** Vector for first temperature sensor's last showStateNumber number of values */
    QList<double> graphTemperatures1;

    /** Vector for second temperature sensor's last showStateNumber number of values */
    QList<double> graphTemperatures2;

    /** Vector for third temperature sensor's last showStateNumber number of values */
    QList<double> graphTemperatures3;

    /** Vector for fourth temperature sensor's last showStateNumber number of values */
    QList<double> graphTemperatures4;

    /** Vector for first fan's last showStateNumber number of speed values */
    QList<int> graphSpeeds1;

    /** Vector for second fan's last showStateNumber number of speed values */
    QList<int> graphSpeeds2;

    /** Vector for first fan's last showStateNumber number of actuator values */
    QList<int> graphActuators1;

    /** Vector for second fan's last showStateNumber number of actuator values */
    QList<int> graphActuators2;

    /** @brief Gets an integer value stored in a QML control
     *
     *  Used for the query of user defined values (temperature setpoints and controller parameters).
     *
     * @param itemName QML control's objectName property ( "temperatureSettings" in case of temperature setpoint values and "setPID" in case of controller parameter values)
     * @param invokeMethodName Name of control itemName's function which returns the needed value
     * @return Integer value stored in the given QML control
     */
    int getValueFromQML(const QString& itemName, const char *invokeMethodName);

public:
    /** @brief Constructor
     *  @param rootObject Root object
     *  @param qmlContext QML context object
     *  @param stateHistory StateHistory object
     */
    GUI(QObject *rootObject, QQmlContext& qmlContext, StateHistory& stateHistory);

    /** @brief Connects signal historyContextUpdated to QML Canvas items' onPaint functions
     *  @param rootObject Root object
     *  @see historyContextUpdated()
     */
    void ConnectQmlSignals(QObject *rootObject);

signals:
    /** @brief Sends a user command which received from QML
     *
     *  @param pid_ID Command ID (see application.h for the proper values)
     *  @param data Command parameter
     *
     *  @see receiveConnectSignal()
     *  @see receiveDisconnectSignal()
     *  @see receiveStartSignal()
     *  @see receiveStopSignal()
     *  @see receiveSetPSignal()
     *  @see receiveSetISignal()
     *  @see receiveSetDSignal()
     *  @see receiveSetTemp1Signal()
     *  @see receiveSetTemp2Signal()
     *  @see receiveSetTemp3Signal()
     *  @see receiveSetTemp4Signal()
     *
     *  @note Proxy must be connected to this signal to get notified about user inputs
     */
    void signalCommand(qint8 pid_ID, qint32 data);

    /** @brief Signal towards QML Canvas items to draw themselves
     *
     *  After this signal the history graphs and column diagram will be refreshed on the user interface.
     *  Connected to QML side by method ConnectQmlSignals.
     *  Emitted by method stateHistoryUpdated.
     *
     *  @see ConnectQmlSignals()
     *  @see stateHistoryUpdated()
     */
    void historyContextUpdated();

public slots:
    // From StateHistory
    /** @brief Prints the current state onto the standard output
     *
     *  Current state asked from stateHistory.
     *
     *  @see StateHistory
     */
    void plotData();

    /** @brief Sends data to the QML side
     *
     *  Data includes current state's measured values and showStateNumber number of previous states' measured values.
     *  Current and previous states asked from stateHistory.
     *  Emits signal historyContextUpdated.
     *  Sets the state of device to "Running" and the state of connection to "Connected" on the user interface.
     *
     *  @see showStateNumber
     *  @see StateHistory
     *  @see historyContextUpdated()
     */
    void stateHistoryUpdated();

    // From Proxy
    /** Sets the state to "Stopped" on the user interface */
    void stoppedSlot();

    /** Sets the state of connection to "Disconnected" and the state of device to "Stopped" on the user interface */
    void disconnectedSlot();

    /** Sets the state of connection to "Connected" on the user interface */
    void connectedSlot();

    // From QML
    /** @brief Emits signalCommand with command ID ID_connect.
     *
     *  Called by the effect of a QML signal which is emitted when Connect selected in the File menu by the user.
     *
     *  @see signalCommand()
     */
    void receiveConnectSignal();

    /** @brief Emits signalCommand with command ID ID_disconnect.
     *
     *  Called by the effect of a QML signal which is emitted when Disconnect selected in the File menu by the user.
     *
     *  @see signalCommand()
     */
    void receiveDisconnectSignal();

    /** @brief Emits signalCommand with command ID ID_start.
     *
     *  Called by the effect of a QML signal which is emitted when Start button is pushed by the user.
     *
     *  @see signalCommand()
     */
    void receiveStartSignal();

    /** @brief Emits signalCommand with command ID ID_stop.
     *
     *  Called by the effect of a QML signal which is emitted when Stop button is pushed by the user.
     *
     *  @see signalCommand()
     */
    void receiveStopSignal();

    /** @brief Gets the value of parameter P from QML and emits signalCommand with command ID ID_pid_p and with new value of P.
     *
     *  Called by the effect of a QML signal which is emitted when Set button for controller parameter P is pushed by the user.
     *
     *  @see getValueFromQML()
     *  @see signalCommand()
     */
    void receiveSetPSignal();

    /** @brief Gets the value of parameter I from QML and emits signalCommand with command ID ID_pid_o and with new value of I.
     *
     *  Called by the effect of a QML signal which is emitted when Set button for controller parameter I is pushed by the user.
     *
     *  @see getValueFromQML()
     *  @see signalCommand()
     */
    void receiveSetISignal();

    /** @brief Gets the value of parameter D from QML and emits signalCommand with command ID ID_pid_d and with new value of D.
     *
     *  Called by the effect of a QML signal which is emitted when Set button for controller parameter D is pushed by the user.
     *
     *  @see getValueFromQML()
     *  @see signalCommand()
     */
    void receiveSetDSignal();

    /** @brief Gets the setpoint for first temperature from QML and emits signalCommand with command ID ID_req_temp_1 and with new value of setpoint.
     *
     *  Called by the effect of a QML signal which is emitted when Set button for first temperature's setpoint is pushed by the user.
     *
     *  @see getValueFromQML()
     *  @see signalCommand()
     */
    void receiveSetTemp1Signal();

    /** @brief Gets the setpoint for second temperature from QML and emits signalCommand with command ID ID_req_temp_1 and with new value of setpoint.
     *
     *  Called by the effect of a QML signal which is emitted when Set button for second temperature's setpoint is pushed by the user.
     *
     *  @see getValueFromQML()
     *  @see signalCommand()
     */
    void receiveSetTemp2Signal();

    /** @brief Gets the setpoint for third temperature from QML and emits signalCommand with command ID ID_req_temp_1 and with new value of setpoint.
     *
     *  Called by the effect of a QML signal which is emitted when Set button for third temperature's setpoint is pushed by the user.
     *
     *  @see getValueFromQML()
     *  @see signalCommand()
     */
    void receiveSetTemp3Signal();

    /** @brief Gets the setpoint for fourth temperature from QML and emits signalCommand with command ID ID_req_temp_1 and with new value of setpoint.
     *
     *  Called by the effect of a QML signal which is emitted when Set button for fourth temperature's setpoint is pushed by the user.
     *
     *  @see getValueFromQML()
     *  @see signalCommand()
     */
    void receiveSetTemp4Signal();

    // From GUITester
    // TODO: remove
    void testMessage(QString message);
};

#endif // GUI

