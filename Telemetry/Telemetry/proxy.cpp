#ifndef PROXY_H
#include <proxy.h>

#endif // PROXY_H



QDataStream& operator<<(QDataStream& out, const Package& package)
{
    out << package.ID << package.payload;
    return out;
}

void Temperatures::reset()
{
    temp1 = temp2 = temp3 = temp4 = 0;
}


void Speeds::reset()
{
    speed1 = speed2 = 0;
}

State::State()
{
    deficient = 1;
    param_set = QVector<quint8>(no_of_params);
    this->reset();

}


State::~State()
{
    param_set.~QVector();
}

void State::setParam(Package &package) //ebben allapitom meg, hogy milyen ID es hogy ez alapjan melyik mezot irom.
{
    param_set[package.ID-1] = (unsigned char)1;
    switch(package.ID)
    {
        case ID_temp1:
            temps.temp1 = package.payload[0];
        case ID_temp2:
            temps.temp2 = package.payload[0];
        case ID_temp3:
            temps.temp3 = package.payload[0];
        case ID_temp4:
            temps.temp4 = package.payload[0];
        case ID_speed1:
            speeds.speed1 = package.payload[0];
        case ID_speed2:
            speeds.speed1 = package.payload[0];
        default:
            std::cout << "ERROR: Undefined package ID: %d, package dropped.";
            //TODO also send the message for the GUI!!!
    }

    //TODO set timestamp?
}

int State::readyToSend()
{
    qint32 sum = 0;
    for(int i=0; i<no_of_params; i++)
    {
        sum += param_set[i];
    }
    if(sum == no_of_params)
    {
        deficient = 0;
        return 1;
    }
    else
        return 0;
}

void State::reset()
{
    temps.reset();
    speeds.reset();
    for(int i=0; i<no_of_params; i++)
    {
        param_set[i] = 0;
    }
    deficient = 1;
}


Communication::Communication(){
    std::cout << "Comm constructor called" << std::endl;
}

Communication::~Communication()
{
    std::cout << "Comm destructor called" << std::endl;
}

Package Communication::getData()
{
    Package package;
    package.ID=1;
    package.payload.append(5);
    return package;
}

void Communication::sendSignal()
{
    emit signalProxy();
}

/*void Communication::signalProxy()
{
    std::cout << "Signal called" << std::endl;
}*/


StateHistory::StateHistory():stateContainer(0)
{
}


/*StateHistory::~StateHistory()
{
//TODO
}*/

void StateHistory::append(State &state)
{
    stateContainer.append(state);

    //TODO signal GUI
}

Proxy::Proxy(Communication& comm, StateHistory& stateHistory):comm(comm),currentState(),stateHistory(stateHistory)
{
    std::cout << "Proxy constructor called" << std::endl;
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

    if(package.ID != 0 )
    {
        if(currentState.param_set[package.ID-1] == 1)
        {
            //parameter already set, send deficient state
            stateHistory.append(currentState);
            currentState.reset();
            currentState.setParam(package);
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
