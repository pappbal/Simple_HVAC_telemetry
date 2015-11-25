#include <statehistory.h>

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
        case ID_act_signal1:
            acts.act1 = package.payload[0];
        break;
        case ID_act_signal2:
            acts.act2 = package.payload[0];
        break;
        default:
            std::cout << "ERROR: Undefined package ID: %d, package dropped." << std::endl;
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

/*******************************************************/

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

    emit newData();
}
