#include <gui.h>

void GUI::sendSignal(int pid_ID, int data)
{
    emit signalPID(pid_ID,data);
}
