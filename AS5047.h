//***************************************************************************
//
//  AS5047.h - Library for working with AS5047.
//  Created by Razvan Marinovici, 27 - March - 2016.
//  Released into the public domain.
//
//***************************************************************************
#ifndef _AS5047_LIB_INCLUDED
#define _AS5047_LIB_INCLUDED

#include <SPI.h>

#define DATA_MASK         0x3FFF

#define OP_WRITE 0x0000
#define OP_READ  0x4000

#define PARITY_EVEN 0x0000
#define PARITY_ODD  0x8000

#define ADDR_NOP          0x0000
#define ADDR_ERRFL        0x0001
#define ADDR_PROG         0x0003
#define ADDR_DIAAGC       0x3FFC
#define ADDR_MAG          0x3FFD
#define ADDR_ANGLEUNC     0x3FFE
#define ADDR_ANGLECOM     0x3FFF
#define ADDR_SETTINGS1    0x0018
#define ADDR_SETTINGS2    0x0019

#define CMD_R_NOP       (PARITY_ODD  | OP_READ | ADDR_NOP)      
#define CMD_R_ERRFL     (PARITY_EVEN | OP_READ | ADDR_ERRFL)    
#define CMD_R_PROG      (PARITY_ODD  | OP_READ | ADDR_PROG)     
#define CMD_R_DIAAGC    (PARITY_ODD  | OP_READ | ADDR_DIAAGC)   
#define CMD_R_MAG       (PARITY_EVEN | OP_READ | ADDR_MAG)      
#define CMD_R_ANGLEUNC  (PARITY_EVEN | OP_READ | ADDR_ANGLEUNC) 
#define CMD_R_ANGLECOM  (PARITY_ODD  | OP_READ | ADDR_ANGLECOM) 
#define CMD_R_SETTINGS1 (PARITY_ODD  | OP_READ | ADDR_SETTINGS1)
#define CMD_R_SETTINGS2 (PARITY_EVEN | OP_READ | ADDR_SETTINGS2)

typedef struct stERRFL {
  uint8_t FRERR   :1;
  uint8_t INVCOMM :1;
  uint8_t PARERR  :1;
};

typedef struct stPROG {
  uint8_t PROGEN  :1;
  uint8_t unused  :1;
  uint8_t ORPREF  :1;
  uint8_t PROGOTP :1;
  uint8_t unused2 :2;
  uint8_t PROGVER :1;
};

typedef struct stDIAAGC {
  uint8_t AGC     :8;
  uint8_t LF      :1;
  uint8_t COF     :1;
  uint8_t MAGH    :1;
  uint8_t MAGL    :1;
};

typedef struct stMAG {
  uint16_t CMAG   :14;
};

typedef struct stANGLECOM {
  uint16_t DAECANG : 14;
};

typedef struct stANGLEUNC {
  uint16_t CORDICANG : 14;
};

typedef struct stReadFlags {
      uint16_t unused :14;
      uint8_t EF      :1;
      uint8_t PARD    :1;
};

// Thank's to union this will be uint16_t so 2 bytes
typedef union uDataMap {
    stERRFL     ERRFL;
    stPROG      PROG;
    stDIAAGC    DIAAGC;
    stMAG       MAG;
    stANGLECOM  ANGLECOM;
    stANGLEUNC  ANGLEUNC;
    stReadFlags flags;
    uint16_t    raw; 
};

typedef struct tAS5047 {
  uint8_t selectPin;
  uDataMap data;
};

void AS5047_SPI_Init(uint8_t rate = SPI_CLOCK_DIV4){
    // SPI Interface Page 10 - AD5047
  SPI.setDataMode(SPI_MODE1);
  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(rate);
  SPI.begin();
}

void AS5047_Init(tAS5047& ic){
  pinMode(ic.selectPin, OUTPUT);
  digitalWrite(ic.selectPin, HIGH);
}

void AS5047_Read(tAS5047& ic, uint16_t command){
  // First frame of SPI - send a 16 bit command
  digitalWrite(ic.selectPin, LOW);
  SPI.transfer(command >> 8);
  SPI.transfer(command & 0xFF);
  digitalWrite(ic.selectPin, HIGH);

  // Second frame of SPI - read the result of the command
  digitalWrite(ic.selectPin, LOW);
  ic.data.raw = SPI.transfer(CMD_R_NOP) << 8;
  ic.data.raw |= SPI.transfer(CMD_R_NOP);
  digitalWrite(ic.selectPin, HIGH);
}

#endif
