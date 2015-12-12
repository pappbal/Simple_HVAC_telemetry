#ifndef PROXY_H
#define PROXY_H

#include <communication.h>
#include <statehistory.h>
#include <QTimer>
#define timeout_in_sec 5


/**
 * @brief The Proxy class
 * Represents the functionality of the controller. Makes the communication transparent to the GUI.
 *
 * Handles connection and disconnection, creates outbound packages.
 * Stores the incoming data in stateHistory
 * Stores the current state (and collects packages into it untill full or deficient.)
 */
class Proxy : public QObject
{
    Q_OBJECT
private:
    Communication& comm;
    State currentState; //ebbe gyujtom a beerkezo adatokat. Amikor televan, beleteszem a history-ba. Szamon kell tudnom tartni, hogy melyik elemek vannak keszen. Ha olyan jon, amit mar beallitottam, eldobom.
    StateHistory& stateHistory;
    quint8 disconnected;
    QTimer timer;
public:
    Proxy(Communication &comm, StateHistory &stateHistory);

    ~Proxy();
signals:
    /**
     * @brief signalDisconnected
     * Signals GUI about disconnection
     */
    void signalDisconnected();
    /**
     * @brief signalHVACStopped
     * Signals GUI that HVAC is sending "Stopped" messages
     */
    void signalHVACStopped();

    /**
     * @brief signalConnected
     * Signals GUI about connection
     */
    void signalConnected();
public slots:
    void dataReady(); //slot for communication. Called on new incoming packet.
    void sendCommand(qint8 pid_ID, qint32 data); //slot for GUI. Called when parameter is set.
    void tick();
};




#endif // PROXY_H

