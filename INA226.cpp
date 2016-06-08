#include "INA226.h"

/**
 *
 *   INA226 class creator function
 *   
 *   Parameters:
 *   DWire &i2c             I2C object
 *   unsigned char addr     I2C address (7 bits)
 *
 */
INA226::INA226(DWire &i2c, unsigned char addr): wire(i2c)
{
    address = addr;
}

/**
 *
 *   Reset the INA226
 *
 */
void INA226::reset()
{
   writeRegister(CONFIGURATION, RESET);
}

/**
 *
 *   Verify if INA226 is present
 *   
 *   Returns:
 *   true                  INA226 is present
 *   false                 otherwise
 *
 */
unsigned char INA226::ping()
{
    return readRegister(ID) == DEVICE_ID;
}

/**
 *
 *   Sets the shunt resistor value in Ohm
 *   
 *   Parameters:
 *   double shunt          shunt resistor value in Ohm
 *
 */
void INA226:: setShuntResistor(double shunt)
{
    writeRegister(CALIBRATION, (unsigned short)(CALIBRATION_REF / shunt));
}

/**
 *
 *   Returns the bus voltage in mV
 *   
 *   Returns:
 *   unsigned short        bus voltage in mV
 *
 */
unsigned short INA226::getVoltage()
{
    unsigned short v = readRegister(VOLTAGE);
    return v + (v >> 2);
}

/**
 *
 *   Returns the voltage across the shunt resistor
 *   
 *   Returns:
 *   signed short        bus voltage (LSB = 2.5 uV)
 *
 */
signed short INA226::getShuntVoltage()
{
    return readRegister(SHUNT);
}

/**
 *
 *   Returns the current through the shunt resistor
 *   
 *   Returns:
 *   signed short         current in mA
 *
 */
signed short INA226::getCurrent()
{
    return readRegister(CURRENT) >> 3;
}

/**
 *
 *   Returns the power across the load in mW
 *   
 *   Returns:
 *   unsigned short        power in mW
 *
 */
unsigned short INA226::getPower()
{
    unsigned short p= readRegister(POWER);
    return (p * 3) + (p >> 3);
}

/**
 *
 *   Returns the value of the selected internal register
 *   
 *
 *   Parameters:
 *   unsigned char reg     register number
 *
 *   Returns:
 *   unsigned short        register value
 *
 */
unsigned short INA226::readRegister(unsigned char reg)
{
    unsigned short ret = -1;
    wire.beginTransmission(address);
    wire.write(reg);
    //wire.endTransmission(false);

    //delay(30);
    // use the casting to prevent warning on ambiguous conversion
    unsigned char res = wire.requestFrom(address, 2);
    if (res == 2)
    {
        ((unsigned char*)&ret)[1] = wire.read();
        ((unsigned char*)&ret)[0] = wire.read();
    }


    return ret;
}

/**
 *
 *   Sets the value of the selected internal register
 *   
 *
 *   Parameters:
 *   unsigned char reg     register number
 *   unsigned short        register value
 *
 */
void INA226::writeRegister(unsigned char reg, unsigned short val)
{
    wire.beginTransmission(address);
    wire.write(reg);
    wire.write((val >> 8) & 0xFF);
    wire.write(val & 0xFF);      

    wire.endTransmission(true);
}
