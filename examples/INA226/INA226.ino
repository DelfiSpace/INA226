/*
 * Copyright (c) 2016 by Stefano Speretta <s.speretta@tudelft.nl>
 *
 * I2CTester: this is an application designed to test the 
 * functionality of the I2C Tester setup. The setup includes several 
 * devices on different boards to perform a long duration test on 
 * the hardware and software implementation. It is designed to use 
 * Energia (the Arduino port for MSP microcontrollers) on an MSP432.
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

INA226 ina(wire, 0x45);

void setup() 
{
  // Initialize I2C master
  wire.setFastMode();
  wire.begin();

  // initialize debug UART
  serial.begin();

  // ensure the serial port is initialized
  delay(200);
  
  serial.println();
  serial.println("-----------------------------------------------------");
  serial.println("------------------    I2CTester    ------------------");
  serial.println("-----------------------------------------------------");
  serial.println();

  unsigned char r = ina.setShuntResistor(0.04);
  if (r)
  {
    serial.println("setShuntResistor   FAILURE");
  }
  else
  {
    serial.println("setShuntResistor   SUCCESS");
  }
  
}

void loop() 
{
  unsigned char failure;

  serial.print("Ping:");
  if (ina.ping())
  {
    
    serial.println("   SUCCESS");
  }
  else
  {
    serial.println("   FAILURE");
  }

  unsigned short v;
  failure = ina.getVoltage(v);
  serial.print("Voltage: ");
  serial.print(v, DEC);
  serial.print(" mV");
  if (failure)
  {
    serial.println("   FAILURE");
  }
  else
  {
    serial.println("   SUCCESS");
  }

  signed short i;
  failure = ina.getCurrent(i);
  serial.print("Current: ");
  serial.print(i, DEC);
  serial.print(" mA");
  if (failure)
  {
    serial.println("   FAILURE");
  }
  else
  {
    serial.println("   SUCCESS");
  }
  
  unsigned short p;
  failure = ina.getPower(p); 
  serial.print("Power: ");
  serial.print(p, DEC);
  serial.print(" mW");
  if (failure)
  {
    serial.println("   FAILURE");
  }
  else
  {
    serial.println("   SUCCESS");
  }
  
  serial.println();
  
  delay(1000);
}