/*
 * Copyright (c) 2016 by Stefano Speretta <s.speretta@tudelft.nl>
 *
 * INA226 example: this is an example to show how to use the INA226 
 * library. It is designed to use Energia (the Arduino port for MSP 
 * microcontrollers) on an MSP432.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License 
 * version 3, both as published by the Free Software Foundation.
 *
 */
 
#include <DWire.h>
#include <DSerial.h>
#include <INA226.h>

DWire wire;
DSerial serial;
INA226 ina(wire, 0x40);

void setup() 
{
  // Initialize I2C master
  wire.setFastMode();
  wire.begin();

  // initialize the UART
  serial.begin();
  
  // initialize the shunt resistor
  ina.setShuntResistor(0.04);

  // ensure the hardware is initialized
  delay(100);
}

void loop() 
{
  signed short i = ina.getCurrent();
  unsigned short v = ina.getVoltage();
  unsigned short p = ina.getPower();
  
  serial.print("Current: ");
  serial.print(i, DEC);
  serial.println(" mA");  

  serial.print("Voltage: ");
  serial.print(v, DEC);
  serial.println(" mV");  

  serial.print("Power: ");
  serial.print(p, DEC);
  serial.println(" mW");  

  delay(1000);
}
