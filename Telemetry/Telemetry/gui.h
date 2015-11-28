#ifndef GUI_H
#define GUI_H

#include <application.h>
#include <statehistory.h>
#include <QQmlProperty>
#include <QObject>
#include <QQuickItem>

class GUI : public QObject
{
    Q_OBJECT
private:
    StateHistory& stateHistory;

    QQuickItem* findItemByName(const QString& name);
    QQuickItem* findItemByName(QObject *rootObject, const QString& name);
    QQuickItem* findItemByName(QList<QObject*> nodes, const QString& name);
    QQuickItem* mainWindowObject;

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
};




#endif // GUI

