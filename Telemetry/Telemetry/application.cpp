#include <application.h>

Package::Package(QByteArray data)
{
    if(data != NULL)
    {
        this->ID = data[0];
        //payload.resize(data.size() - 5);
        for(int i=5;i<data.size();i++)
        {
            //this->payload[i-5] = data[i];
            payload.append(data[i]);
        }


        std::cout << "ID: " << (uint)ID << std::endl;
    }
}

Package::Package(quint8 ID, qint8 data)
{
    this->ID = ID;
    this -> payload = QVector<qint8>(1);
    this -> payload[0] = data;
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

void Actuators::reset()
{
    act1 = act2 = 0;
}
Actuators& Actuators::operator=(Actuators&& other)
{
    if(this != &other)
    {
        this->act1 = other.act1;
        this->act2 = other.act2;
        other.act1 = 0;
        other.act2 = 0;
    }
    return *this;
}

Actuators& Actuators::operator=(const Actuators& other)
{
    if(this != &other)
    {
        this->act1 = other.act1;
        this->act2 = other.act2;
    }
    return *this;
}
