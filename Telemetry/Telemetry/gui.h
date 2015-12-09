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

/**
 * @brief Ez az osztály teremt kapcsolatot a QML-ben megírt grafikus felhasználó felület és a program többi része között.
 *
 * Konstruktorában referenciát kap az eszköz jelenlegi és múltbéli állapotait leíró StateHistory típusú objektumra.
 * Amennyiben frissül az eszköz állapota, a referált StateHistory objektum emittál egy jelzést (newData()),
 * amelyre a GUI osztály konstruktora feliratkoztatja a konstruált GUI objektumot (stateHistoryUpdated()).
 * A jelzés elkapásával értesül a GUI arról, hogy a felhasználói felületen kijelzett adatokat (mért értkek, állapot) frissíteni kell.
 * A legfrissebb állapot értékei és a korábbi állapotok értékei is lekérhetőek a StateHistory-ra vonatkozó referencián keresztül.
 *
 * Az eszközt reprezentáló Proxy osztály felé egy signal és két slot jelenti az interfészt.
 * A signalCommand() szignál küldi el a felhasználótól érkező parancs kódját és paraméterét, erre a jelzésre a Proxy feliratkozik.
 * A stoppedSlot() slot-on keresztül tudja jelzni a Proxy, hogy az eszközön nem fut a szabályozás. A felhasználói felületen ennek megflelően frissól az állapot.
 * A disconnectedSlot()-on keresztül tudja jelezni a Proxy, hogy nem kapcsolódik az eszközhöz. A felhasználói felületen ennek megflelően frissól az állapot.
 *
 * Ezen kívül egy szignál és számos slot szolgál a QML oldallal való kommunikációra.
 */

class GUI : public QObject
{
    Q_OBJECT
private:
    /** Konstans referencia az eszköz jelenlegi és korábbi állapotait tartalmazó StateHistory objektumra. */
    const StateHistory& stateHistory;

    /** @brief Segédfüggvény QML elemek rekurzív megkeresésére.
     *  @param name Az objectName (QML elemek tulajdonsága), amit keresünk.
     */
    QQuickItem* findItemByName(const QString& name);

    /** @brief Segédfüggvény QML elemek rekurzív megkeresésére.
     *  @param rootObject A QML gyökérelem, amivel a keresést kezdjük.
     *  @param name Az objectName (QML elemek tulajdonsága), amit keresünk.
     */
    QQuickItem* findItemByName(QObject *rootObject, const QString& name);

    /** @brief Segédfüggvény QML elemek rekurzív megkeresésére.
     *  A FindItemByName(QObject *rootObject, const QString& name) használja.
     *  @param nodes Azon node-ok listája, melyekben (rekurzívan) az adott nevű elemet keressük.
     *  @param name  Az objectName (QML elemek tulajdonsága), amit keresünk.
     */
    QQuickItem* findItemByName(QList<QObject*> nodes, const QString& name);

    /** Főablak objektum */
    QQuickItem* mainWindowObject;

    /** QML környezet */
    QQmlContext &qmlContext;

    // TODO: remove
    GUITester tester;

    /** Az időgrafikonokon kijelzett értékek száma.
     *  Ennyi darab visszamenőleges érték kerül elküldésre a QML-nek az idődiagramok megrajzolásához */
    const unsigned showStateNumber = 80;

    /** Az 1. hőmérő elküldendő múltbéli értékeit tartalmazó vektor. */
    QList<double> graphTemperatures1;

    /** A 2. hőmérő elküldendő múltbéli értékeit tartalmazó vektor. */
    QList<double> graphTemperatures2;

    /** A 3. hőmérő elküldendő múltbéli értékeit tartalmazó vektor. */
    QList<double> graphTemperatures3;

    /** A 4. hőmérő elküldendő múltbéli értékeit tartalmazó vektor. */
    QList<double> graphTemperatures4;

    /** Az 1. ventillátor-sebesség elküldendő múltbéli értékeit tartalmazó vektor. */
    QList<int> graphSpeeds1;

    /** A 2. ventillátor-sebesség elküldendő múltbéli értékeit tartalmazó vektor. */
    QList<int> graphSpeeds2;

    /** Az 1. beavatkozó elküldendő múltbéli értékeit tartalmazó vektor. */
    QList<int> graphActuators1;

    /** A 2. beavatkozó elküldendő múltbéli értékeit tartalmazó vektor. */
    QList<int> graphActuators2;

    /** @brief QML vezérlőben tárolt adat lekérdezésére.
     *
     *  A felhasználó által beállított célhőmérsékletek és szabályozó paraméterek (PID) lekérdezésére szolgál.
     *
     * @param itemName A QML vezérlő objectName tulajdonsága (hőmérsékletekhez "temperatureSettings", PID paraméterekhez "setPID")
     * @param invokeMethodName Az itemName vezérlő meghívni kívánt függvénye, amely visszaadja a kért adatot
     * @return A lekérdezett adat
     */
    int getValueFromQML(const QString& itemName, const char *invokeMethodName);

public:
    /** @brief Konstruktor
     *  @param rootObject Gyökérobjektum
     *  @param qmlContext QML környezet objektuma
     *  @param stateHistory Az eszköz jelenlegi és korábbi állapotait tároló objektum
     */
    GUI(QObject *rootObject, QQmlContext& qmlContext, StateHistory& stateHistory);

    /** @brief Beregisztrálja a historyContextUpdated szignált a QML Canvas elemeihez
     *  @param rootObject Gyökérobjektum
     *  @see historyContextUpdated()
     */
    void ConnectQmlSignals(QObject *rootObject);

signals:
    /** @brief A felhasználótól érkezett parancs jelzése a többi programrész felé.
     *
     *  @param pid_ID Parancs azonosító (lehetséges értékeihez ld. application.h konstansait)
     *  @param data A parancshoz tartozó érték (pl. beállított hőmérsékletérték)
     *
     *  @see stateHistoryUpdated()
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
     */
    void signalCommand(qint8 pid_ID, qint32 data);

    /** Jelzés a QML Canvas vezérlőinek, hogy rajzolják újra magukat.
     *
     *  Hatására frissülnek a hőmérsékletek, ventillátor-sebességek és beavatkozók idődiagramjai,
     *  valamint a hőmérsékletek oszlopdiagramja.
     *  Az esemény beregisztrálását a ConnectQmlSignals metódus végzi el.
     *  Ezt a jelzést stateHistoryUpdated() metódus váltja ki.
     *
     *  @see ConnectQmlSignals()
     *  @see stateHistoryUpdated()
     */
    void historyContextUpdated();

public slots:
    // From StateHistory
    /** Lekérdezi a statHistory-tól az eszköz aktuális állapotát, és kiírja a standard outputra. */
    void plotData();

    /** Lekérdezi a statHistory-tól az eszköz aktuális és korábbi állapotait, majd frissíti a felhasználói felületet. */
    void stateHistoryUpdated();

    // From Proxy
    /** Az eszköz állapotát "Stopped" állapotra váltja a felhasználói felületen */
    void stoppedSlot();

    /** Az eszköz kapcsolódási állapotát "Disconnected" állapotra váltja a felhasználói felületen. */
    void disconnectedSlot();

    // From QML
    /** @brief Kapcsolódás kezdeményezése.
     *
     *  QML esemény hatására hívódik meg.
     *  Emittálja a signalCommand jelzést.
     *
     *  @see signalCommand()
     */
    void receiveConnectSignal();

    /** @brief Kapcsolatbontás kezdeményezése.
     *
     *  QML esemény hatására hívódik meg.
     *  Emittálja a signalCommand jelzést.
     *
     *  @see signalCommand()
     */
    void receiveDisconnectSignal();

    /** @brief Szabályozás elindításának kezdeményezése.
     *
     *  QML esemény hatására hívódik meg.
     *  Emittálja a signalCommand jelzést.
     *
     *  @see signalCommand()
     */
    void receiveStartSignal();

    /** @brief Szabályozás leállításának kezdeményezése.
     *
     *  QML esemény hatására hívódik meg.
     *  Emittálja a signalCommand jelzést.
     *
     *  @see signalCommand()
     */
    void receiveStopSignal();

    /** @brief P paraméter beállítása.
     *
     *  QML esemény hatására hívódik meg.
     *  Lekérdezi a beállított értéket a getValueFromQML metódussal.
     *  Emittálja a signalCommand jelzést.
     *
     *  @see getValueFromQML()
     *  @see signalCommand()
     */
    void receiveSetPSignal();

    /** @brief I paraméter beállítása.
     *
     *  QML esemény hatására hívódik meg.
     *  Lekérdezi a beállított értéket a getValueFromQML metódussal.
     *  Emittálja a signalCommand jelzést.
     *
     *  @see getValueFromQML()
     *  @see signalCommand()
     */
    void receiveSetISignal();

    /** @brief D paraméter beállítása.
     *
     *  QML esemény hatására hívódik meg.
     *  Lekérdezi a beállított értéket a getValueFromQML metódussal.
     *  Emittálja a signalCommand jelzést.
     *
     *  @see getValueFromQML()
     *  @see signalCommand()
     */
    void receiveSetDSignal();

    /** @brief Alapjel beállítása az 1-es hőmérséklethez.
     *
     *  QML esemény hatására hívódik meg.
     *  Lekérdezi a beállított értéket a getValueFromQML metódussal.
     *  Emittálja a signalCommand jelzést.
     *
     *  @see getValueFromQML()
     *  @see signalCommand()
     */
    void receiveSetTemp1Signal();

    /** @brief Alapjel beállítása az 2-es hőmérséklethez.
     *
     *  QML esemény hatására hívódik meg.
     *  Lekérdezi a beállított értéket a getValueFromQML metódussal.
     *  Emittálja a signalCommand jelzést.
     *
     *  @see getValueFromQML()
     *  @see signalCommand()
     */
    void receiveSetTemp2Signal();

    /** @brief Alapjel beállítása az 3-as hőmérséklethez.
     *
     *  QML esemény hatására hívódik meg.
     *  Lekérdezi a beállított értéket a getValueFromQML metódussal.
     *  Emittálja a signalCommand jelzést.
     *
     *  @see getValueFromQML()
     *  @see signalCommand()
     */
    void receiveSetTemp3Signal();

    /** @brief Alapjel beállítása az 4-es hőmérséklethez.
     *
     *  QML esemény hatására hívódik meg.
     *  Lekérdezi a beállított értéket a getValueFromQML metódussal.
     *  Emittálja a signalCommand jelzést.
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

