#ifndef __INA226_H__
#define __INA226_H__

#include "Energia.h"
#include <DWire.h>

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

class INA226

{
protected:
    DWire &wire;
    unsigned char address;


public:
    INA226(DWire&, unsigned char);
    virtual ~INA226( ) {};
    
    void reset();
    unsigned char ping();

    // configure the device
    void setShuntResistor(double);

    // functions used to retrieve the measurements from the device
    signed short getShuntVoltage();
    unsigned short getVoltage();
    signed short getCurrent();
    unsigned short getPower();

    // used only for debug purposes, use the other functions in normal code
    unsigned short readRegister(unsigned char);
    void writeRegister(unsigned char, unsigned short); 

private:
    
    
};

#endif // __INA226_H__ 