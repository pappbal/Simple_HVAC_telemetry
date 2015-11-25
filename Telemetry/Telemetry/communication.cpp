#include <communication.h>

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

void Communication::sendData(Package package)
{
    std::cout << "setData called with package ID of " << (uint)package.ID  <<std::endl;
}

void Communication::sendSignal()
{
    emit signalToProxy();
}
