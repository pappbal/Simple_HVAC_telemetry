#include <application.h>


/**
 * @brief Package::Package
 * Construct a  package from a byte array. First byte must be the ID.
 * @param data
 */
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
        for(auto elem : payload){
            std::cout << "data: " << static_cast<unsigned>(elem) << std::endl;
        }
    }
}

/**
 * @brief Package::Package
 * Constructs a package from an explicit ID and a data field of 32 bits.
 * @param ID - ID to send via the Communication
 * @param data - Data to send
 */
Package::Package(quint8 ID, quint32 data)
{
    this->ID = ID;
    this -> payload = QVector<quint8>(4);

    this -> payload[0] = data;
    this -> payload[1] = data >> 8;
    this -> payload[2] = data >> 16;
    this -> payload[3] = data >> 24;
}

/**
 * @brief Package::Package
 * Copy constructor
 * @param other - Reference to the object to copy
 */
Package::Package(const Package& other)
{
    this->ID = other.ID;
    this->payload = other.payload;
}

/**
 * @brief Package::Package
 * Constructor
 */
Package::Package()
{
    //this->ID = 0;
    //this->payload = NULL;
}

/**
 * @brief operator <<
 * Serializing package to stream into communication
 * @param out - the stream to use
 * @param package - package to serialize
 * @return  - the stream
 */
QDataStream& operator<<(QDataStream& out, const Package& package)
{
    out << package.ID << package.payload;
    return out;
}

/**
 * @brief Temperatures::reset
 * Resets all the temperatures to zero.
 */
void Temperatures::reset()
{
    temp1 = temp2 = temp3 = temp4 = 0;
}

/**
 * @brief Temperatures::operator =
 * Move assignment operator. Implemented but not used.
 * @param other
 * @return
 */
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

/**
 * @brief Temperatures::operator =
 * Assignment operator for Temperatures. Implemented but not used
 * @param other
 * @return
 */
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

/**
 * @brief Speeds::reset
 * Resets the speed values to zero.
 */
void Speeds::reset()
{
    speed1 = speed2 = 0;
}

/**
 * @brief Speeds::operator =
 * Move assignment operator for Speeds. Implemented but not used.
 * @param other
 * @return
 */
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

/**
 * @brief Speeds::operator =
 * Assignment operator for Speeds.
 * @param other
 * @return
 */
Speeds& Speeds::operator=(const Speeds& other)
{
    if(this != &other)
    {
        this->speed1 = other.speed1;
        this->speed2 = other.speed2;
    }
    return *this;
}

/**
 * @brief Actuators::reset
 * Resets the actuator values to zero.
 */
void Actuators::reset()
{
    act1 = act2 = 0;
}

/**
 * @brief Actuators::operator =
 * Move assignment operator for Actuators. Implemented but not used.
 * @param other
 * @return
 */
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

/**
 * @brief Actuators::operator =
 * Assignment operator for Actuators.
 * @param other
 * @return
 */
Actuators& Actuators::operator=(const Actuators& other)
{
    if(this != &other)
    {
        this->act1 = other.act1;
        this->act2 = other.act2;
    }
    return *this;
}
