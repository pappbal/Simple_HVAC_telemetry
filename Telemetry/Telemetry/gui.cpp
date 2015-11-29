#include <gui.h>
#include <iostream>
#include <sstream>

using namespace std;

GUI::GUI(QObject *rootObject, QQmlContext &qmlContext, StateHistory& stateHistory)
    : qmlContext(qmlContext), stateHistory(stateHistory), tester(stateHistory)
{
    mainWindowObject = findItemByName(rootObject, QString("ApplicationWindow"));

    if (!mainWindowObject)
    {
        qDebug() << "ApplicationWindow object not found";
    }

    QObject::connect(&stateHistory, SIGNAL(newData()),  this, SLOT(stateHistoryUpdated()));

    QObject::connect(mainWindowObject, SIGNAL(connectSignal()),     this, SLOT(receiveConnectSignal()));
    QObject::connect(mainWindowObject, SIGNAL(disconnectSignal()),  this, SLOT(receiveDisconnectSignal()));
    QObject::connect(mainWindowObject, SIGNAL(startSignal()),       this, SLOT(receiveStartSignal()));
    QObject::connect(mainWindowObject, SIGNAL(stopSignal()),        this, SLOT(receiveStopSignal()));

    QObject::connect(&tester, SIGNAL(sendMessage(QString)),         this, SLOT(testMessage(QString)));
    QObject::connect(&tester, SIGNAL(stateHistoryUpdateSimulatorSignal()), this, SLOT(stateHistoryUpdated()));

    ConnectQmlSignals(rootObject);
}

void GUI::ConnectQmlSignals(QObject *rootObject)
{
    QQuickItem *historyGraph = findItemByName(rootObject, QString("historyGraph"));
    if (historyGraph)
    {
        QObject::connect(this, SIGNAL(historyContextUpdated()), historyGraph, SLOT(requestPaint()));
    }
    else
    {
        qDebug() << "GUI ERROR: historyGraph not found in QML";
    }
}

void GUI::sendSignal(qint8 pid_ID, qint32 data) //for debug to emit signal to Proxy
{
    emit signalCommand(pid_ID,data);
}

void GUI::stateHistoryUpdated()
{
    qDebug() << "GUI: slot stateHistorySignal received a signal";

    if(!stateHistory.GetSize())
    {
        return;
    }

    State currentState = stateHistory.GetCurrentState();
    ostringstream stream;
    stream << "T1: " << currentState.temps.temp1 << " ";
    stream << "T2: " << currentState.temps.temp2 << " ";
    stream << "T3: " << currentState.temps.temp3 << " ";
    stream << "T4: " << currentState.temps.temp4 << " ";
    stream << "S1: " << currentState.speeds.speed1 << " ";
    stream << "S2: " << currentState.speeds.speed2 << " ";
    string messageString = stream.str();
    QString message = QString::fromStdString(messageString);

    auto MainForm = findItemByName("MainForm");
    QVariant returnedValue;
    QVariant messageText = message;
    QMetaObject::invokeMethod(MainForm, "showMessage",
        Q_RETURN_ARG(QVariant, returnedValue),
        Q_ARG(QVariant, messageText));

    QList<int> graphVelocities;

    for(int i=0; i<showStateNumber; i++)
    {
        graphVelocities.append(0);
    }
    qmlContext.setContextProperty(QStringLiteral("historyGraphVelocity"), QVariant::fromValue(graphVelocities));


    graphTemperatures1.clear();
    graphTemperatures2.clear();
    graphTemperatures3.clear();
    graphTemperatures4.clear();
    unsigned lastElementsNum = stateHistory.GetSize() > showStateNumber ? showStateNumber : stateHistory.GetSize();
    auto it = stateHistory.End() - lastElementsNum;
    for(; it != stateHistory.End(); it++)
    {
        graphTemperatures1.append(it->temps.temp1);
        graphTemperatures2.append(it->temps.temp2);
        graphTemperatures3.append(it->temps.temp3);
        graphTemperatures4.append(it->temps.temp4);
    }

    qmlContext.setContextProperty(QStringLiteral("historyGraphTemperatures1"), QVariant::fromValue(graphTemperatures1));
    qmlContext.setContextProperty(QStringLiteral("historyGraphTemperatures2"), QVariant::fromValue(graphTemperatures2));
    qmlContext.setContextProperty(QStringLiteral("historyGraphTemperatures3"), QVariant::fromValue(graphTemperatures3));
    qmlContext.setContextProperty(QStringLiteral("historyGraphTemperatures4"), QVariant::fromValue(graphTemperatures4));

    emit historyContextUpdated();

}

void GUI::plotData(){
    std::cout << "-----------INCOMING---------"    << std::endl;
    std::cout << "Deficient: " << (uint)stateHistory.stateContainer.end()->deficient << std::endl;
    std::cout << "Speed1: "    << (uint)stateHistory.stateContainer.end()->speeds.speed1 << std::endl;
    std::cout << "Speed2: "    << (uint)stateHistory.stateContainer.end()->speeds.speed2 << std::endl;
    std::cout << "Temp1: "     << stateHistory.stateContainer.end()->temps.temp1 << std::endl;
    std::cout << "Temp2: "     << stateHistory.stateContainer.end()->temps.temp2 << std::endl;
    std::cout << "Temp3: "     << stateHistory.stateContainer.end()->temps.temp3 << std::endl;
    std::cout << "Temp4: "     << stateHistory.stateContainer.end()->temps.temp4 << std::endl;

    std::cout << "Act1: " << stateHistory.stateContainer.end()->acts.act1 << std::endl;
    std::cout << "Act2: " << stateHistory.stateContainer.end()->acts.act2 << std::endl;

}

void GUI::testMessage(QString message)
{
    qDebug() << "GUI: slot testMessage received a signal";
    auto MainForm = findItemByName("MainForm");

    QVariant returnedValue;
    QVariant messageText = message;
    QMetaObject::invokeMethod(MainForm, "showMessage",
        Q_RETURN_ARG(QVariant, returnedValue),
        Q_ARG(QVariant, messageText));
}

void GUI::receiveConnectSignal()
{
    qDebug() << "Connect";
}

void GUI::receiveDisconnectSignal()
{
    qDebug() << "Disconnect";
}

void GUI::receiveStartSignal()
{
    qDebug() << "Start";
    tester.Start(1);
}

void GUI::receiveStopSignal()
{
    qDebug() << "Stop";
    tester.Stop();
}

QQuickItem* GUI::findItemByName(const QString& name)
{
    Q_ASSERT(mainWindowObject != nullptr);
    if (mainWindowObject->objectName() == name)
    {
        return mainWindowObject;
    }
    return findItemByName(mainWindowObject->children(), name);
}

QQuickItem* GUI::findItemByName(QObject *rootObject, const QString& name)
{
    Q_ASSERT(rootObject != nullptr);
    if (rootObject->objectName() == name)
    {
        return (QQuickItem*)rootObject;
    }
    return findItemByName(rootObject->children(), name);
}

QQuickItem* GUI::findItemByName(QList<QObject*> nodes, const QString& name)
{
    for(int i = 0; i < nodes.size(); i++)
    {
        // Node keresése
        if (nodes.at(i) && nodes.at(i)->objectName() == name)
        {
            return dynamic_cast<QQuickItem*>(nodes.at(i));
        }
        // Gyerekekben keresés
        else if (nodes.at(i) && nodes.at(i)->children().size() > 0)
        {
            QQuickItem* item = findItemByName(nodes.at(i)->children(), name);
            if (item)
                return item;
        }
    }
    // Nem találtuk.
    return nullptr;
}

GUITester::GUITester(StateHistory &stateHistory) : stateHistory(stateHistory)
{
    connect(&timer, SIGNAL(timeout()), this, SLOT(tick()));

    temps.temp1 = 0;
    temps.temp2 = 1;
    temps.temp3 = 2;
    temps.temp4 = 3;

    speeds.speed1 = 0;
    speeds.speed2 = 1;
}

void GUITester::Start(float intervalSec)
{
    timer.start((long)(intervalSec*1000.0F));
}

void GUITester::Stop()
{
    timer.stop();
}

void GUITester::tick()
{
    //emit sendMessage("Hello world!");

    State newState;
    newState.temps.temp1 = temps.temp1 > 10 ? temps.temp1 = 0 : temps.temp1++;
    newState.temps.temp2 = temps.temp2 > 10 ? temps.temp2 = 0 : temps.temp2++;
    newState.temps.temp3 = temps.temp3 > 10 ? temps.temp3 = 0 : temps.temp3++;
    newState.temps.temp4 = temps.temp4 > 10 ? temps.temp4 = 0 : temps.temp4++;
    newState.speeds.speed1 = speeds.speed1 > 5 ? speeds.speed1 = 0 : speeds.speed1++;
    newState.speeds.speed2 = speeds.speed2 > 5 ? speeds.speed2 = 0 : speeds.speed2++;
    stateHistory.AddNewState(newState);

    emit stateHistoryUpdateSimulatorSignal();
}
