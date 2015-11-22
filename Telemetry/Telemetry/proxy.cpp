#ifndef PROXY_H
#include <proxy.h>

#endif // PROXY_H



Package::Package(QByteArray& data)
{
    if(data != NULL)
    {
        this->ID = data[0];
        payload.resize(data.size() - 1);
        for(int i=1;i<data.size();i++)
        {
            this->payload[i-1] = data[i];
        }
    }
}

Package::Package(const Package& other)
{
    this->ID = other.ID;
    this->payload = other.payload;
}

Package::Package()
{
    //this->ID = 0;
    //this->payload = NULL;
}

QDataStream& operator<<(QDataStream& out, const Package& package)
{
    out << package.ID << package.payload;
    return out;
}


void Temperatures::reset()
{
    temp1 = temp2 = temp3 = temp4 = 0;
}


Temperatures& Temperatures::operator=(Temperatures&& other)
{
    if (this != &other)
    {
    this->temp1 = other.temp1;
    this->temp2 = other.temp2;
    this->temp3 = other.temp3;
    this->temp4 = other.temp4;
    other.temp1 = 0;
    other.temp2 = 0;
    other.temp3 = 0;
    other.temp4 = 0;
    }

    return *this;
}

Temperatures& Temperatures::operator=(const Temperatures& other)
{
    if (this != &other)
    {
    this->temp1 = other.temp1;
    this->temp2 = other.temp2;
    this->temp3 = other.temp3;
    this->temp4 = other.temp4;
    }

    return *this;
}

void Speeds::reset()
{
    speed1 = speed2 = 0;
}

Speeds& Speeds::operator=(Speeds&& other)
{
    if(this != &other)
    {
        this->speed1 = other.speed1;
        this->speed2 = other.speed2;
        other.speed1 = 0;
        other.speed2 = 0;
    }
    return *this;
}

Speeds& Speeds::operator=(const Speeds& other)
{
    if(this != &other)
    {
        this->speed1 = other.speed1;
        this->speed2 = other.speed2;
    }
    return *this;
}

State::State()
{
    deficient = 1;
    param_set = QVector<quint8>(no_of_params);
    this->reset();
}



//move constructor
State::State(State&& other)
{
    std::cout << "Moving resource: State" << std::endl;
    this->temps = other.temps;
    this->speeds = other.speeds;
    this->deficient = other.deficient;
    this->param_set = other.param_set;
    //other.temps = nullptr;
   // other.speeds = nullptr;
    other.deficient = 0;
    other.param_set.clear();
}

State::State(const State& other)
{
    std::cout << "Copying resource: State" << std::endl;
    this->temps = other.temps;
    this->speeds = other.speeds;
    this->deficient = other.deficient;
    this->param_set = other.param_set;
}

//move assignment operator
State& State::operator=(State&& other)
{
    if (this != &other)
      {
         // Free the existing resource.
         //delete temps;
         //delete speeds;
         //delete[] param_set;

         // Copy the data pointer and its length from the
         // source object.
         this->temps = other.temps;
         this->speeds = other.speeds;
         this->deficient = other.deficient;
         this->param_set = other.param_set;

         // Release the data pointer from the source object so that
         // the destructor does not free the memory multiple times.
         //other.temps = nullptr;
         //other.speeds = nullptr;
         other.deficient = 0;
         other.param_set.clear();
      }
      return *this;
}

State& State::operator=(const State& other)
{
    if (this != &other)
      {
         // Free the existing resource.
         //delete temps;
         //delete speeds;
         //delete &param_set;

         // Copy the data pointer and its length from the
         // source object.
         temps = other.temps;
         speeds = other.speeds;
         deficient = other.deficient;
         param_set = other.param_set;

      }
      return *this;
}



void State::setParam(Package &package) //ebben allapitom meg, hogy milyen ID es hogy ez alapjan melyik mezot irom.
{
    param_set[package.ID-1] = (unsigned char)1;
    switch(package.ID)
    {
        case ID_temp1:
            temps.temp1 = package.payload[0];
        break;
        case ID_temp2:
            temps.temp2 = package.payload[0];
        break;
        case ID_temp3:
            temps.temp3 = package.payload[0];
        break;
        case ID_temp4:
            temps.temp4 = package.payload[0];
        break;
        case ID_speed1:
            speeds.speed1 = package.payload[0];
        break;
        case ID_speed2:
            speeds.speed2 = package.payload[0];
        break;
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
    //generates a random ID from 0-6, payload is the double of ID, 0 is handled as invalid
    Package package;
    package.ID=rand() % 7;
    package.payload.append(package.ID*2);
    return package;
}

void Communication::sendSignal()
{
    emit signalProxy();
}




StateHistory::StateHistory():stateContainer(0)
{
}


/*StateHistory::~StateHistory()
{
//TODO
}*/

void StateHistory::append(State& state)
{
    State* stateToStore = new State(state);

    stateContainer.append(*stateToStore);

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
