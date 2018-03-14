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

#include "satellite.h"
#include <stdint.h>
#include <stdbool.h>

void ina_reset(dev_id id);

bool ina_readDeviceID(dev_id id);

void ina_setShuntResistor(dev_id id, double shunt);

uint16_t ina_getVoltage(dev_id id);

int16_t ina_getShuntVoltage(dev_id id);

int16_t ina_getCurrent(dev_id id);

uint16_t ina_getPower(dev_id id);

#endif // __INA226_H__
