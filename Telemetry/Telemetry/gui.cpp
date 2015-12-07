#include <gui.h>
#include <iostream>
#include <sstream>
#include "application.h"

using namespace std;

GUI::GUI(QObject *rootObject, QQmlContext &qmlContext, StateHistory& stateHistory) : qmlContext(qmlContext), stateHistory(stateHistory)/*, tester(stateHistory)*/
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
    QObject::connect(mainWindowObject, SIGNAL(setPSignal()),        this, SLOT(receiveSetPSignal()));
    QObject::connect(mainWindowObject, SIGNAL(setISignal()),        this, SLOT(receiveSetISignal()));
    QObject::connect(mainWindowObject, SIGNAL(setDSignal()),        this, SLOT(receiveSetDSignal()));
    QObject::connect(mainWindowObject, SIGNAL(setTemp1()),          this, SLOT(receiveSetTemp1Signal()));
    QObject::connect(mainWindowObject, SIGNAL(setTemp2()),          this, SLOT(receiveSetTemp2Signal()));
    QObject::connect(mainWindowObject, SIGNAL(setTemp3()),          this, SLOT(receiveSetTemp3Signal()));
    QObject::connect(mainWindowObject, SIGNAL(setTemp4()),          this, SLOT(receiveSetTemp4Signal()));

    /*QObject::connect(&tester, SIGNAL(sendMessage(QString)),         this, SLOT(testMessage(QString)));
    QObject::connect(&tester, SIGNAL(stateHistoryUpdateSimulatorSignal()), this, SLOT(stateHistoryUpdated()));*/
   QObject::connect(&stateHistory, SIGNAL(newData()), this, SLOT(stateHistoryUpdated()));

    ConnectQmlSignals(rootObject);
}

void GUI::ConnectQmlSignals(QObject *rootObject)
{
    QQuickItem *historyGraphTemp = findItemByName(rootObject, QString("historyGraphTemperature"));
    if (historyGraphTemp)
    {
        QObject::connect(this, SIGNAL(historyContextUpdated()), historyGraphTemp, SLOT(requestPaint()));
    }
    else
    {
        qDebug() << "GUI ERROR: historyGraphTemp not found in QML";
    }

    QQuickItem *historyGraphSpeed = findItemByName(rootObject, QString("historyGraphSpeed"));
    if (historyGraphSpeed)
    {
        QObject::connect(this, SIGNAL(historyContextUpdated()), historyGraphSpeed, SLOT(requestPaint()));
    }
    else
    {
        qDebug() << "GUI ERROR: historyGraphSpeed not found in QML";
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

     // Send text message to QML side
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


    // Send previous temp and speed values to QML side for the graphs
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
    graphSpeeds1.clear();
    graphSpeeds2.clear();
    unsigned lastElementsNum = stateHistory.GetSize() > showStateNumber ? showStateNumber : stateHistory.GetSize();
    auto it = stateHistory.End() - lastElementsNum;
    for(; it != stateHistory.End(); it++)
    {
        graphTemperatures1.append(it->temps.temp1);
        graphTemperatures2.append(it->temps.temp2);
        graphTemperatures3.append(it->temps.temp3);
        graphTemperatures4.append(it->temps.temp4);

        graphSpeeds1.append(it->speeds.speed1);
        graphSpeeds2.append(it->speeds.speed2);
    }

    qmlContext.setContextProperty(QStringLiteral("historyGraphTemperatures1"), QVariant::fromValue(graphTemperatures1));
    qmlContext.setContextProperty(QStringLiteral("historyGraphTemperatures2"), QVariant::fromValue(graphTemperatures2));
    qmlContext.setContextProperty(QStringLiteral("historyGraphTemperatures3"), QVariant::fromValue(graphTemperatures3));
    qmlContext.setContextProperty(QStringLiteral("historyGraphTemperatures4"), QVariant::fromValue(graphTemperatures4));

    qmlContext.setContextProperty(QStringLiteral("historyGraphSpeeds1"), QVariant::fromValue(graphSpeeds1));
    qmlContext.setContextProperty(QStringLiteral("historyGraphSpeeds2"), QVariant::fromValue(graphSpeeds2));

    emit historyContextUpdated();

    // Send current measured values to QML for show the current measured values
    qmlContext.setContextProperty(QStringLiteral("valueMeasuredTemp1"), QVariant::fromValue(currentState.temps.temp1));
    qmlContext.setContextProperty(QStringLiteral("valueMeasuredTemp2"), QVariant::fromValue(currentState.temps.temp2));
    qmlContext.setContextProperty(QStringLiteral("valueMeasuredTemp3"), QVariant::fromValue(currentState.temps.temp3));
    qmlContext.setContextProperty(QStringLiteral("valueMeasuredTemp4"), QVariant::fromValue(currentState.temps.temp4));

    qmlContext.setContextProperty(QStringLiteral("valueMeasuredSpeed1"), QVariant::fromValue(currentState.speeds.speed1));
    qmlContext.setContextProperty(QStringLiteral("valueMeasuredSpeed2"), QVariant::fromValue(currentState.speeds.speed2));

    qmlContext.setContextProperty(QStringLiteral("valueMeasuredActuator1"), QVariant::fromValue(currentState.acts.act1));
    qmlContext.setContextProperty(QStringLiteral("valueMeasuredActuator2"), QVariant::fromValue(currentState.acts.act2));
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
    qDebug() << "GUI: connectSignal received";
    sendSignal(ID_connect,0);
}

void GUI::receiveDisconnectSignal()
{
    qDebug() << "GUI: disconnectSignal received";
    sendSignal(ID_disconnect,0);
}

void GUI::receiveStartSignal()
{
    qDebug() << "GUI: startSignal received";
    //tester.Start(1);
    sendSignal(ID_start,0);
}

void GUI::receiveStopSignal()
{
    qDebug() << "GUI: stopSignal received";
    //tester.Stop();
    sendSignal(ID_stop,0);
}

void GUI::receiveSetPSignal()
{
    qDebug() << "GUI: setPSignal received";

    int valueP = getValueFromQML("setPID", "getP");

    qDebug() << "New P value:" << valueP;
}

void GUI::receiveSetISignal()
{
   qDebug() << "GUI: setISignal received";

    int valueI = getValueFromQML("setPID", "getI");

    qDebug() << "New I value:" << valueI;
}

void GUI::receiveSetDSignal()
{
    qDebug() << "GUI: setDSignal received";

    int valueD = getValueFromQML("setPID", "getD");

    qDebug() << "New D value:" << valueD;
}

void GUI::receiveSetTemp1Signal()
{
    qDebug() << "GUI: setTemp1 received";

    int valueTemp1 = getValueFromQML("temperatureSettings", "getTemp1");

    qDebug() << "New temp1 set value:" << valueTemp1;

    sendSignal(ID_req_temp1,valueTemp1);
}

void GUI::receiveSetTemp2Signal()
{
    qDebug() << "GUI: setTemp2 received";

    int valueTemp2 = getValueFromQML("temperatureSettings", "getTemp2");

    qDebug() << "New temp1 set value:" << valueTemp2;

    sendSignal(ID_req_temp2,valueTemp2);
}

void GUI::receiveSetTemp3Signal()
{
    qDebug() << "GUI: setTemp3 received";

    int valueTemp3 = getValueFromQML("temperatureSettings", "getTemp3");

    qDebug() << "New temp1 set value:" << valueTemp3;
    sendSignal(ID_req_temp3,valueTemp3);
}

void GUI::receiveSetTemp4Signal()
{
    qDebug() << "GUI: setTemp4 received";

    int valueTemp4 = getValueFromQML("temperatureSettings", "getTemp4");

    qDebug() << "New temp1 set value:" << valueTemp4;
     sendSignal(ID_req_temp4,valueTemp4);
}

int GUI::getValueFromQML(const QString &itemName, const char *invokeMethodName)
{
    auto item = findItemByName(itemName);

    if(item == nullptr)
    {
        qDebug() << "temperatureSettings not found";
        return 0;
    }

    QVariant returnedValue;
    QMetaObject::invokeMethod(item, invokeMethodName, Q_RETURN_ARG(QVariant, returnedValue));

    int value = returnedValue.toInt();
    return value;
}

void GUI::stoppedSlot()
{
    std::cout << "HVAC stopped" << std::endl;
    //TODO
    // write stuff to the message box or anywhere on GUI
}

void GUI::disconnectedSlot()
{
    std::cout << "Disconnected" << std::endl;
    //TODO
    // write stuff to the message box or anywhere on GUI
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

    acts.act1 = 1;
    acts.act2 = 2;
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

    newState.acts.act1 = acts.act1 > 64 ? acts.act1 = 1 : acts.act1 += 2;
    newState.acts.act2 = acts.act2 > 64 ? acts.act2 = 1 : acts.act2 += 2;

    stateHistory.AddNewState(newState);

    emit stateHistoryUpdateSimulatorSignal();
}
