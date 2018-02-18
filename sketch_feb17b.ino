#include <Wire.h>
#include "paj7620.h"

#define GES_REACTION_TIME    400       // You can adjust the reaction time according to the actual circumstance.
#define GES_ENTRY_TIME      800       // When you want to recognize the Forward/Backward gestures, your gestures' reaction time must less than GES_ENTRY_TIME(0.8s). 
#define GES_QUIT_TIME     1000

int LED = 4;
int LED2 = 2;
const int TouchPin = 9;

void setup()
{
  pinMode(LED,OUTPUT);
  pinMode(LED2,OUTPUT);
  pinMode(TouchPin, INPUT);
  
  uint8_t error = 0;

  Serial.begin(9600);
  Serial.println("\nPAJ7620U2 TEST DEMO: Recognize 9 gestures.");

  error = paj7620Init();      // initialize Paj7620 registers
  if (error) 
  {
    Serial.print("INIT ERROR,CODE:");
    Serial.println(error);
  }
  else
  {
    Serial.println("INIT OK");
  }
  Serial.println("Please input your gestures:\n");
}


void loop()
{
    uint8_t data = 0, data1 = 0, data2=0, data3=0,error; 
    static int flag=0, a=0; 
    
    error = paj7620ReadReg(0x43, 1, &data);       // Read Bank_0_Reg_0x43/0x44 for gesture result.
    
loopA: 
    if(!error){
//preset password
      delay(1000);
      paj7620ReadReg(0x43, 1, &data1);
      delay(10000);
      Serial.println("Please input your gesture2:\n");
      paj7620ReadReg(0x43, 1, &data2);
      delay(10000);
      Serial.println("Please input your gesture3:\n");
      paj7620ReadReg(0x43, 1, &data3);

//match the preset password
//      if(data1 == GES_LEFT_FLAG){
//        data1 = GES_LEFT_FLAG;
//        }
//        else if (data1 == GES_RIGHT_FLAG){
//          data1 = GES_RIGHT_FLAG;
//          }
//     
      if(data2 == GES_LEFT_FLAG){
        a = 0;
        }
        else if (data2 == GES_RIGHT_FLAG){
          a = 1;
          }
       
//       if(data3 == GES_LEFT_FLAG){
//        data3 = GES_LEFT_FLAG;
//        }
//        else if (data3 == GES_RIGHT_FLAG){
//          data3 = GES_RIGHT_FLAG;
//          }
          
      delay(100);
      Serial.println("Start");
       paj7620ReadReg(0x43, 1, &data);
      if (data == data1){
        Serial.println("Left");
        delay(50);
        paj7620ReadReg(0x43, 1, &data);
        switch(data)
        {
 tryright:
          case GES_RIGHT_FLAG:
            if(a==0){
              goto tryleft;
              }
            delay(GES_ENTRY_TIME);
            Serial.println("Right");
            flag = flag+1;
           
            paj7620ReadReg(0x43, 1, &data);
            if(data == data3)
            {
                delay(GES_ENTRY_TIME);
                Serial.println("Forward");
                flag= flag+1;
                
              }
             else
             {
              flag = 0;
              Serial.println("Restart");
              goto loopA;            
              }
              break;
 tryleft:             
          case GES_LEFT_FLAG:
            if(a==1){
              goto tryright;
              }
            delay(GES_ENTRY_TIME);
            Serial.println("Right");
            flag = flag+1;
           
            paj7620ReadReg(0x43, 1, &data);
            if(data == data3)
            {
                delay(GES_ENTRY_TIME);
                Serial.println("Forward");
                flag= flag+1;
                
              }
             else
             {
              flag = 0;
              Serial.println("Restart");
              goto loopA;            
              }
              break;
              
          default:
            paj7620ReadReg(0x44, 1, &data);   
             Serial.println("restart");
             flag=0;
             goto loopA;
        
            break;  
          }
      }
//        else{
//          flag=0;
//           goto loopA;
//          }
        
//      if (data == GES_FORWARD_FLAG){
//          Serial.println("Forward");
//          //delay(10);
//          flag = flag+1;
//          Serial.println(flag);
//          
//        }
//      if (data == GES_BACKWARD_FLAG){
//          Serial.println("Backward");
//          flag = flag+1;
//          Serial.println(flag);
//        } 
   
      //REMAIN ------
      if (flag == 2){
          digitalWrite(LED,HIGH);
          
         // delay(2000);
        }

      int sensorValue = digitalRead(TouchPin);
      if (sensorValue == 1){
        digitalWrite(LED2,HIGH);
        flag=0;
        }
        else{
          digitalWrite(LED2,LOW);
          }
      if(flag == 0){
        digitalWrite(LED,LOW);
        }
 
    } 

    int red, green, blue;
    GroveColorSensor colorSensor;
    colorSensor.ledStatus = 1;            // When turn on the color sensor LED, ledStatus = 1; When turn off the color sensor LED, ledStatus = 0.
    while(1)
    {
        colorSensor.readRGB(&red, &green, &blue);    //Read RGB values to variables.
        delay(300);
        Serial.print("The RGB value are: RGB( ");
        Serial.print(red,DEC);
        Serial.print(", ");
        Serial.print(green,DEC);
        Serial.print(", ");
        Serial.print(blue,DEC);
        Serial.println(" )");
        colorSensor.clearInterrupt();
    }
    
}
