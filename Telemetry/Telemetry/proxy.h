#ifndef PROXY_H
#define PROXY_H

#include <communication.h>
#include <statehistory.h>



class Proxy : public QObject
{
    Q_OBJECT
private:
    Communication& comm;
    State currentState; //ebbe gyujtom a beerkezo adatokat. Amikor televan, beleteszem a history-ba. Szamon kell tudnom tartni, hogy melyik elemek vannak keszen. Ha olyan jon, amit mar beallitottam, eldobom.
    StateHistory& stateHistory;
public:
    Proxy(Communication &comm, StateHistory &stateHistory);

    ~Proxy();
public slots:
    void dataReady();
    void sendPID(int pid_ID, int data);
};




#endif // PROXY_H

