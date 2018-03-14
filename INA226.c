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

#include "INA226.h"
#include "hal_uart.h"

#define CONFIGURATION    0x00
#define SHUNT            0x01
#define VOLTAGE          0x02
#define POWER            0x03
#define CURRENT          0x04
#define CALIBRATION      0x05
#define ID               0xFF

#define DEVICE_ID        0x2260
#define RESET            0x8000
#define CALIBRATION_REF  40.96f


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
uint8_t ina_readRegister(const dev_id id, uint8_t reg, uint16_t *res)
{
    HAL_I2C_readWrite(id, &reg, 1, res, 2);

    return 0;
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
void ina_writeRegister(const dev_id id, const uint8_t reg, const uint16_t val)
{
    uint8_t tx_buf[3] = { reg, ((val >> 8) & 0xFF), (val & 0xFF) };

    HAL_I2C_readWrite(id, tx_buf, 3, NULL, 0);

}

/**
 *
 *   Reset the INA226
 *
 */
void ina_reset(dev_id id)
{
   ina_writeRegister(id, CONFIGURATION, RESET);
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
bool ina_readDeviceID(dev_id id)
{
    uint16_t res = 0;
    ina_readRegister(id, ID, &res);

    return res == DEVICE_ID;
}

/**
 *
 *   Sets the shunt resistor value in Ohm
 *
 *   Parameters:
 *   double shunt          shunt resistor value in Ohm
 *
 */
void ina_setShuntResistor(dev_id id, double shunt)
{
    ina_writeRegister(id, CALIBRATION, (uint16_t)(CALIBRATION_REF / shunt));
}

/**
 *
 *   Returns the bus voltage in mV
 *
 *   Returns:
 *   unsigned short        bus voltage in mV
 *
 */
uint16_t ina_getVoltage(dev_id id)
{
    uint16_t res = 0;
    ina_readRegister(id, VOLTAGE, &res);
    return res + (res >> 2);
}

/**
 *
 *   Returns the voltage across the shunt resistor
 *
 *   Returns:
 *   signed short        bus voltage (LSB = 2.5 uV)
 *
 */
int16_t ina_getShuntVoltage(dev_id id)
{
    uint16_t res = 0;
    ina_readRegister(id, SHUNT, &res);
    return res;
}

/**
 *
 *   Returns the current through the shunt resistor
 *
 *   Returns:
 *   signed short         current in mA
 *
 */
int16_t ina_getCurrent(dev_id id)
{
    uint16_t res = 0;
    ina_readRegister(id, CURRENT, &res);
    return (res >> 3);
}

/**
 *
 *   Returns the power across the load in mW
 *
 *   Returns:
 *   unsigned short        power in mW
 *
 */
uint16_t ina_getPower(dev_id id)
{
    uint16_t res = 0;
    ina_readRegister(id, POWER, &res);
    return (res * 3) + (res >> 3);
}
