# AS5047
Code for interfacing with AS5047

## Example

```C++
#include "AS5047.h"

#define ENC_1_SELECT 10 // pin 10 is used for selecting the first encoder
#define ENC_2_SELECT 13 // pin 13 is used for selecting the seconds encoder

tAS5047 encoder1 = { .selectPin = ENC_1_SELECT };
tAS5047 encoder2 = { .selectPin = ENC_2_SELECT };

void setup() {
  Serial.begin(9600);
  // Initialize SPI, not needed if SPI is initialized manually
  //    Datasheet
  //    SPI - order MSBFIRST
  //        - mode  SPI_MODE_1
  AS5047_SPI_Init();  

  // Initialize the encoders (basically sets the select pin mode to OUTPUT and the current value to HIGH)
  //  Can be done manually
  AS5047_Init(encoder1);
  AS5047_Init(encoder2);
}

void loop() {
  // The encoder data field will be populated with the value from the last command
  //    That field is a union of all registers, so depending on the command you can access the needed register and all the fields inside
  //    The data field register names and the field names are the same as in the datasheet
  // The command format is CMD_R_<register name>
  
  // Read some values from the first encoder
  AS5047_Read(encoder1, CMD_R_ANGLECOM);
  Serial.print("1. Angle = ");Serial.println(encoder1.data.ANGLECOM.DAECANG);
  AS5047_Read(encoder1, CMD_R_DIAAGC);
  Serial.print("1. AGC = ");Serial.println(encoder1.data.DIAAGC.AGC);
  
  // Read some values from the second encoder
  AS5047_Read(encoder2, CMD_R_ANGLECOM);
  Serial.print("2. Angle = ");Serial.println(encoder2.data.ANGLECOM.DAECANG);
  AS5047_Read(encoder2, CMD_R_DIAAGC);
  Serial.print("2. AGC = ");Serial.println(encoder2.data.DIAAGC.AGC);
  
  delay(1000);
}
```
