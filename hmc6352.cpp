/*
    hmc6352 Arduino library
    Copyright (C) 2012 Diego Fernandez Duran <diego@goedi.net>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    Based on Vaibhav Bhawsar work:
    http://recombine.net/blog/article/49/hmc6352-sparkfun-compass-and-arduino
*/

#include "hmc6352.h"
#include <Arduino.h>
#include <Wire.h>

HMC6352::HMC6352(int address)
{
  
  this->address = address;

  // Shift the device's documented slave address (0x42) 1 bit right
  // This compensates for how the TWI library only wants the
  // 7 most significant bits (with the high bit padded with 0)
  slave_address = address >> 1;

  // Start Wire library
  Wire.begin();
}

int HMC6352::get_heading()
{
  // Send a "A" command to the HMC6352
  // This requests the current heading data
  Wire.beginTransmission(slave_address);
  // Send "Get Data" command
  Wire.write("A");           
  Wire.endTransmission();
  // The HMC6352 needs at least a 70us (microsecond) delay
  // after this command.  Using 10ms just makes it safe
  delay(10);
  // Read the 2 heading bytes, MSB first
  // The resulting 16bit word is the compass heading in 10th's of a degree
  // For example: a heading of 1345 would be 134.5 degrees
  // Request the 2 byte heading (MSB comes first)
  Wire.requestFrom(slave_address, 2);

  byte headingData[2];
  int i = 0;
  while(Wire.available() && i < 2)
  { 
    headingData[i] = Wire.read();
    i++;
  }
  
  // Transform the data to integer
  int headingValue = headingData[0]*256 + headingData[1];
  return headingValue;
}
