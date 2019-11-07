    
      /*
       * DeAGOSTINI Back To The Future Delorean Dash Lights
       */
  
       /*
        * Dash_Lights Version 2.00 by TBoy58 14Oct18
        * Works with BTTF_Delorean_Mega_4.07_release_091218
        */
        
        #include <Wire.h>
        #include "DigitalPin.h"
        
        #define LEDMAX 11
    
        DigitalPin LAMBDA(2);
        DigitalPin OIL(3);
        DigitalPin BATTERY(4);
        DigitalPin FUEL(5);
        DigitalPin BRAKE(6);
        DigitalPin R_BLINK(7);
        DigitalPin MAIN(8);
        DigitalPin F_BEAM(9);
        DigitalPin L_BLINK(10);
        DigitalPin SEAT_BELT(11);
        DigitalPin DOORS(12);
    
    
        DigitalPin D_PIN[LEDMAX] = { LAMBDA,   // 0
                                     OIL,      // 1
                                     BATTERY,  // 2
                                     FUEL,     // 3
                                     BRAKE,    // 4
                                     L_BLINK,  // 5
                                     MAIN,     // 6
                                     F_BEAM,   // 7
                                     R_BLINK,  // 8
                                     SEAT_BELT,// 9
                                     DOORS  };  //10
                                     
    
      
      int dashLightsMax = LEDMAX;
      int dashSelection = 0; 
    
    
    
      void setup() {
        
           LAMBDA.beginOUT();
           OIL.beginOUT();
           BATTERY.beginOUT();
           FUEL.beginOUT();
           BRAKE.beginOUT();
           R_BLINK.beginOUT();
           MAIN.beginOUT();
           F_BEAM.beginOUT();
           L_BLINK.beginOUT();
           SEAT_BELT.beginOUT();       
           DOORS.beginOUT();
      
          Serial.begin(9600);      
      
          Wire.begin(8);                // join i2c bus with address #8
          Wire.onReceive(receiveEvent); // register event
             
      }
      
      void loop() {
        // put your main code here, to run repeatedly:
      
      }
      
      void allLightsOn()
      {  
        for(int d = 0; d < dashLightsMax; d++)
        {
          D_PIN[d].on();
        }  
      }
      
      void allLightsOff()
      {   
        for(int d = 0; d < dashLightsMax; d++)
        {
          D_PIN[d].off();
        } 
      }
      
      
      void receiveEvent() {
      byte c;
        Serial.println("Receive");
      
        while (Wire.available()) { // slave may send less than requested
          c = Wire.read(); // receive a byte as character
        }
        dashSelection = c;
        switch (dashSelection)
        {
          case 0: // LIGHTS OFF
                  allLightsOff();
                  break;
  
          case 1: // LIGHTS ON
          
                  allLightsOn();
                  
                  break;
                  
          case 2: // LEFT INDICATOR ON
                  
                  L_BLINK.on();               
                  
                  break;
                  
          case 3: // LEFT INDICATOR OFF
          
                  L_BLINK.off();
                  
                  break;
                  
          case 4: // MAIN LIGHTS ON
          
                  MAIN.on();
                  
                  break;
  
          case 5: // MAIN LIGHTS OFF
                  
                  MAIN.off();
                  
                  break;
                  
          case 6: // FULL BEAM ON
          
                  F_BEAM.on();
                  
                  break;
  
          case 7: // FULL BEAM OFF
          
                  F_BEAM.off();
                  
                  break;
                  
          case 8: // RIGHT INDICATOR ON
                  
                  R_BLINK.on();
                  
                  break;
  
          case 9: // RIGHT INDICATOR OFF
                  
                  R_BLINK.off();
                  
                  break; 
          
                  
          case 10: // DOORS OPEN
                  
                  DOORS.on();             
                  
                  break;
  
          case 11: // DOORS CLOSED
      
                  DOORS.off();
        
                  break;    
          
      
          case 12: // HAZARD LIGHTS ON
                  
                  L_BLINK.on();
                  R_BLINK.on();
                                    
                  break; 
  
          case 13: // HAZARD LIGHTS OFF
                  
                  L_BLINK.off();
                  R_BLINK.off();
                  
                  break;        
      
          case 14: // PASSING LIGHTS ON
                 
                  MAIN.on();
                  F_BEAM.on();
                    
                  break;
  
           case 15: // PASSING LIGHTS OFF
                 
                  MAIN.off();
                  F_BEAM.off();
                    
                  break;       
          
          default:
                   break;
        }      
      }



    
