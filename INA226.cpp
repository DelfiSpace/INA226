/*
 * Copyright (c) 2016 by Stefano Speretta <s.speretta@tudelft.nl>
 *
 * INA226: a library to provide high level APIs to interface with the
 * TI INA226 current sensor. It is possible to use this library in
 * Energia (the Arduino port for MSP microcontrollers) or in other
 * toolchains.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * version 3, both as published by the Free Software Foundation.
 *
 */

#include <INA226.h>

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
 *   Returns:
 *   unsigned char         0 succes
 *                         1 fail
 *
 */
unsigned char INA226::reset()
{
   return writeRegister(INA226_REG_CONFIG, INA226_RESET);
}

/**
 *
 *   Verify if INA226 is present
 *
 *   Returns:
 *   unsigned char         1 device found
 *                         0 device not found
 *
 */
unsigned char INA226::ping()
{
    unsigned short id;
    unsigned char r = readRegister(INA226_REG_ID, id);
    return !r && (id == INA226_DEVICE_ID);
}

/**
 *
 *   Sets the shunt resistor value in mOhm
 *
 *   Parameters:
 *   unsigned short shunt  shunt resistor value in mOhm
 *
 *   Returns:
 *   unsigned char         0 success
 *                         1 fail
 *
 */
unsigned char INA226:: setShuntResistor(unsigned short shunt)
{
    return writeRegister(INA226_REG_CALIBRATION, INA226_CALIBRATION_REF / shunt);
}

/**
 *
 *   Returns the bus voltage in mV
 *
 *   Parameters:
 *   unsigned short &      bus voltage in mV
 *
 *   Returns:
 *   unsigned char         0 success
 *                         1 fail
 *
 */
unsigned char INA226::getVoltage(unsigned short &v)
{
    unsigned char ret = readRegister(INA226_REG_BUSVOLTAGE, v);
    v = v + (v >> 2);
    if (ret)
    {
        v = USHRT_MAX;
    }
    return ret;
}

/**
 *
 *   Returns the voltage across the shunt resistor
 *
 *   Parameters:
 *   signed short &      bus voltage (LSB = 2.5 uV)
 *
 *   Returns:
 *   unsigned char         0 success
 *                         1 fail
 *
 */
unsigned char INA226::getShuntVoltage(signed short &v)
{
    unsigned char ret = readRegister(INA226_REG_SHUNTVOLTAGE, reinterpret_cast<unsigned short&>(v));
    if (ret)
    {
        v = SHRT_MAX;
    }
    return ret;
}

/**
 *
 *   Returns the current through the shunt resistor
 *
 *   Parameters:
 *   signed short &        current in mA
 *
 *   Returns:
 *   unsigned char         0 success
 *                         1 fail
 *
 */
unsigned char INA226::getCurrent(signed short &c)
{
    unsigned char ret = readRegister(INA226_REG_CURRENT, reinterpret_cast<unsigned short&>(c));
    c/=8;
    if (ret)
    {
        c = SHRT_MAX;
    }
    return ret;
}

/**
 *
 *   Returns the power across the load in mW
 *
 *   Parameters:
 *   unsigned short &      power in mW
 *
 *   Returns:
 *   unsigned char         0 success
 *                         1 fail
 *
 */
unsigned char INA226::getPower(unsigned short &p)
{
    unsigned char ret = readRegister(INA226_REG_POWER, p);
    p = (p * 3) + (p >> 3);
    if (ret)
    {
        p = USHRT_MAX;
    }
    return ret;
}

/**
 *
 *   Returns the value of the selected internal register
 *
 *   Parameters:
 *   unsigned char reg     register number
 *   unsigned short &      register value
 *
 *   Returns:
 *   unsigned char         0 success
 *                         1 fail
 *
 */
unsigned char INA226::readRegister(unsigned char reg, unsigned short &output)
{
    wire.beginTransmission(address);
    wire.write(reg);

    unsigned char res = wire.requestFrom(address, 2);
    if (res == 2)
    {
        output = ((unsigned short)wire.read()) << 8;
        output |= wire.read() & 0xFF;
        return 0;
    }
    else
    {
        return 1;
    }
}

/**
 *
 *   Sets the value of the selected internal register
 *
 *   Parameters:
 *   unsigned char reg     register number
 *   unsigned short        register value
 *
 *   Returns:
 *   unsigned char         0 success
 *                         1 fail
 *
 */
unsigned char INA226::writeRegister(unsigned char reg, unsigned short val)
{
    wire.beginTransmission(address);
    wire.write(reg);
    wire.write((val >> 8) & 0xFF);
    wire.write(val & 0xFF);
    return wire.endTransmission();
}
