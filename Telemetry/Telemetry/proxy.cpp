#include <proxy.h>


/**
 * @brief Proxy::Proxy
 * Constructor of Proxy
 * @param comm - reference to the Communication object
 * @param stateHistory - reference to the StateHistory object
 *
 * The disconnected flag indicates disconnection by default. User must connect before using any functionality.
 */
Proxy::Proxy(Communication& comm, StateHistory& stateHistory):comm(comm),currentState(),stateHistory(stateHistory)
{
    std::cout << "Proxy constructor called" << std::endl;
    disconnected = 1;

    connect(&timer, SIGNAL(timeout()), this, SLOT(tick()));
}

/**
 * @brief Proxy::~Proxy
 * Destructor of Proxy
 */
Proxy::~Proxy()
{
    std::cout << "Proxy destructor called" << std::endl;
}

/**
 * @brief Proxy::dataReady
 * Gets the data from Communication. One type at a time.
 *
 * called by Communication class via a signal (slot)
 * One data field at once, e.g. temp1
 * Must assemble current state, identify packages by IDs (defined)
 * When ready, put to state history (which alerts GUI)
 */
void Proxy::dataReady()
{

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

/**
 * @brief Proxy::sendCommand
 * Connects and disconnects. If connected, sends the command to the Communication class.
 * Restarts the 5 sec timer, to indicate online connection
 *
 * @param pid_ID - the ID to send the packet with
 * @param data - data to send the packet with
 */
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
        emit signalConnected();
       // comm.sendData(package);
        timer.start(timeout_in_sec*1000.0F);
    }
    else if(!(this->disconnected))
    {
        Package package(pid_ID,data);
        comm.sendData(package);

    }
}

/**
 * @brief Proxy::tick
 * Slot for the timer of 5 secs. If timer signals, communication is disconnected.
 * Signals GUI for disconnection.
 */
void Proxy::tick()
{
    this->disconnected = 1;
    timer.stop();
    emit signalDisconnected();
}
