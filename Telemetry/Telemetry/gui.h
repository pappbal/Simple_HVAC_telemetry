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

    GUITester tester;

    const int showStateNumber = 40;
    QList<double> graphTemperatures1;
    QList<double> graphTemperatures2;
    QList<double> graphTemperatures3;
    QList<double> graphTemperatures4;

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

    // From QML
    void receiveConnectSignal();
    void receiveDisconnectSignal();
    void receiveStartSignal();
    void receiveStopSignal();

    // From GUITester
    void testMessage(QString message);
};

#endif // GUI

