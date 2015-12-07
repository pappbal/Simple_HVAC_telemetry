#include <proxy.h>

Proxy::Proxy(Communication& comm, StateHistory& stateHistory):comm(comm),currentState(),stateHistory(stateHistory)
{
    std::cout << "Proxy constructor called" << std::endl;
    disconnected = 1;
    connect(&timer, SIGNAL(timeout()), this, SLOT(tick()));
}

Proxy::~Proxy()
{
    std::cout << "Proxy destructor called" << std::endl;
}

void Proxy::dataReady()
{
    //called by Communication class via a signal
    //One data field at one, e.g. temp1
    //Must assemble current state, identify packages by IDs (defined)
    //When ready, put to state history (which alerts GUI)
    std::cout << "Slot called" << std::endl;
    Package package;
    package = comm.getData();
    timer.start(timeout_in_sec*1000.0F);
    if(!disconnected)
    {
    if(package.ID == ID_HVAC_stopped)
    {
        emit signalHVACStopped();
    }
    else if(package.ID != 0 )
    {
        if(currentState.param_set[package.ID-1] == 1)
        {
            //parameter already set, send deficient state
            stateHistory.append(currentState);
            currentState.reset();
            currentState.setParam(package);
            std::cout << "just for breakpoint" << std::endl;
        }
        else
        {
            currentState.setParam(package);
            if(currentState.readyToSend())
            {
            stateHistory.append(currentState);
            currentState.reset();
            }
        }
    }
    }

}

void Proxy::sendCommand(qint8 pid_ID, qint32 data)
{
    if(pid_ID == ID_disconnect)
    {
        //Package package(ID_stop,0);
        this->disconnected = 1;
        //comm.sendData(package);
        emit signalDisconnected();
        timer.stop();
    }
    else if(pid_ID == ID_connect)
    {
       // Package package(ID_start,0);
        this->disconnected = 0;
       // comm.sendData(package);
        timer.start(timeout_in_sec*1000.0F);
    }
    else if(!(this->disconnected))
    {
        Package package(pid_ID,data);
        comm.sendData(package);

    }
}

void Proxy::tick()
{
    this->disconnected = 1;
    timer.stop();
    emit signalDisconnected();
}
