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

#ifndef __INA226_H__
#define __INA226_H__

#include <limits.h>
#include <DWire.h>

#define INA226_DEVICE_ID            (0x2260)
#define INA226_RESET                (0x8000)
#define INA226_CALIBRATION_REF       40960

#define INA226_REG_CONFIG           (0x00)
#define INA226_REG_SHUNTVOLTAGE     (0x01)
#define INA226_REG_BUSVOLTAGE       (0x02)
#define INA226_REG_POWER            (0x03)
#define INA226_REG_CURRENT          (0x04)
#define INA226_REG_CALIBRATION      (0x05)
#define INA226_REG_MASKENABLE       (0x06)
#define INA226_REG_ALERTLIMIT       (0x07)
#define INA226_REG_ALERTLIMIT       (0x07)
#define INA226_REG_ID               (0xFF)

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

class INA226

{
protected:
    DWire &wire;
    unsigned char address;

public:
    INA226(DWire&, unsigned char);
    virtual ~INA226( ) {};

    unsigned char reset();
    unsigned char ping();

    // configure the device
    unsigned char setShuntResistor( unsigned short );

    // functions used to retrieve the measurements from the device
    unsigned char getShuntVoltage(signed short &);
    unsigned char getVoltage(unsigned short &);
    unsigned char getCurrent(signed short &);
    unsigned char getPower(unsigned short &);

private:
    // only for use
    unsigned char readRegister(unsigned char, unsigned short &);
    unsigned char writeRegister(unsigned char, unsigned short);

};

#endif // __INA226_H__
