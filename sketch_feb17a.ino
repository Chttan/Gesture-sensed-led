#include <Wire.h>
#include "paj7620.h"

#define GES_REACTION_TIME    400       // You can adjust the reaction time according to the actual circumstance.
#define GES_ENTRY_TIME      800       // When you want to recognize the Forward/Backward gestures, your gestures' reaction time must less than GES_ENTRY_TIME(0.8s). 
#define GES_QUIT_TIME     1000

int LED = 4;
int LED2 = 2;
const int TouchPin = 9;
int soundPin = A0;

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
    uint8_t data = 0, data1 = 0, error; 
    static int flag=0, a=0; int val=0;
    
    error = paj7620ReadReg(0x43, 1, &data);       // Read Bank_0_Reg_0x43/0x44 for gesture result.

loopA: 
    a=0;
    val = analogRead(soundPin);
    //Serial.println(val);
    if(val >200){
      a=1;
      }
    Serial.println(a);
    if(!error){
      delay(GES_ENTRY_TIME);
      if (data == GES_LEFT_FLAG){
        Serial.println("Left");
        delay(20);
        paj7620ReadReg(0x43, 1, &data);
        switch(data)
        {
          case GES_RIGHT_FLAG:
            delay(GES_ENTRY_TIME);
            Serial.println("Right");
            flag = flag+1;
           
            paj7620ReadReg(0x43, 1, &data);
            if(data == GES_LEFT_FLAG)
            {
                delay(GES_ENTRY_TIME);
                Serial.println("Forward");
                flag= flag+1;
                
              }
             else
             {
              flag = 0; a=0;
              Serial.println("Restart");
              Serial.println(a);
              goto loopA;            
              }
              break;
          default:
            paj7620ReadReg(0x44, 1, &data1);   
             Serial.println("restart");
             Serial.println(a);
             a=0;
             flag=0;
             goto loopA;
        
            break;  
          }
      }
   
      //REMAIN ------
      if (flag == 2 && a ==1){
          for (int i =0; i<25; i++){
            digitalWrite(LED,HIGH);
            delay(40);
            digitalWrite(LED,LOW);
            }
          a=0;
          Serial.println(a);
         // delay(2000);
        }
      

      int sensorValue = digitalRead(TouchPin);
      if (sensorValue == 1){
        digitalWrite(LED2,HIGH);
        flag=0; a =0;
        }
        else{
          digitalWrite(LED2,LOW);
          }
      if(flag == 0){
        digitalWrite(LED,LOW);
        a=0;
        }
 
    } 
    
}
