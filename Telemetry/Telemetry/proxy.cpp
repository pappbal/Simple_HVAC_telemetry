#include <proxy.h>

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

void Proxy::sendPID(int pid_ID, int data)
{

    Package package(pid_ID,data);
    comm.sendData(package);
}
