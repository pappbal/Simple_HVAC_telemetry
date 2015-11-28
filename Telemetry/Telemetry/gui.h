#ifndef GUI_H
#define GUI_H

#include <application.h>
#include <statehistory.h>
#include <QQmlProperty>
#include <QObject>
#include <QQuickItem>
#include <QTimer>
#include <QString>

class GUITester : public QObject
{
    Q_OBJECT
public:
    GUITester();

    void Start(float intervalSec);
    void Stop();

private:
    /** Időzítő a tick() metódus periodikus hívására. */
    QTimer timer;

private slots:
    /** A timer hívja meg, meghatározza a robot
     * állapotát a következő időpillanatban. */
    void tick();

signals:
    void sendMessage(QString message);
};



class GUI : public QObject
{
    Q_OBJECT
private:
    StateHistory& stateHistory;

    QQuickItem* findItemByName(const QString& name);
    QQuickItem* findItemByName(QObject *rootObject, const QString& name);
    QQuickItem* findItemByName(QList<QObject*> nodes, const QString& name);
    QQuickItem* mainWindowObject;

    GUITester tester;

public:
    GUI(QObject *rootObject, StateHistory& stateHistory);

    void sendSignal(qint8 pid_ID, qint32 data);

signals:
    void signalCommand(qint8 pid_ID, qint32 data);

public slots:
    void plotData();

    void receiveConnectSignal();
    void receiveDisconnectSignal();
    void receiveStartSignal();
    void receiveStopSignal();

    void testMessage(QString message);
};

#endif // GUI

