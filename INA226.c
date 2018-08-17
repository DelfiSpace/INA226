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
#include "hal_functions.h"
#include <math.h>

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


#define INA226_REG_CONFIG           (0x00)
#define INA226_REG_SHUNTVOLTAGE     (0x01)
#define INA226_REG_BUSVOLTAGE       (0x02)
#define INA226_REG_POWER            (0x03)
#define INA226_REG_CURRENT          (0x04)
#define INA226_REG_CALIBRATION      (0x05)
#define INA226_REG_MASKENABLE       (0x06)
#define INA226_REG_ALERTLIMIT       (0x07)

#define INA226_BIT_SOL              (0x8000)
#define INA226_BIT_SUL              (0x4000)
#define INA226_BIT_BOL              (0x2000)
#define INA226_BIT_BUL              (0x1000)
#define INA226_BIT_POL              (0x0800)
#define INA226_BIT_CNVR             (0x0400)
#define INA226_BIT_AFF              (0x0010)
#define INA226_BIT_CVRF             (0x0008)
#define INA226_BIT_OVF              (0x0004)
#define INA226_BIT_APOL             (0x0002)
#define INA226_BIT_LEN              (0x0001)

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

    uint8_t rx_buf[3] = { reg, 0, 0 };

    bool res1 = HAL_I2C_readWrite(id, rx_buf, 1, res, 2);

    uint16_t temp;
    temp = *res;
    *res = ((0x00ff & temp) << 8);
    *res |= (temp >> 8);

    return res1;
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
bool ina_writeRegister(const dev_id id, const uint8_t reg, const uint16_t val)
{
    uint8_t tx_buf[3] = { reg, ((val >> 8) & 0xFF), (val & 0xFF) };
 //   uint8_t rx_buf[3] = { };

//    HAL_I2C_readWrite(id, tx_buf, 3, rx_buf, 0);
    bool res = HAL_I2C_readWrite(id, tx_buf, 3, NULL, 0);

    return res;

}

uint16_t INA226_getConfig(const dev_id id)
{
    uint16_t value;
    ina_readRegister(id, INA226_REG_CONFIG, &value);

    return value;
}

uint16_t INA226_getCalibration(const dev_id id)
{
    uint16_t value;
    ina_readRegister(id, INA226_REG_CALIBRATION, &value);

    return value;
}

void INA226_configure(const dev_id id,
                      ina226_averages_t avg,
                      ina226_busConvTime_t busConvTime,
                      ina226_shuntConvTime_t shuntConvTime,
                      ina226_mode_t mode)
{
    uint16_t config = 0;

    config |= (avg << 9 | busConvTime << 6 | shuntConvTime << 3 | mode);

    ina_writeRegister(id, INA226_REG_CONFIG, config);
}

bool INA226_calibrate(const dev_id id,
                      float rShunt,
                      float iMaxExpected,
                      float *currentLSB,
                      float *powerLSB)
{
    uint16_t calibrationValue;

    //float vBusMax = 36;
    //float vShuntMax = 0.08192f;

    //float iMaxPossible;
    //iMaxPossible = vShuntMax / rShunt;
    float minimumLSB;

    minimumLSB = iMaxExpected / 32767;

    *currentLSB = (uint16_t)(minimumLSB * 100000000);
    *currentLSB /= 100000000;
    *currentLSB /= 0.0001;
    *currentLSB = ceil(*currentLSB);
    *currentLSB *= 0.0001;

    *powerLSB = *currentLSB * 25;

    calibrationValue = (uint16_t)((0.00512) / (*currentLSB * rShunt));

    ina_writeRegister(id, INA226_REG_CALIBRATION, calibrationValue);

    ina_readRegister(id, INA226_REG_CALIBRATION, &calibrationValue);

    return true;
}

float INA226_readBusPower(const dev_id id, float powerLSB)
{
    int16_t power;

    ina_readRegister(id, INA226_REG_POWER, &power);

    return (power * powerLSB);
}

float INA226_readShuntCurrent(const dev_id id, float currentLSB)
{
    int16_t current;

    ina_readRegister(id, INA226_REG_CURRENT, &current);

    return (current * currentLSB);
}

float INA226_readShuntVoltage(const dev_id id)
{
    int16_t voltage;

    ina_readRegister(id, INA226_REG_SHUNTVOLTAGE, &voltage);

    return (voltage * 0.0000025);
}

float INA226_readBusVoltage(const dev_id id)
{
    uint16_t voltage;

    ina_readRegister(id, INA226_REG_BUSVOLTAGE, &voltage);

    return (voltage * 0.00125);
}

float INA226_rawBusPower(int16_t power, float powerLSB) {

     return (power * powerLSB);
}

float INA226_rawShuntCurrent(int16_t current, float currentLSB)
{
     return (current * currentLSB);
}

float INA226_rawShuntVoltage(int16_t voltage)
{
     return (voltage * 0.0000025);
}

float INA226_rawBusVoltage(uint16_t voltage)
{
     return (voltage * 0.00125);
}

bool INA226_readBusPower_raw(const dev_id id, uint16_t *power)
{
    bool res;

    res = ina_readRegister(id, INA226_REG_POWER, power);

    return res;
}

bool INA226_readShuntCurrent_raw(const dev_id id, uint16_t *current)
{
    bool res;

    res = ina_readRegister(id, INA226_REG_CURRENT, current);

    return res;
}

bool INA226_readShuntVoltage_raw(const dev_id id, uint16_t *voltage)
{
    bool res;

    res = ina_readRegister(id, INA226_REG_SHUNTVOLTAGE, voltage);

    return res;
}

bool INA226_readBusVoltage_raw(const dev_id id, uint16_t *voltage)
{
    bool res;

    res = ina_readRegister(id, INA226_REG_BUSVOLTAGE, voltage);

    return res;
}

/**
 *
 *   Reset the INA226
 *
 */
void INA226_reset(dev_id id)
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
bool INA226_readDeviceID(dev_id id)
{
    uint16_t res = 0;
    uint8_t reg = ID;

    ina_readRegister(id, &reg, &res);

    return res == DEVICE_ID;
}
