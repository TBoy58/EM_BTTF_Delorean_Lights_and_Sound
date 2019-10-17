  /*
   * BTTF Delorean Reactor Lights by TBoy58
   * Simulates reactor operation when Time Circuits are on
   * and during time travel
   */
  
  #include <Adafruit_NeoPixel.h>
  #include <Wire.h>

  #define I2C_ADDRESS 7

  #define PIN 6
  #define MAX_NEOPIXELS 12
  #define MAX_FLUX_LEVEL 7
  #define RE_ENTRY 10

  int BRIGHTNESS[MAX_FLUX_LEVEL] =    { 0, 10, 20, 40, 40, 60, 90};
  int REACTOR_SPEED[MAX_FLUX_LEVEL] = {80, 80, 60, 50, 40, 30, 20};
  
  int fluxLevel = 0;

  uint32_t color;

  bool isReentry = false;
  

  /*
   *   Parameter 1 = number of pixels in reactorRing
    // Parameter 2 = Arduino pin number (most are valid)
    // Parameter 3 = pixel type flags, add together as needed:
    // NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
    // NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
    // NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
    // NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
    // NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
   */

   Adafruit_NeoPixel reactorRing = Adafruit_NeoPixel(MAX_NEOPIXELS, PIN, NEO_GRB + NEO_KHZ800);

    
   /********************************************************************************
    * =======================SETUP===============SETUP==============================
    ********************************************************************************/
  
    void setup() {      
      reactorRing.begin();
      reactorRing.setBrightness(BRIGHTNESS[fluxLevel]);
      reactorRing.show(); // Initialize all pixels to 'off'   

      color = reactorRing.Color(255, 85, 0);

      Wire.begin(I2C_ADDRESS);       // join i2c bus with address
      Wire.onReceive(receiveEvent); // register event
       
    }

    /********************************************************************************
    * =======================LOOP===============LOOP==============================
    ********************************************************************************/    
    void loop() {
      if(isReentry){
        reentryFlash();
      }
      else {
        reactorRing.setBrightness(BRIGHTNESS[fluxLevel]);
        theaterChase(color, REACTOR_SPEED[fluxLevel]); // Orange  
      }
    }
  
    //Theatre-style crawling lights.
    void theaterChase(uint32_t c, uint8_t wait) {
      for (int j=0; j<10; j++) {  //do 10 cycles of chasing
        for (int q=0; q < 3; q++) {
          for (uint16_t i=0; i < reactorRing.numPixels(); i=i+3) {
            reactorRing.setPixelColor(i+q, c);    //turn every third pixel on
          }
          reactorRing.show();
    
          delay(wait);
    
          for (uint16_t i=0; i < reactorRing.numPixels(); i=i+3) {
            reactorRing.setPixelColor(i+q, 0);        //turn every third pixel off
          }
        }
      }
    }


     void reentryFlash() {

      delay(250);
      
      reactorRing.setBrightness(90);
    
      for (int i = 0; i < 3; i++) {
        
        for(uint16_t i=0; i<reactorRing.numPixels(); i++) {
          reactorRing.setPixelColor(i, color);         
        }
        reactorRing.show();
    
        delay(200);
    
        for(uint16_t i=0; i<reactorRing.numPixels(); i++) {
          reactorRing.setPixelColor(i, 0);         
        }
        reactorRing.show();
    
        delay(400);
      }

      isReentry = false;      
    }

    void receiveEvent() {
      byte c;
      while (Wire.available()) { // slave may send less than requested
        c = Wire.read(); // receive a byte as character
      }
      if(c == RE_ENTRY){
        isReentry = true;
      }
      else
      {
        fluxLevel = c;
      }
    }
