/*
  Library examples for TM1638.

  Copyright (C) 2011 Ricardo Batista <rjbatista at gmail dot com>

  This program is free software: you can redistribute it and/or modify
  it under the terms of the version 3 GNU General Public License as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

// Library from https://github.com/rjbatista/tm1638-library
#include <TM1638.h>
// Library from https://github.com/k3ng/k3ng_analog_devices_dds
#include <dds.h>

#define AD9851_DATA 6
#define AD9851_LOAD 5
#define AD9851_CLK 4

#define TM1638_DATA 8
#define TM1638_CLOCK 9
#define TM1638_STROBE 7

unsigned long display = 0;
unsigned long olddisplay = 0;
unsigned long _1 = 0;
unsigned long _10 = 0;
unsigned long _100 = 0;
unsigned long _1000 = 0;
unsigned long _10000 = 0;
unsigned long _100000 = 0;
unsigned long _1000000 = 0;
unsigned long _10000000 = 0;
byte oldkeys;
byte points;
//unsigned int waittime = 0;;
unsigned int dontchange = 0;
boolean power = false;
byte keyscount = 0;
byte command = 0;

// define a module on data pin 8, clock pin 9 and strobe pin 7
TM1638 tm1638(TM1638_DATA, TM1638_CLOCK, TM1638_STROBE);
dds ad9851(DDS9851, AD9851_DATA, AD9851_LOAD, AD9851_CLK, 180000000LL);

void setup() {
  // display a hexadecimal number and set the left 4 dots
  tm1638.setDisplayToHexNumber(display, 0, true);
  ad9851.set_clock_multiplier(1);
  ad9851.setfrequency(0);
  Serial.begin(9600);
}

void loop() {
  byte keys = tm1638.getButtons();

  if ( keys != 0 ) {
    if ( keys ==  oldkeys ) {
      keyscount ++;
    } else {
      keyscount = 0;
    }
  }

  if ( keyscount == 150 && dontchange == 0 ) {
    command = keys;
    keyscount = 0;
    dontchange = 400;
    Serial.println(command);
  } else {
    command = 0;
  }

  switch (command) {
    case 129:
      if ( display != 0) {
        if ( power == false) {
          tm1638.setLEDs(255);
          power = true;
          ad9851.setfrequency(display);
        } else {
          tm1638.setLEDs(0);
          power = false;
          ad9851.setfrequency(0);
        }
      }
      break;
    case 24:
         _1 = _10 = _100 = _1000 = _10000 = _100000 = _1000000 = _10000000 = 0;
         power = false;
         tm1638.setLEDs(0);
         break;
    case 1:
      if ( !power) {
        _10000000 += 10000000;
        if ( _10000000 > 90000000 ) {
          _10000000 = 0;
        }
      }
      break;
    case 2:
      if ( !power) {
        _1000000 += 1000000;
        if ( _1000000 > 9000000 ) {
          _1000000 = 0;
        }
      }
      break;
    case 4:
      if ( !power) {
        _100000 += 100000;
        if ( _100000 > 900000 ) {
          _100000 = 0;
        }
      }
      break;
    case 8:
      if ( !power) {
        _10000 += 10000;
        if ( _10000 > 90000 ) {
          _10000 = 0;
        }
      }
      break;
    case 16:
      if ( !power) {
        _1000 += 1000;
        if ( _1000 > 9000 ) {
          _1000 = 0;
        }
      }
      break;
    case 32:
      if ( !power) {
        _100 += 100;
        if ( _100 > 900 ) {
          _100 = 0;
        }
      }
      break;
    case 64:
      if ( !power) {
        _10 += 10;
        if ( _10 > 90 ) {
          _10 = 0;
        }
      }
      break;
    case 128:
      if ( !power) {
        _1++;
        if ( _1 > 9 ) {
          _1 = 0;
        }
      }
      break;
  }

  display = _1 + _10 + _100 + _1000 + _10000 + _100000 + _1000000 + _10000000;

  if (olddisplay != display ) {
    if (display) {
      if (display < 1000 ) {
        points = 0;
      } else if ( display < 1000000 ) {
        points = 8;
      } else {
        points = 72;
      }
      tm1638.setDisplayToDecNumber(display, points, false);
    } else {
      tm1638.setDisplayToDecNumber(display, 0 , true);
      points = 0;
    }
  }

  oldkeys = keys;

  if (dontchange > 0) {
    dontchange--;
  }
  olddisplay = display;
}
