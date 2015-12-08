#include <statehistory.h>

/**
 * @brief State::State
 * Constructor for the State class. Deficient state is default. Not deficient is set when every datafield is set.
 */
State::State()
{
    deficient = 1;
    param_set = QVector<quint8>(no_of_params);
    this->reset();
}



/**
 * @brief State::State
 * Move constructor for the State class. Implemented but not used.
 * @param other
 */
State::State(State&& other)
{
    this->temps = other.temps;
    this->speeds = other.speeds;
    this->deficient = other.deficient;
    this->param_set = other.param_set;
    this->acts = other.acts;
    //other.temps = nullptr;
   // other.speeds = nullptr;
    other.deficient = 0;
    other.param_set.clear();
}

/**
 * @brief State::State
 * Copy constructor of the State class.
 * @param other
 */
State::State(const State& other)
{
    this->temps = other.temps;
    this->speeds = other.speeds;
    this->deficient = other.deficient;
    this->param_set = other.param_set;
    this->acts = other.acts;
}

/**
 * @brief State::operator =
 * Move assignment operator for the State class. Implemented but not used.
 * @param other
 * @return
 */
State& State::operator=(State&& other)
{
    if (this != &other)
      {
         this->temps = other.temps;
         this->speeds = other.speeds;
         this->deficient = other.deficient;
         this->param_set = other.param_set;
         this->acts = other.acts;

         other.deficient = 0;
         other.param_set.clear();
      }
      return *this;
}

/**
 * @brief State::operator =
 * Assignment operator for the State class.
 * @param other
 * @return
 */
State& State::operator=(const State& other)
{
    if (this != &other)
      {
         temps = other.temps;
         speeds = other.speeds;
         deficient = other.deficient;
         param_set = other.param_set;
         acts = other.acts;

      }
      return *this;
}


/**
 * @brief State::setParam
 * Based on the ID of the inbound package, the relevant datafield in State is set accordingly.
 * @param package - inbound package from Communication
 */
void State::setParam(Package package)
{
    param_set[package.ID-1] = (unsigned char)1;
    switch(package.ID)
    {
        case ID_temp1:
        temps.temp1 = calcTemp(package.payload[0],package.payload[1]);
        break;
        case ID_temp2:
            temps.temp2 = calcTemp(package.payload[0],package.payload[1]);
        break;
        case ID_temp3:
            temps.temp3 = calcTemp(package.payload[0],package.payload[1]);
        break;
        case ID_temp4:
            temps.temp4 = calcTemp(package.payload[0],package.payload[1]);
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
            //TODO also send the message for the GUI
    }

}

/**
 * @brief State::calcTemp
 * Calculates the temperature in Celsius from the two bytes of raw data.
 * @param lower - lower byte of the datafield
 * @param upper - upper byte of the datafield
 * @return
 */
double State::calcTemp(quint8 lower, quint8 upper)
{
    qint16 temp_raw = (((qint16)upper) << 8) + (qint16)lower;
    return 0.0625 * (double)(temp_raw >> 3);
}

/**
 * @brief State::readyToSend
 * Checks if the state is ready to put the history or not, based on deficiency.
 * If the paramater is alredy set by previous package, deficient State is appanded.
 * @return 1 if ready 0 if not
 */
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

/**
 * @brief State::reset
 * Resets the values of the current state after it is copied to the history.
 */
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
/**
 * @brief StateHistory::StateHistory
 * Constructor for the StateHistory class.
 */
StateHistory::StateHistory():stateContainer(0)
{
}

/**
 * @brief StateHistory::append
 * Appends the current state to stateHistory and signals GUI about new data
 * @param state - current state to append
 */
void StateHistory::append(State& state)
{
    State* stateToStore = new State(state);

    stateContainer.append(*stateToStore);

    emit newData();
}
