#include <gui.h>
#include <iostream>

using namespace std;

GUI::GUI(QObject *rootObject, StateHistory& stateHistory) : stateHistory(stateHistory), tester()
{
    std::cout << "GUI constructor called" << std::endl;

    mainWindowObject = findItemByName(rootObject, QString("ApplicationWindow"));

    if (!mainWindowObject)
    {
        qDebug() << "ApplicationWindow object not found";
    }

    QObject::connect(mainWindowObject, SIGNAL(connectSignal()),     this, SLOT(receiveConnectSignal()));
    QObject::connect(mainWindowObject, SIGNAL(disconnectSignal()),  this, SLOT(receiveDisconnectSignal()));
    QObject::connect(mainWindowObject, SIGNAL(startSignal()),       this, SLOT(receiveStartSignal()));
    QObject::connect(mainWindowObject, SIGNAL(stopSignal()),        this, SLOT(receiveStopSignal()));

    QObject::connect(&tester, SIGNAL(sendMessage(QString)),         this, SLOT(testMessage(QString)));
}

void GUI::sendSignal(qint8 pid_ID, qint32 data) //for debug to emit signal to Proxy
{
    emit signalCommand(pid_ID,data);
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
    auto MainForm = findItemByName("MainForm");

    QVariant returnedValue;
    QVariant messageText = message;
    QMetaObject::invokeMethod(MainForm, "showMessage",
        Q_RETURN_ARG(QVariant, returnedValue),
        Q_ARG(QVariant, messageText));
}

void GUI::receiveConnectSignal()
{
    qDebug() << "Connect" << endl;
}

void GUI::receiveDisconnectSignal()
{
    qDebug() << "Disconnect" << endl;
}

void GUI::receiveStartSignal()
{
    qDebug() << "Start" << endl;
    tester.Start(1);
}

void GUI::receiveStopSignal()
{
    qDebug() << "Stop" << endl;
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

GUITester::GUITester()
{
    connect(&timer, SIGNAL(timeout()), this, SLOT(tick()));
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
    cout << "time" << endl;
    emit sendMessage("Hello world!");
}
