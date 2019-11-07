    /*
     * BTTF DeAGOSTINI DELOREAN LIGHT AND SOUND SYSTEM - TBoy58
     * Based on:
      DeAGOSTINI REAR_LIGHTS To The Furture Delorian Light & Sound Control Mega2560 V1.10 By LsKaz
      2018/04/08 V1.09 ADD door open sound 
      2018/04/28 V1.10 ADD door LED2(D10),change hood LED pin(D11) 

      ===============GITHUB COPY===================================================
    */
    
   
    /*
     * Version 4.07 - TBoy58 24Oct18
     * Works with Dash_Lights_2.00_release_241018
     */
    
    #include <DFPlayer_Mini_Mp3.h>
    #include <Wire.h>
    #include <DigitalPin.h>
    #include <Adafruit_GFX.h>
    #include <Adafruit_LEDBackpack.h>  
    #include <AltSoftSerial.h>  

    AltSoftSerial assSerial;

    #define I2C_FIRE_ADDRESS 9
    #define I2C_DASH_ADDRESS 8
    #define I2C_REACTOR_ADDRESS 7
    
    #define Ready_LED 22        // 受信可能赤外線モジュールPIN (NOT A PIN)       
    #define PINMAX 22       // シフトレジスタービット数
    #define LEDMAX 24       // LED定義数
    #define INDMAX 4        // Maximum indicator lights
    #define MAXINTERVALS 33
    #define SW_FUNCS 22

    #define RE_ENTRY 10

    // DASH LIGHT DEFINITIONS
    #define LIGHTS_OFF            0
    #define LIGHTS_ON             1
    #define LEFT_INDICATOR_ON     2
    #define LEFT_INDICATOR_OFF    3
    #define MAIN_LIGHTS_ON        4
    #define MAIN_LIGHTS_OFF       5
    #define FULL_BEAM_ON          6
    #define FULL_BEAM_OFF         7
    #define RIGHT_INDICATOR_ON    8
    #define RIGHT_INDICATOR_OFF   9 
    #define DOOR_OPEN            10
    #define DOORS_CLOSED         11    
    #define HAZARD_LIGHTS_ON     12
    #define HAZARD_LIGHTS_OFF    13
    #define PASSING_LIGHTS_ON    14
    #define PASSING_LIGHTS_OFF   15
    
    

    // INITIALISE THE BOARD PINS 
    DigitalPin L_SIDE(2);    // Left front amber side-light and indicator
    DigitalPin R_SIDE(3);    // Right front amber side-light and indicator    
    
    DigitalPin TC_OFF(16);      // Time circuits OFF light (Power ON)
    DigitalPin DFP_BUSY_LED(17);  // Radio LED
    
    DigitalPin DFP_BUSY(22);
    DigitalPin DIPPED_LIGHTS(23);
    DigitalPin F_BEAM(24);
    DigitalPin REAR_LIGHTS(25);
    DigitalPin BRAKE_LIGHTS(26);
    DigitalPin REVERSE_LIGHTS(27);

    DigitalPin RED_AMBER_GREEN_KEYPAD(28);
    DigitalPin WHITE_KEYPAD(29);

    DigitalPin PLATE_LIGHTS(30); // Licence plate lights
       
    DigitalPin TC_ON(32);       // Center console time circuits ON light
    DigitalPin TCD(33);         // Time circuits display
    DigitalPin EIGHTY8_MPH(34); // 88mph indicator
    DigitalPin P_EMPTY(35);    // Plutonium empty light 
    DigitalPin DASH_BACKLIGHT(36);  // Driver's dash backlight 
    DigitalPin PSU(37);       // Center console power supply unit light
    DigitalPin PLUTONIUM_BACKLIGHT(38); // Plutonium dash back-light
    
    DigitalPin L_BLINK(39);   // Left rear indicator
    DigitalPin R_BLINK(40);   // Right rear indicator

    DigitalPin COURTESY_LIGHT(47);
    DigitalPin ENGINE_SPACE_LIGHT(49); 
    DigitalPin DOOR_1_LIGHTS(50);
    DigitalPin DOOR_2_LIGHTS(51);
    DigitalPin FRONT_HOOD_LIGHT(52);    
    DigitalPin ROOF_LIGHT(53);  

       
         
    DigitalPin BRAKE_SW(54);          // A0
    DigitalPin ACCELERATOR_SW(56);    // A2
    DigitalPin DOOR_SW_ONE(60);       // A6
    DigitalPin DOOR_SW_TWO(64);       // A10
    DigitalPin TRUNK_SW(68);          // A14
    DigitalPin ENGINE_SPACE(69);      // A15
   

    DigitalPin D_PIN[LEDMAX] = { L_BLINK,                // 0
                                 R_BLINK,                // 1
                                 BRAKE_LIGHTS,           // 2
                                 REVERSE_LIGHTS,         // 3
                                 L_SIDE,                 // 4
                                 R_SIDE,                 // 5
                                 REAR_LIGHTS,            // 6
                                 PLATE_LIGHTS,           // 7
                                 DIPPED_LIGHTS,          // 8
                                 F_BEAM,                 // 9 
                                 COURTESY_LIGHT,         //10  
                                 DOOR_1_LIGHTS,          //11
                                 DOOR_2_LIGHTS,          //12
                                 ROOF_LIGHT,             //13
                                 FRONT_HOOD_LIGHT,       //14
                                 TC_OFF,                 //15
                                 TC_ON,                  //16
                                 TCD,                    //17
                                 RED_AMBER_GREEN_KEYPAD, //18
                                 EIGHTY8_MPH,            //19
                                 P_EMPTY,                //20
                                 DASH_BACKLIGHT,         //21
                                 PSU,                    //22                                               
                                 PLUTONIUM_BACKLIGHT };  //23   

                                
                                 
    DigitalPin IND_LGHTS[INDMAX] = { L_BLINK,      // 0
                                     R_BLINK,      // 1
                                     L_SIDE,       // 2
                                     R_SIDE };     // 3


  // ===APP MESSAGE DEFINITIONS===
    byte PWR_OFF_MSG            = 100;
    byte PWR_ON_MSG             = 101;
    byte VOL_UP_OFF_MSG         = 102;
    byte VOL_UP_ON_MSG          = 103;
    byte RAD_NEXT_OFF_MSG       = 104;
    byte RAD_NEXT_ON_MSG        = 105;
    byte TM_CCT_OFF_MSG         = 106;
    byte TM_CCT_ON_MSG          = 143; // FOR SOME REASON 107 IS RECEIVED BY APP AS 251
    byte MOV_DEMO_OFF_MSG       = 108;
    byte MOV_DEMO_ON_MSG        = 109;
    byte RAD_PREV_OFF_MSG       = 110;
    byte RAD_PREV_ON_MSG        = 111;
    byte LGHTS_ON_OFF_MSG       = 112;
    byte LGHTS_ON_ON_MSG        = 113;
    byte VOL_DN_OFF_MSG         = 114;
    byte VOL_DN_ON_MSG          = 115;
    byte LGHTS_OFF_OFF_MSG      = 116;
    byte LGHTS_OFF_ON_MSG       = 117;
    byte RF_LGHT_OFF_MSG        = 118;
    byte RF_LGHT_ON_MSG         = 119;
    byte PSS_LGHT_OFF_MSG       = 120;
    byte PSS_LGHT_ON_MSG        = 121;
    byte HORN_OFF_MSG           = 122;
    byte HORN_ON_MSG            = 123;
    byte ENG_OFF_MSG            = 124;
    byte ENG_ON_MSG             = 145; // FOR SOME REASON 125 DOES NOT WORK
    byte BRAKE_LIGHTS_OFF_MSG   = 126;
    byte BRAKE_LIGHTS_ON_MSG    = 127;
    byte ACCEL_OFF_MSG          = 128;
    byte ACCEL_ON_MSG           = 129;
    byte LFT_IND_OFF_MSG        = 130;
    byte LFT_IND_ON_MSG         = 131;
    byte HZD_LGHT_OFF_MSG       = 132;
    byte HZD_LGHT_ON_MSG        = 133;
    byte RGT_IND_OFF_MSG        = 134;
    byte RGT_IND_ON_MSG         = 135;
    byte REV_LGHT_OFF_MSG       = 136;
    byte REV_LGHT_ON_MSG        = 137;
    byte TM_TRAV_OFF_MSG        = 138;
    byte TM_TRAV_ON_MSG         = 139;
    byte RADIO_STOP_MSG         = 140;
    byte RADIO_PLAY_MSG         = 141;

    boolean radioOn                = false;
    boolean timeCctsOn             = false;
    boolean engineStartOn          = false;
    boolean headLightsOn           = false;
    boolean leftIndicatorsOn       = false;
    boolean rightIndicatorsOn      = false;
    boolean hazardIndicatorsOn     = false;
    boolean brakeLightsOn          = false;
    boolean reverseLightsOn        = false;
    boolean roofLightOn            = false;


    
                            //     0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17  18  19  20  21
                            //   
    bool    SW_LED[INDMAX]  = {    0,  0,  0,  0}; // LED点灯スイッチ
    int     IND_INTERVAL[INDMAX] = {    45, 45,  0,  0}; // LED点灯時間(0:常灯)    
    bool    SW_On[INDMAX]   = {};          // LED点灯（0:消灯中 1:点灯中）
    int     SW_ST[LEDMAX]   = {};          // LED点灯ステータス（0:消灯中 1:点灯中）
    int     FLASH_DELAY[INDMAX]  = {};          // LEDカウンタ

    
    boolean OLD_Door1=0;
    boolean OLD_Door2=0;
    boolean NEW_Door1=0;
    boolean NEW_Door2=0;
    boolean doorsClosed = true;
    boolean indicatorsOn = false;
    boolean appBrake = false;

    boolean SYSTEM_ON = false;

    /*===========================================================MOVIE DEMO TIME TRAVEL INTERVAL SETTINGS============================================================================================*/
     //                                          1                                     2           3          4         5          6          7          8
    long INTERVAL[MAXINTERVALS] = { 3000, 3000, 1000,  500, 11000, 3000, 4000, 2000, 25500, 500, 2000, 500, 5200, 500, 300, 500, 1000, 500, 2500, 500, 3000, 2000, 100, 12000, 500, 600, 600, 1500, 6500, 1000,500,400,100};


    
    /*===========================================================TIME TRAVEL INTERVAL SETTINGS============================================================================================*/
    //                                              1                           2           3         4           5          6          7     8
    long B8_INTERVAL[MAXINTERVALS] = { 500, 500, 2500,  1000, 500, 1000, 1000, 1500, 500, 2000, 500, 3500, 500, 2200, 3200, 300, 500, 3500, 2500, 1000, 500, 1500, 1500, 400, 400, 100, 0,0,0,0,0,0};
         
    boolean STOP = false;
    boolean first = true;
    boolean second = false;
    boolean third = false;
    boolean fourth = false;
    boolean fifth = false;
    boolean fifth_b = false;
    boolean fifth_c = false;
    boolean sixth = false;
    boolean seventh = false;
    boolean speedoOutOpen = false;
    boolean eighth = false;
    boolean ninth = false;
    boolean tenth = false;
    boolean eleventh = false;
    boolean twelfth = false;
    boolean thirteenth = false;
    boolean fourteenth = false;
    boolean fifteenth = false;
    boolean sixteenth = false;
    boolean seventeenth = false;
    boolean eighteenth = false;
    boolean nineteenth = false;
    boolean twentieth = false;
    boolean twentyfirst = false;
    boolean twentysecond = false;
    boolean fluxflash = false;
    boolean twentythird = false;
    boolean twentyfourth = false;
    boolean twentyfifth = false;
    boolean twentysixth = false;
    boolean twentyseventh = false;
    boolean twentyeighth = false;
    boolean twentyninth = false;
    boolean thirtieth = false;
    
    unsigned long currentMillis; // Current value of millis() clock
    unsigned long currentMicros; // Current value of micros() clock
           
    
    long previousTtMillis = 0;
    long previousSpeedoMicros = 0;
    long previousSpeedoReturnMicros = 0;
    long previousFlashMillis = 0;
    int j = 0; // Access for INTERVAL and B8_INTERVAL arrays
    int pEmptyLoopCount = 0; 
    
    Adafruit_7segment speedo = Adafruit_7segment();
    long speedoIntervalSettingOne = 3765;
    long speedoIntervalSettingTwo = 4200;
    long speedoInterval = 0;
    long speedoReturnInterval = 400;
    long B8_speedoIntervalSetting = 6500;
    boolean speedoOut = false;
    boolean speedoReturn = false;
    uint16_t counter = 0;    
    
    long flashInterval = 5; // Indicator flash routine interval

    /*===FLUX CAPACITOR PARAMETERS===*/
    DigitalPin FLUX_BAND(7);
    DigitalPin FC_ONE(8);
    DigitalPin FC_TWO(9);
    DigitalPin FC_THREE(10);
    DigitalPin FC_FOUR(11);
    DigitalPin FC_MID(12);

    int fluxLevel = 0;
    int LedSw  = 1; // Controls which group of flux capacitor elements illuminates

    //                         0    1    2    3    4    5    6    7    8    Flux Level
    int LedBright[9]     = {   0,  10,  10,  10,  20,  50, 100, 180, 255}; // Flux capacitor brightness level
    long fluxInterval[9] = { 150, 150, 100,  90,  80,  70,  20,  20,  10}; // Flux capacitor flash rate

    long previousFluxMillis = 0;

    boolean fluxSwitch = false;

    int s4CountMax = 45;
    int s4Count = 0;

    /*===END FLUX CAPACITOR PARAMETERS===*/

     /*===XMAS TREE PARAMETERS===*/
     Adafruit_8x16matrix matrixOne = Adafruit_8x16matrix();

    
     static const uint8_t PROGMEM fill_bmp[] =
                                      { B11111111,
                                        B11111111,
                                        B11111111,
                                        B11111111,
                                        B11111111,
                                        B11111111,
                                        B11111111,
                                        B11111111,
                                        B11111111,
                                        B11111111,
                                        B11111111,
                                        B11111111,
                                        B11111111,
                                        B11111111,
                                        B11111111,
                                        B11111111
                                      };

    int i; // Access for columns of matrixColumn

    int RndStart[9] =  {   0,   0,   0,   1,   3,   5,   7,  11,  13}; // Minimum end of line value
    int RndEnd[9]   =  {   0,   3,   4,   6,   8,  10,  12,  14,  16}; // Maximum end of line value
    
    long matrixInterval_1 = 150; 
    long matrixInterval_2 = 60;
    long previousLevelOneMatrixMillis = 0;
    long previousOneSevenMatrixMillis = 0;
    long previousThreeSixMatrixMillis = 0;
    long previousZeroFourMatrixMillis = 0;
    long previousTwoFiveMatrixMillis = 0;

    boolean matrixSwitch_1 = true;
    boolean matrixSwitch_2 = true;
    boolean matrixSwitch_3 = false;
    boolean matrixSwitch_4 = false;
    boolean matrixSwitch_5 = false;

    // Columns on Xmas Tree
    int8_t colZero  = 0;
    int8_t colOne   = 0;
    int8_t colTwo   = 0;
    int8_t colThree = 0;
    int8_t colFour  = 0;
    int8_t colFive  = 0;
    int8_t colSix   = 0;
    int8_t colSeven = 0;

    int8_t matrixColumn[5][8] = {{ 0, 4, 0, 0, 7, 0, 0, 0},       // Level 2
                                  { 7, 6, 0, 6, 7, 0, 7, 7},      // Level 3
                                  { 7, 8, 0, 7, 7, 2, 7, 7},      // Level 4
                                  { 7, 15, 0, 9, 7, 6, 10, 15},   // Level 5
                                  {13, 15, 6, 15, 15, 8, 15, 15}  // Level 6
                                };

    int colOneSevenCntMax = 5;
    int colOneSevenCnt = 0;
    int colZeroFourCntMax = 10;
    int colZeroFourCnt = 0;
    int colThreeSixCntMax = 1;
    int colThreeSixCnt = 0;
    int colTwoFiveCntMax = 2;
    int colTwoFiveCnt = 0;
    
          
    /*===END XMAS TREE PARAMETERS===*/

    
    
    // MP3再生用
    #define MP3_CFolder 240  //フォルダー指定コマンド
    #define MP3_CPlay   236  //再生コマンド
    #define MP3_CStop   239  //停止コマンド
    #define MP3_CVol    200  //音量コマンド
    int MP3_Vol    = 17;     //MP3音量15
    int MusicST    = 0;      //音楽再生
    //===RADIO========================
    #define FIRST_TRACK  21
    #define LAST_TRACK   38
    int currentTrack = 0;
    int trackNumber = 0;
    

/*==============================================================================
 *      MP3 ROUTINES          MP3 ROUTINES            MP3 ROUTINES        
 *==============================================================================*/       
    
    //MP3再生終わり待ち
    void my_mp3_playend() 
    {   
      boolean play_state = DFP_BUSY.readState();
      do {play_state = DFP_BUSY.readState(); } 
      while(play_state == LOW);
    }
    
    //MP3再生
    void my_mp3_play(int F,int N)
    {
      mp3_play(N);
      delay(200);
    } 
    
    //MP3停止
    void my_mp3_stop()
    {
      mp3_stop();
      delay(100);
    } 
    
    //MP3音量
    void my_mp3_vol(int V)
    {  
       mp3_set_volume(V);
       delay(100);
    } 
    
    //MP3音量アップ
    void my_mp3_volup()
    {
      assSerial.write(VOL_UP_ON_MSG);
      if (MP3_Vol++ > 29) {MP3_Vol=30;}
      mp3_set_volume(MP3_Vol);
      delay(1000);
      assSerial.write(VOL_UP_OFF_MSG);
      assSerial.write(MP3_Vol);
    } 
    
    //MP3音量ダウン
    void my_mp3_voldown()
    {
      assSerial.write(VOL_DN_ON_MSG);
      if (MP3_Vol > 0) {MP3_Vol--;}
      mp3_set_volume(MP3_Vol);
      delay(1000);
      assSerial.write(VOL_DN_OFF_MSG);
      assSerial.write(MP3_Vol);
    } 

/*==============================================================================
 *      RADIO ROUTINES            RADIO ROUTINES        
 *==============================================================================*/    

    void radioPlay()
    {
      radioOn = true;
      DFP_BUSY_LED.on();
      assSerial.write(RADIO_PLAY_MSG);
      assSerial.write(trackNumber);
      my_mp3_play(1, currentTrack);
    }

    void radioStop()
    {      
      my_mp3_stop();
      assSerial.write(RADIO_STOP_MSG);
      radioOn = false;
      DFP_BUSY_LED.off();
    }

    void radioNext()
    {      
      trackNumber++;
      currentTrack++;
      if( currentTrack > LAST_TRACK )
      {
        currentTrack = FIRST_TRACK;
        trackNumber = currentTrack - 20;
      }
      assSerial.write(RAD_NEXT_ON_MSG);
      assSerial.write(trackNumber);
      my_mp3_play(1, currentTrack);
      delay(1000);
      assSerial.write(RAD_NEXT_OFF_MSG);
      
      
    }

    void radioPrev()
    {
      trackNumber--;
      currentTrack--;
      if( currentTrack < FIRST_TRACK )
      {
        currentTrack = LAST_TRACK;
        trackNumber = currentTrack - 20;
      }
      assSerial.write(RAD_PREV_ON_MSG);
      assSerial.write(trackNumber);
      my_mp3_play(1, currentTrack);
      delay(1000);
      assSerial.write(RAD_PREV_OFF_MSG);
    }

//===========END RADIO FUNCTIONS===================================


/*==============================================================================
 *      INDICATOR FLASH ROUTINE            INDICATOR FLASH ROUTINE        
 *==============================================================================*/    
    
    // タイマー割り込み時に処理される関数
    void indicatorFlash() {  
      /*     
       *      Routine to make indicators flash 
       *      
       */
      
      for( int i = 0; i < INDMAX; i++ )
      {
      if (IND_INTERVAL[i] > 0) {     // LED点滅処理
         FLASH_DELAY[i]--;
         if (SW_On[i] == true)      // LED点灯中
            {
            if (FLASH_DELAY[i] <= 0) 
               { 
                  SW_On[i]  = false;
                  SW_On[i + 2]  = false;
                  FLASH_DELAY[i] = IND_INTERVAL[i]; //FLASH_INTERVAL;                  
               }
            } 
            else 
            {           // LED消灯中
               if (FLASH_DELAY[i] <= 0)
               { 
                  SW_On[i]  = true;
                  SW_On[i + 2]  = true;
                  FLASH_DELAY[i] = IND_INTERVAL[i]; //FLASH_INTERVAL;                  
               }
          }
      } else {SW_On[i] = true;}   // LED常灯処理    
      
       if ((SW_On[i] == true) && (SW_LED[i] == true)) {
            if (i == 2 || i == 3)
                {
                  if(i == 2)
                  {
                    if((SW_On[0] == true) && (SW_LED[0] == true))
                    {
                      L_SIDE.on();
                      sendDashLight(LEFT_INDICATOR_ON);
                    }
                    else
                    {
                      if(SW_ST[3] == 0)
                      {
                        L_SIDE.off();
                        sendDashLight(LEFT_INDICATOR_OFF);
                      }
                      else
                      {
                        L_SIDE.level(80);
                        sendDashLight(LEFT_INDICATOR_OFF);
                      }
                    }
                  }
                  if(i == 3)
                  {
                    if((SW_On[1] == true) && (SW_LED[1] == true))
                    {
                      R_SIDE.on();
                      sendDashLight(RIGHT_INDICATOR_ON);
                    }
                    else
                    {
                      if(SW_ST[3] == 0)
                      {
                        R_SIDE.off();
                        sendDashLight(RIGHT_INDICATOR_OFF);
                      }
                      else
                      {
                        R_SIDE.level(80);
                        sendDashLight(RIGHT_INDICATOR_OFF);
                      }
                    }
                  }

                } 
                else 
                  {IND_LGHTS[i].on();} 
       } 
       else 
       {
           IND_LGHTS[i].off();
       }       
      }     
    }
    

    /*=================== 初期セットアップ =========================== */

/*==============================================================================
 *      SETUP ROUTINE            SETUP ROUTINE        SETUP ROUTINE
 *==============================================================================*/        
    /*===================SETUP=============================*/
    void setup()  // 
    {
       L_SIDE.beginOUT();
       R_SIDE.beginOUT();

       TC_OFF.beginOUT();
       DFP_BUSY_LED.beginOUT();
       
       DFP_BUSY.beginIN();    // DFPlayer Busy pin
       DIPPED_LIGHTS.beginOUT();
       F_BEAM.beginOUT();
       REAR_LIGHTS.beginOUT();
       BRAKE_LIGHTS.beginOUT();
       REVERSE_LIGHTS.beginOUT();

       RED_AMBER_GREEN_KEYPAD.beginOUT();
       WHITE_KEYPAD.beginOUT();

       PLATE_LIGHTS.beginOUT();

       TC_ON.beginOUT();      
       TCD.beginOUT();
       EIGHTY8_MPH.beginOUT();
       P_EMPTY.beginOUT();
       DASH_BACKLIGHT.beginOUT();       
       PSU.beginOUT();
       PLUTONIUM_BACKLIGHT.beginOUT();
       
       L_BLINK.beginOUT();
       R_BLINK.beginOUT();

       COURTESY_LIGHT.beginOUT();
       DOOR_1_LIGHTS.beginOUT();
       DOOR_2_LIGHTS.beginOUT();
       FRONT_HOOD_LIGHT.beginOUT();       
       ROOF_LIGHT.beginOUT();
       
       BRAKE_SW.beginPULLUP();
       ACCELERATOR_SW.beginPULLUP();

       DOOR_SW_ONE.beginPULLUP();
       
       DOOR_SW_TWO.beginPULLUP();
       
       TRUNK_SW.beginPULLUP();  

       ENGINE_SPACE.beginPULLUP();

       FLUX_BAND.beginOUT();
       FC_ONE.beginOUT();
       FC_TWO.beginOUT();
       FC_THREE.beginOUT();
       FC_FOUR.beginOUT();
       FC_MID.beginOUT();
    

       //if (DOOR_SW_ONE.status() || DOOR_SW_TWO.status()) {SW_LED[7]=true;}
       if (DOOR_SW_ONE.status()) {OLD_Door1=true;}
       if (DOOR_SW_TWO.status()) {OLD_Door2=true;}
  
       //==== INITIALISE SPEEDOMETER=====
       speedo.begin(0x71); 
       speedo.clear();
       speedo.writeDisplay();

     //==== INITIALISE XMAS TREE=====
        randomSeed(analogRead(1));      // 乱数リセット
        matrixOne.begin(0x70);  // define the address for the matrix
        matrixOne.clear();
        matrixOne.writeDisplay();    
  
        Serial.begin(9600);
        Serial1.begin(9600);
        assSerial.begin(9600);  

        randomSeed(analogRead(7));        
        
        //MP3 初期設定
        Serial.println("START ININ");
        mp3_set_serial (Serial1);    //set softwareSerial for DFPlayer-mini mp3 module 
        mp3_stop();                //MP3 停止
        delay(5000);               //MP3 初期起動待ち
        mp3_set_volume(MP3_Vol);   //MP3 音量初期セット
        delay(200);                //MP3 待ち
        mp3_stop();                //MP3 停止
        delay(200);                //MP3 待ち
  
        // RADIO
        currentTrack = random(FIRST_TRACK, LAST_TRACK + 1);
  
        trackNumber = currentTrack - 20;
        
        Wire.begin();        // join i2c bus (address optional for master)
        
        Serial.println("START loop");
    }

    /*===================END SETUP=============================*/


/*==============================================================================
 *      MAIN LOOP            MAIN LOOP            MAIN LOOP       
 *==============================================================================*/        
    
    /*=================== メイン処理 =========================== */
    /*===================MAIN LOOP=============================*/
    void loop() {
      getAppInstr();
      checkSwitches();
      currentMillis = millis();
      if(currentMillis - previousFlashMillis >= flashInterval) 
      {         
            // Call to turn the indicators ON or OFF every 5ms   
            previousFlashMillis = currentMillis;
            
            indicatorFlash();
      }

      if ((currentMillis - previousFluxMillis >= fluxInterval[fluxLevel]) && fluxSwitch) 
          {     
                previousFluxMillis = currentMillis;

                // FLUX CAPACITOR UPDATE ROUTINE
                updateFluxCap();    
          }


      if ((currentMillis - previousLevelOneMatrixMillis >= matrixInterval_1) && (fluxLevel == 1)) {

            previousLevelOneMatrixMillis = currentMillis;

            // XMAS TREE UPDATE ROUTINE FOR LEVEL ONE
            updateLevelOneMatrix();           
       }

       if ( radioOn && DFP_BUSY.readState() )
       {
          // PLAY NEXT TRACK AFTER PREVIOUS TRACK FINISHED
          radioNext();
       }

       if ( SW_ST[1] == 1 && DFP_BUSY.readState() )
       {
          // CONTINUE ENGINE SOUND ON A LOOP IF ENGINE ON
          my_mp3_play(1,5);
       }
    }

    /*===================END MAIN LOOP=============================*/

    /*==================POWER ON=========================*/
    void powerOn()
    {
      TC_OFF.on();
      assSerial.write(PWR_ON_MSG);
      assSerial.write(MP3_Vol);
    }

    /*==================END POWER ON=====================*/

    /*==================POWER OFF========================*/

    void powerOff()
    {
        for(int i = 0 ; i < LEDMAX ; i++) {
          D_PIN[i].off();
          SW_ST [i] = 0;
          }            // 全ＬＥＤ ＯＦＦ
        lightsOut();            
        my_mp3_stop(); // MP3停止 
        
        if (radioOn) {
            assSerial.write(RADIO_STOP_MSG);
            radioOn = false;        
        }

        switchOffMultipleFunctions();
       
        if (roofLightOn) {
            assSerial.write(RF_LGHT_OFF_MSG);
            roofLightOn = false;
        }
                      
        delay(100);
        assSerial.write(PWR_OFF_MSG);
        
    }


    /*==================END POWER OFF========================*/

    /*=================SWITCH OFF MULTIPLE FUNCTIONS=========*/

    void switchOffMultipleFunctions(){
       if (timeCctsOn) {
            timeCircuitsOff();
        }
        if (engineStartOn) {
            engineStop();
            delay(100);
        }
        if (headLightsOn) {
            L_SIDE.off(); // Left side-light
            R_SIDE.off(); // Right side-light
            SW_LED[2]=false; // Left side-light
            SW_LED[3]=false; // Right side-light
            assSerial.write(LGHTS_OFF_ON_MSG);
            SW_ST[3] = 0;
            assSerial.write(SW_ST[3]);
            delay(100);
            assSerial.write(LGHTS_OFF_OFF_MSG);
            headLightsOn = false;          
        }
        if (leftIndicatorsOn) {
            SW_LED[0]=false;            
            SW_LED[1]=false;
            SW_LED[2]=false;
            indicatorsOn = false;
            sendDashLight(LEFT_INDICATOR_OFF);
            assSerial.write(LFT_IND_OFF_MSG);  
            leftIndicatorsOn = false;  
        }
        if (rightIndicatorsOn) {
            SW_LED[0]=false;            
            SW_LED[1]=false;
            SW_LED[2]=false;
            indicatorsOn = false;
            sendDashLight(RIGHT_INDICATOR_OFF);
            assSerial.write(RGT_IND_OFF_MSG); 
            rightIndicatorsOn = false;           
        }
        if (hazardIndicatorsOn) {
            SW_LED[0]=false;            
            SW_LED[1]=false;
            SW_LED[2]=false;            
            SW_LED[3]=false;
            indicatorsOn = false;
            sendDashLight(LEFT_INDICATOR_OFF);
            sendDashLight(RIGHT_INDICATOR_OFF);
            assSerial.write(HZD_LGHT_OFF_MSG);
            hazardIndicatorsOn = false;
        }
        if (brakeLightsOn) {
            SW_ST[2] = 0;
            assSerial.write(BRAKE_LIGHTS_OFF_MSG);
            appBrake = false; 
            brakeLightsOn = false;         
        }
        if (reverseLightsOn) {
            REVERSE_LIGHTS.off();
            assSerial.write(REV_LGHT_OFF_MSG);
            reverseLightsOn = false;
        }

        delay(100);
      
    }



    /*=================END SWITCH OFF MULTIPLE FUNCTIONS=========*/

    /*================ENGINE START==========================*/
    void engineStart()
    {      
      engineStartOn = true;
      assSerial.write(ENG_ON_MSG);
      SW_ST[1] = 1;
      my_mp3_play(1,3);
      sendDashLight(LIGHTS_ON);
      delay(3000);
      sendDashLight(LIGHTS_OFF);
    }
    /*================END ENGINE START==========================*/

    /*================ENGINE STOP==========================*/

    void engineStop()
    {
      engineStartOn = false;
      SW_ST[1] = 0;
      my_mp3_stop();
      assSerial.write(ENG_OFF_MSG);
    }

    /*================END ENGINE STOP==========================*/ 

    /*================ACCELERATOR==========================*/

    void accelerator()
    {
      if(!radioOn)
      {
        if(SW_ST[1] == 1)
        {
          assSerial.write(ACCEL_ON_MSG);
          my_mp3_play(1,4);
          SW_ST[1] = 1;
          delay(1000);
          assSerial.write(ACCEL_OFF_MSG);
        }
      }
    }

    /*================END ACCELERATOR==========================*/

    /*================LEFT INDICATOR ON==========================*/ 

    void leftIndOn()
    {    
        leftIndicatorsOn = true;
        if(SW_ST[3] == 0)
        {
            IND_INTERVAL[0] = 45;
            IND_INTERVAL[1] = 45; 
        }
        else
        {
            IND_INTERVAL[0] = 25;
            IND_INTERVAL[1] = 25;
        }
        assSerial.write(LFT_IND_ON_MSG);        
        FLASH_DELAY[0] = IND_INTERVAL[0];
        FLASH_DELAY[1] = IND_INTERVAL[1]; 
        SW_LED[0]=true;
        SW_LED[2]=true;
        SW_LED[1]=false;
        indicatorsOn = true;          
    }


    /*================END LEFT INDICATOR ON==========================*/ 


     /*================LEFT INDICATOR OFF==========================*/ 

    void leftIndOff()
    {      
        leftIndicatorsOn = false;
        SW_LED[0]=false;            
        SW_LED[1]=false;
        if(SW_ST[3] == 0){SW_LED[2]=false;}
        indicatorsOn = false;
        sendDashLight(LEFT_INDICATOR_OFF);
        assSerial.write(LFT_IND_OFF_MSG);            
    }


    /*================END LEFT INDICATOR OFF==========================*/ 


     /*================HAZARD LIGHTS ON==========================*/ 

      void hazardLightsOn()
      {        
          hazardIndicatorsOn = true;
          IND_INTERVAL[0] = 25;
          IND_INTERVAL[1] = 25;  
          assSerial.write(HZD_LGHT_ON_MSG);  
          FLASH_DELAY[0] = IND_INTERVAL[0];
          FLASH_DELAY[1] = IND_INTERVAL[1];        
          SW_LED[0]=true;
          SW_LED[1]=true;
          SW_LED[2]=true;
          SW_LED[3]=true;
          indicatorsOn = true;             
      }


     /*================END HAZARD LIGHTS ON==========================*/ 

     /*================HAZARD LIGHTS OFF==========================*/ 

      void hazardLightsOff()
      {          
          hazardIndicatorsOn = false;
          SW_LED[0]=false;            
          SW_LED[1]=false;
          if(SW_ST[3] == 0){
          SW_LED[2]=false;            
          SW_LED[3]=false;}
          indicatorsOn = false;
          sendDashLight(LEFT_INDICATOR_OFF);
          sendDashLight(RIGHT_INDICATOR_OFF);
          assSerial.write(HZD_LGHT_OFF_MSG);           
      }


     /*================END HAZARD LIGHTS OFF==========================*/ 


    /*================RIGHT INDICATOR ON==========================*/ 

    void rightIndOn()
    {           
        rightIndicatorsOn = true;   
        if(SW_ST[3] == 0)
        {
            IND_INTERVAL[0] = 45;
            IND_INTERVAL[1] = 45; 
        }
        else
        {
            IND_INTERVAL[0] = 25;
            IND_INTERVAL[1] = 25;
        }
        FLASH_DELAY[0] = IND_INTERVAL[0];
        FLASH_DELAY[1] = IND_INTERVAL[1]; 
        SW_LED[0]=false;
        SW_LED[1]=true;
        SW_LED[3]=true;
        indicatorsOn = true;
        assSerial.write(RGT_IND_ON_MSG);                  
    }


    /*================END RIGHT INDICATOR ON==========================*/ 


    /*================RIGHT INDICATOR OFF==========================*/ 

    void rightIndOff()
    {      
        rightIndicatorsOn = false;
        SW_LED[0]=false;            
        SW_LED[1]=false;
        if(SW_ST[3] == 0){SW_LED[3]=false;} 
        indicatorsOn = false;
        sendDashLight(RIGHT_INDICATOR_OFF);
        assSerial.write(RGT_IND_OFF_MSG);                
    }


    /*================END RIGHT INDICATOR OFF==========================*/ 


    /*================LIGHTS ON============================*/
    void lightsOn()
    {
      SW_ST[3]++;
      if (SW_ST[3] > 3) 
      {
          SW_ST[3] = 3;
      }
      else
      {
        switch (SW_ST[3])
        {
          case 1:      
            headLightsOn = true;
            L_SIDE.level(80); // Left side-light
            R_SIDE.level(80); // Right side-light
            SW_LED[2]=true; // Left side-light
            SW_LED[3]=true; // Right side-light
            DASH_BACKLIGHT.on();
            
            assSerial.write(LGHTS_ON_ON_MSG);
            assSerial.write(SW_ST[3]);
            break;
          case 2:
            DIPPED_LIGHTS.on(); // Main lights     
            REAR_LIGHTS.on(); // Rear lights
            PLATE_LIGHTS.on();            
            sendDashLight(MAIN_LIGHTS_ON);
            assSerial.write(LGHTS_ON_ON_MSG);
            assSerial.write(SW_ST[3]);
            break;
          case 3:      
            F_BEAM.on(); // Full beam
            sendDashLight(FULL_BEAM_ON);
            assSerial.write(LGHTS_ON_ON_MSG);
            assSerial.write(SW_ST[3]);
            break;
        }
        delay(1000);
        assSerial.write(LGHTS_ON_OFF_MSG);
      }            
    }

    /*================END LIGHTS ON======================*/
    
    /*================LIGHTS OFF======================*/
    void lightsOff()
    {
      SW_ST[3]--;
      if (SW_ST[3] < 0) 
      {
          SW_ST[3] = 0;
      }
      else
      {
        switch (SW_ST[3])
        {
          case 0:
            headLightsOn = false;           
            L_SIDE.off(); // Left side-light
            R_SIDE.off(); // Right side-light
            if(!indicatorsOn)
            {
                SW_LED[2]=false; // Left side-light
                SW_LED[3]=false; // Right side-light
            }
            DASH_BACKLIGHT.off();
            
            assSerial.write(LGHTS_OFF_ON_MSG);
            assSerial.write(SW_ST[3]);
            break;
          case 1:      
            DIPPED_LIGHTS.off(); // Main lights     
            REAR_LIGHTS.off(); // Rear lights
            PLATE_LIGHTS.off();   
            sendDashLight(MAIN_LIGHTS_OFF);
            assSerial.write(LGHTS_OFF_ON_MSG);
            assSerial.write(SW_ST[3]);
            break;
          case 2:      
            F_BEAM.off();
            sendDashLight(FULL_BEAM_OFF);
            assSerial.write(LGHTS_OFF_ON_MSG);
            assSerial.write(SW_ST[3]);
            break;
        }
        delay(1000);
        assSerial.write(LGHTS_OFF_OFF_MSG);
      }
    }
    /*================END LIGHTS OFF======================*/    

    /*================PASSING LIGHTS======================*/

    void passingLights()
    {
      assSerial.write(PSS_LGHT_ON_MSG);
      for( int i = 0; i < 4; i++ )
          {
           sendDashLight(PASSING_LIGHTS_ON);
           DIPPED_LIGHTS.on();
           F_BEAM.on();
           delay(140);
           sendDashLight(PASSING_LIGHTS_OFF);
           DIPPED_LIGHTS.off();
           F_BEAM.off();
           delay(120);
          }
          switch (SW_ST[3])
          {
            case 2:      
              sendDashLight(MAIN_LIGHTS_ON);
              DIPPED_LIGHTS.on();
              F_BEAM.off();
              break;
            case 3:     
              sendDashLight(FULL_BEAM_ON);
              sendDashLight(MAIN_LIGHTS_ON);
              DIPPED_LIGHTS.on();
              F_BEAM.on();
              break;
           default:
              DIPPED_LIGHTS.off();
              F_BEAM.off();
          }
          assSerial.write(PSS_LGHT_OFF_MSG);
    }

    /*================END PASSING LIGHTS======================*/
    

    /*================TIME CIRCUITS ON======================*/

    void timeCircuitsOn()
    {
      timeCctsOn = true;
      assSerial.write(TM_CCT_ON_MSG);
      my_mp3_play(1,12); // Time circuits ON sound  
      TC_OFF.off();         // Time circuits OFF light off
      TC_ON.on();            // Time circuits ON light on  
      TCD.on();
      RED_AMBER_GREEN_KEYPAD.on();
      delay(3000);
      PLUTONIUM_BACKLIGHT.on();  
      PSU.on();              // Power supply unit ON      
      delay(1000);  
      if(SW_ST[1] == 1) {
        my_mp3_play(1,5); // Engine on
      }  
      fluxSwitch = true;
      fluxLevel = 1;
      sendReactorMode(fluxLevel);
    }

    /*================END TIME CIRCUITS ON======================*/

    /*================TIME CIRCUITS OFF=========================*/

    void timeCircuitsOff()
    {
      timeCctsOn = false;
      fluxSwitch = false;
      resetFluxTree();
      my_mp3_play(1,15); // Time circuits OFF sound
      TCD.off();
      RED_AMBER_GREEN_KEYPAD.off();
      PSU.off();
      PLUTONIUM_BACKLIGHT.off();
      TC_ON.off();
      TC_OFF.on();       
      assSerial.write(TM_CCT_OFF_MSG); 
      delay(1000);
      if(SW_ST[1] == 1) {
        my_mp3_play(1,5); // Engine on
      }  
    }

    /*================END TIME CIRCUITS OFF=========================*/


    /*================HORN=========================*/

    void horn()
    {
      assSerial.write(HORN_ON_MSG);
      my_mp3_play(1,6);
      delay(100);
      my_mp3_playend();
      if (SW_ST[1] == 1) {
          my_mp3_play(1,5);
          }
      delay(100);
      assSerial.write(HORN_OFF_MSG);
    }

    /*================END HORN=========================*/

/*==============================================================================
 *      TIME TRAVEL ROUTINE            TIME TRAVEL ROUTINE        
 *==============================================================================*/    
    
    /*===============START TIME TRAVEL======================*/
    void time_travel() {
           assSerial.write(TM_TRAV_ON_MSG);   

           speedoInterval = B8_speedoIntervalSetting;
                   
           TC_OFF.on();            // Time circuits OFF light on
    
           currentMillis = millis(); 
           previousTtMillis = currentMillis;
           currentMicros = micros();
           previousSpeedoMicros = currentMicros;
           
    
          do {
    
            currentMillis = millis();
            currentMicros = micros();    


            if ((currentMillis - previousFluxMillis >= fluxInterval[fluxLevel]) && fluxSwitch) 
          {     
                previousFluxMillis = currentMillis;

                // FLUX CAPACITOR UPDATE ROUTINE
                updateFluxCap();
    
          }


          if ((currentMillis - previousLevelOneMatrixMillis >= matrixInterval_1) && (fluxLevel == 1)) {
    
                previousLevelOneMatrixMillis = currentMillis;

                // XMAS TREE UPDATE ROUTINE FOR LEVEL ONE
                updateLevelOneMatrix();       
           
           }


           if ((currentMillis - previousOneSevenMatrixMillis >= matrixInterval_2) && (fluxLevel > 1) && matrixSwitch_2) {
    
              previousOneSevenMatrixMillis = currentMillis;
              // XMAS TREE UPDATE ROUTINE FOR COLUMNS 1 AND 7
              updateColumnOneSevenMatrix();      
              
              matrixSwitch_2 = false;
              matrixSwitch_3 = true;   
            }


            if ((currentMillis - previousThreeSixMatrixMillis >= matrixInterval_2) && (fluxLevel > 1) && matrixSwitch_3) {
    
              previousThreeSixMatrixMillis = currentMillis;

              // XMAS TREE UPDATE ROUTINE FOR COLUMNS 3 AND 6
              updateColumnThreeSixMatrix();      
              
              matrixSwitch_3 = false;
              matrixSwitch_4 = true;    
            }


            if ((currentMillis - previousZeroFourMatrixMillis >= matrixInterval_2) && (fluxLevel > 1) && matrixSwitch_4) {
    
              previousZeroFourMatrixMillis = currentMillis;

              // XMAS TREE UPDATE ROUTINE FOR COLUMNS 0 AND 4
              updateColumnZeroFourMatrix();     
              
              matrixSwitch_4 = false;
              matrixSwitch_5 = true;    
            }


            if ((currentMillis - previousTwoFiveMatrixMillis >= matrixInterval_2) && (fluxLevel > 1) && matrixSwitch_5) {
    
              previousTwoFiveMatrixMillis = currentMillis;

              // XMAS TREE UPDATE ROUTINE FOR COLUMNS 2 AND 5
              updateColumnTwoFiveMatrix();              
          
              matrixSwitch_5 = false;
              matrixSwitch_2 = true;    
            }
     
    
            if((currentMicros - previousSpeedoMicros >= speedoInterval) && (speedoOut)) {        
            previousSpeedoMicros = currentMicros;        
            B8_speedoAcceleration();        
           }
          
          
          //delay(500);                
          
          if((currentMillis - previousTtMillis >= B8_INTERVAL[j]) && (first)) {

            previousTtMillis = currentMillis;
          
            my_mp3_play(1,7);

            sendDashLight(LIGHTS_ON);  // All dash lights ON
                 
                
          
            
            first = false;
            second = true;        
            j++;
            
          }
    
         
          //delay(500);
    
         if((currentMillis - previousTtMillis >= B8_INTERVAL[j]) && (second)) {

            previousTtMillis = currentMillis; 

            TC_OFF.off();         // Time circuits OFF light off
            TC_ON.on();            // Time circuits ON light on
            TCD.on();            // Time circuits display ON  
            RED_AMBER_GREEN_KEYPAD.on();           
            
            second = false;
            third = true;        
            j++;
                        
          }
          
          
          //delay(2500);           3 secs
    
          if((currentMillis - previousTtMillis >= B8_INTERVAL[j]) && (third)) {

            previousTtMillis = currentMillis; 

            sendDashLight(LIGHTS_OFF);   // All dash lights OFF             
            PLUTONIUM_BACKLIGHT.on();  
            PSU.on();
            DASH_BACKLIGHT.on();
            L_SIDE.level(80); //  Left side light ON
            R_SIDE.level(80); //  Right side light ON  
            DIPPED_LIGHTS.on(); // DIPPED_LIGHTS.on(); Main beam, rear lights, licence plate ON
            REAR_LIGHTS.on(); // REAR_LIGHTS.on();
            PLATE_LIGHTS.on();           
            sendDashLight(MAIN_LIGHTS_ON);  // Main lights ON                  
            
            third = false;
            fourth = true;        
            j++;
            
          }
    
          
          //delay(1000);         4 secs
    
          if((currentMillis - previousTtMillis >= B8_INTERVAL[j]) && (fourth)) {

            previousTtMillis = currentMillis;

        

            fluxSwitch = true;
            fluxLevel++; // FLUX LEVEL 1                   
                  
            
            fourth = false;
            fifth = true;        
            j++;
           
          }
    
          
          //delay(500); 4.5 secs
    
    
          if((currentMillis - previousTtMillis >= B8_INTERVAL[j]) && (fifth)) {

            previousTtMillis = currentMillis;


            
            //WHITE_KEYPAD.off();       

            sendReactorMode(fluxLevel);
                 
            
            fifth = false;
            fifth_b = true;        
            j++;
            
          }    

          //delay(1000); 5.5 secs

          if((currentMillis - previousTtMillis >= B8_INTERVAL[j]) && (fifth_b)) {

            previousTtMillis = currentMillis;

            WHITE_KEYPAD.on();

            
            fifth_b = false;
            fifth_c = true;        
            j++;
            
          }

          if((currentMillis - previousTtMillis >= B8_INTERVAL[j]) && (fifth_c)) {

            previousTtMillis = currentMillis;

            WHITE_KEYPAD.off();

            
            fifth_c = false;
            sixth = true;        
            j++;
            
          }              
               
          
          //delay(1500);        6 secs
    
    
          if((currentMillis - previousTtMillis >= B8_INTERVAL[j]) && (sixth)) {

            previousTtMillis = currentMillis;

            speedoOut = true; 
            fluxLevel++; // FLUX LEVEL 2    
                 
            
            sixth = false;
            seventh = true;        
            j++;
            
          }
          
          
          //delay(500);
    
    
          if((currentMillis - previousTtMillis >= B8_INTERVAL[j]) && (seventh)) {

            previousTtMillis = currentMillis;


            sendReactorMode(fluxLevel);          
                
            
            seventh = false;
            eighth = true;        
            j++;
           
          }
          
          
          //delay(2000);       10.5 secs
    
    
    
          if((currentMillis - previousTtMillis >= B8_INTERVAL[j]) && (eighth)) {

            previousTtMillis = currentMillis;


            fluxLevel++; // FLUX LEVEL 3   
                   
            
            eighth = false;
            ninth = true;        
            j++;
            
          } 
                
          
          //delay(500);
    
    
          if((currentMillis - previousTtMillis >= B8_INTERVAL[j]) && (ninth)) { 

            previousTtMillis = currentMillis; 


            sendReactorMode(fluxLevel);         
                 
            
            ninth = false;
            tenth = true;        
            j++;
          
          }
          
          
          //delay(3500); 14.5 secs
    
    
    
          if((currentMillis - previousTtMillis >= B8_INTERVAL[j]) && (tenth)) {

            previousTtMillis = currentMillis;


            fluxLevel++; // FLUX LEVEL 4                  
                  
            
            tenth = false;
            eleventh = true;        
            j++;
            
          }
          
          
          //delay(500);
    
    
          if((currentMillis - previousTtMillis >= B8_INTERVAL[j]) && (eleventh)) {

            previousTtMillis = currentMillis;


            sendReactorMode(fluxLevel);                 
                  
            
            eleventh = false;
            twelfth = true;
            
            j++;
            
          }
          
          
          //delay(2200);    17.7 secs
    
    
          if((currentMillis - previousTtMillis >= B8_INTERVAL[j]) && (twelfth)) {

            previousTtMillis = currentMillis;


            fluxLevel++; // FLUX LEVEL 5     

            sendReactorMode(fluxLevel);
                  
            
            twelfth = false;
            thirteenth = true;        
            j++;
           
          }
          
          
          //delay(3200);  20.9 secs
    
    
          if((currentMillis - previousTtMillis >= B8_INTERVAL[j]) && (thirteenth)) {

            previousTtMillis = currentMillis; 

            EIGHTY8_MPH.on();               // 88mph display ON                   
            
            thirteenth = false;
            fourteenth = true;
            
            j++;
          }
          
          
          //delay(300); 21.2 secs
    
    
          if((currentMillis - previousTtMillis >= B8_INTERVAL[j]) && (fourteenth)) {

            previousTtMillis = currentMillis;            

            fluxLevel++; // FLUX LEVEL 6                    
            
            fourteenth = false;
            fifteenth = true;            
            j++;            
          }
          
          
          //delay(500);
    
    
          if((currentMillis - previousTtMillis >= B8_INTERVAL[j]) && (fifteenth)) {

            previousTtMillis = currentMillis;


            sendReactorMode(fluxLevel);          
            
            
            fifteenth = false;
            sixteenth = true;
            j++;            
           
          }

          //delay(3500); 25.2 secs

          if((currentMillis - previousTtMillis >= B8_INTERVAL[j]) && (sixteenth)) {

            previousTtMillis = currentMillis;
          
            fluxLevel++; // FLUX LEVEL 7
            send_flux(7);              
            
            sixteenth = false;
            seventeenth = true;
            
            j++;
            
          }

          //delay(2500); 27.7 secs

          if((currentMillis - previousTtMillis >= B8_INTERVAL[j]) && (seventeenth)) {

            previousTtMillis = currentMillis;
          
            send_flux(8);   

            resetFluxTree();

            lightsOut(); 
            speedo.clear();
            speedo.writeDisplay(); 
            speedoOut = false;          
            
            seventeenth = false;
            eighteenth = true;
            
            j++;
            
          }

          //delay(1000); 28.7 secs

          if((currentMillis - previousTtMillis >= B8_INTERVAL[j]) && (eighteenth)) {

            previousTtMillis = currentMillis;          

            entryFlash();
            
            eighteenth = false;
            nineteenth = true;
            j++;
           
          }

          // delay(500);
          

         if((currentMillis - previousTtMillis >= B8_INTERVAL[j]) && (nineteenth)) {

            previousTtMillis = currentMillis; 
                       
            DASH_BACKLIGHT.on();
            L_SIDE.level(80); // Left side light ON
            R_SIDE.level(80); // Right side light ON
            DIPPED_LIGHTS.on(); // DIPPED_LIGHTS.on(); Main beam, rear lights, licence plate ON
            REAR_LIGHTS.on();; // REAR_LIGHTS.on();
            PLATE_LIGHTS.on();
            sendDashLight(MAIN_LIGHTS_ON);  // Main lights ON

            PLUTONIUM_BACKLIGHT.on();
            TCD.on();
            RED_AMBER_GREEN_KEYPAD.on();
            TC_ON.on();
    
            BRAKE_LIGHTS.on();
        
            nineteenth = false;
            twentieth = true;
            j++;  

          }

          //delay(1500);   30.7 secs  INTERVAL 21 

          if((currentMillis - previousTtMillis >= B8_INTERVAL[j]) && (twentieth)) {

            previousTtMillis = currentMillis;          

            BRAKE_LIGHTS.off();
            
            twentieth = false;
            twentyfirst = true;
            j++;
           
          }

          //delay(1500);   INTERVAL 22

           // Plutonium empty loop
           // delay(400); INTERVAL 24
           
           if((currentMillis - previousTtMillis >= B8_INTERVAL[j]) && (twentyfirst)) {
  
              previousTtMillis = currentMillis;  
  
              P_EMPTY.on();            // Plutonium empty ON                            
              
              twentyfirst = false;
              twentysecond = true;              
              j = 23;  
          }


          //delay(400); INTERVAL 23


           if((currentMillis - previousTtMillis >= B8_INTERVAL[j]) && (twentysecond)) {

            previousTtMillis = currentMillis;


            P_EMPTY.off();            // Plutonium empty OFF 


            if (pEmptyLoopCount++ < 6)
            {
               twentysecond = false; 
               twentyfirst = true;  
               j =24;
            }
            else
            {
              twentysecond = false;
              twentythird = true;              
              j = 25;
              resetFluxTree();
            }
          }       

          //delay(100); INTERVAL 25

          if((currentMillis - previousTtMillis >= B8_INTERVAL[j]) && (twentythird)) {

            previousTtMillis = currentMillis;

            pEmptyLoopCount = 0;  
            twentythird = false;        
            first = true;
            STOP = true;           
            j = 0;
           }          
    
        }while(!STOP);
        
        send_flux(9);
        my_mp3_playend();
        
        lightsOut();

        speedo.clear();
        speedo.writeDisplay(); 

        TC_OFF.on();  

        counter = 0; // reset speedo counter
        
        send_flux(0);       
                  
        STOP = false; 

        assSerial.write(TM_TRAV_OFF_MSG);
    }

    /*==================End Time Travel=====================*/


/*==============================================================================
 *      TIME TRAVEL MOVIE DEMO ROUTINE            TIME TRAVEL MOVIE DEMO ROUTINE        
 *==============================================================================*/    

    /*================START TIME TRAVEL MOVIE DEMO================*/
    void timeTravelMovieDemo() {           
           speedoInterval = speedoIntervalSettingOne;
           
           assSerial.write(MOV_DEMO_ON_MSG);                
          
           delay(2500);                // 
    
           my_mp3_play(1,11);          // "First, you turn the time circuits on"    

           //SET CLOCK TO ZERO FROM START OF MOVIE SEQUENCE
           currentMillis = millis(); 
           previousTtMillis = currentMillis;
           currentMicros = micros();
           previousSpeedoMicros = currentMicros;
    
       do {
            // UPDATE CLOCK ON EACH LOOP ITERATION
            currentMillis = millis();
            currentMicros = micros();     

          if ((currentMillis - previousFluxMillis >= fluxInterval[fluxLevel]) && fluxSwitch) 
          {     
                previousFluxMillis = currentMillis;

                // FLUX CAPACITOR UPDATE ROUTINE
                updateFluxCap();
    
          }


          if ((currentMillis - previousLevelOneMatrixMillis >= matrixInterval_1) && (fluxLevel == 1)) {
    
                previousLevelOneMatrixMillis = currentMillis;

                // XMAS TREE UPDATE ROUTINE FOR LEVEL ONE
                updateLevelOneMatrix();       
           
           }


           if ((currentMillis - previousOneSevenMatrixMillis >= matrixInterval_2) && (fluxLevel > 1) && matrixSwitch_2) {
    
              previousOneSevenMatrixMillis = currentMillis;
              
              // XMAS TREE UPDATE ROUTINE FOR COLUMNS 1 AND 7
              updateColumnOneSevenMatrix();      
              
              matrixSwitch_2 = false;
              matrixSwitch_3 = true;   
            }


            if ((currentMillis - previousThreeSixMatrixMillis >= matrixInterval_2) && (fluxLevel > 1) && matrixSwitch_3) {
    
              previousThreeSixMatrixMillis = currentMillis;

              // XMAS TREE UPDATE ROUTINE FOR COLUMNS 3 AND 6
              updateColumnThreeSixMatrix();      
              
              matrixSwitch_3 = false;
              matrixSwitch_4 = true;    
            }


            if ((currentMillis - previousZeroFourMatrixMillis >= matrixInterval_2) && (fluxLevel > 1) && matrixSwitch_4) {
    
              previousZeroFourMatrixMillis = currentMillis;

              // XMAS TREE UPDATE ROUTINE FOR COLUMNS 0 AND 4
              updateColumnZeroFourMatrix();     
              
              matrixSwitch_4 = false;
              matrixSwitch_5 = true;    
            }


            if ((currentMillis - previousTwoFiveMatrixMillis >= matrixInterval_2) && (fluxLevel > 1) && matrixSwitch_5) {
    
              previousTwoFiveMatrixMillis = currentMillis;

              // XMAS TREE UPDATE ROUTINE FOR COLUMNS 2 AND 5
              updateColumnTwoFiveMatrix();              
          
              matrixSwitch_5 = false;
              matrixSwitch_2 = true;    
            }
          
    
          if((currentMicros - previousSpeedoReturnMicros >= speedoReturnInterval) && (speedoReturn)) {       
            previousSpeedoReturnMicros = currentMicros;
            speedoDecceleration();        
           }
    
          if((currentMicros - previousSpeedoMicros >= speedoInterval) && (speedoOut)) {        
            previousSpeedoMicros = currentMicros;        
            speedoAcceleration();        
           }
          
          
          //delay(3000);                // 3 seconds
    
          if((currentMillis - previousTtMillis >= INTERVAL[j]) && (first)) {

            previousTtMillis = currentMillis;

            TC_OFF.off();         // Time circuits OFF light off
            TC_ON.on();            // Time circuits ON light on
            TCD.on();            // Time circuits display ON    
            RED_AMBER_GREEN_KEYPAD.on();    
            
            first = false;
            second = true;        
            j++;
            
          }
       
          //delay(3000);                // 6 seconds
    
          if((currentMillis - previousTtMillis >= INTERVAL[j]) && (second)) {

            previousTtMillis = currentMillis;                  
            
            PLUTONIUM_BACKLIGHT.on();  
            PSU.on();              // Power supply unit ON           

            second = false;
            third = true;        
            j++;
           
          }
     
         // delay(1000);
    
          if((currentMillis - previousTtMillis >= INTERVAL[j]) && (third)) {

            previousTtMillis = currentMillis; 

            fluxSwitch = true;
            fluxLevel++; // FLUX LEVEL 1   

            sendReactorMode(fluxLevel);  
                        
            third = false;
            fourth = true;        
            j++;
          }
          
         // delay(500);
    
          if((currentMillis - previousTtMillis >= INTERVAL[j]) && (fourth)) {

            previousTtMillis = currentMillis;
                  
                  
            
            fourth = false;
            fifth = true;        
            j++;
          }
          
          //delay(11000);               // 18.5 seconds
    
          if((currentMillis - previousTtMillis >= INTERVAL[j]) && (fifth)) {

            previousTtMillis = currentMillis;
          
                    
            sendDashLight(LIGHTS_ON);  // All dash lights ON
            
            
            fifth = false;
            sixth = true;        
            j++;

          }      
       
          //delay(3000);                // 21.5 seconds
    
          if((currentMillis - previousTtMillis >= INTERVAL[j]) && (sixth)) {

            previousTtMillis = currentMillis;
    
            sendDashLight(LIGHTS_OFF);   // Console warning lights OFF 
            L_SIDE.level(80); // Left side light ON
            R_SIDE.level(80); // Right side light ON
            DASH_BACKLIGHT.on();
            DIPPED_LIGHTS.on(); //Main beam, rear lights, licence plate ON
            REAR_LIGHTS.on();
            PLATE_LIGHTS.on();
            //SW_ST[3] = 1;     // Set side lights switch ON 
            
            sendDashLight(MAIN_LIGHTS_ON);  // Main lights ON
            
            sixth = false;
            seventh = true;        
            j++;

          }
         
          
          //delay(4000);                // 25.5 seconds
    
          if((currentMillis - previousTtMillis >= INTERVAL[j]) && (seventh)) {

            previousTtMillis = currentMillis;
          
            BRAKE_LIGHTS.on();       
            
            seventh = false;
            speedoOutOpen = true;        
            j++;

          }
    
          // delay(2500);
    
          if((currentMillis - previousTtMillis >= INTERVAL[j]) && (speedoOutOpen)) {

            previousTtMillis = currentMillis;
            
            speedoOut = true;        
            
            speedoOutOpen = false;
            eighth = true;        
            j++;
          } 
           
          //delay(25000);               // 53 seconds
    
          if((currentMillis - previousTtMillis >= INTERVAL[j]) && (eighth)) {

            previousTtMillis = currentMillis;
            
            BRAKE_LIGHTS.off();         
            speedoInterval = speedoIntervalSettingTwo;          


            fluxLevel++; // FLUX LEVEL 2        
                       
            eighth = false;
            ninth = true;        
            j++;
          } 
             
          //delay(500);
    
          if((currentMillis - previousTtMillis >= INTERVAL[j]) && (ninth)) { 

            previousTtMillis = currentMillis;    
          
            sendReactorMode(fluxLevel);       
            
            ninth = false;
            tenth = true;        
            j++;
          }
          
          //delay(2000);
    
          if((currentMillis - previousTtMillis >= INTERVAL[j]) && (tenth)) {

            previousTtMillis = currentMillis;

            fluxLevel++; // FLUX LEVEL 3 
            
            tenth = false;
            eleventh = true;        
            j++;
          }
          
          //delay(500);
    
          if((currentMillis - previousTtMillis >= INTERVAL[j]) && (eleventh)) {

            previousTtMillis = currentMillis;
                  
            sendReactorMode(fluxLevel);        
            
            eleventh = false;
            twelfth = true;            
            j++;
          }
          
          //delay(5200);
    
          if((currentMillis - previousTtMillis >= INTERVAL[j]) && (twelfth)) {

            previousTtMillis = currentMillis;          

            fluxLevel++; // FLUX LEVEL 4 
            
            twelfth = false;
            thirteenth = true;        
            j++;
          }
          
          //delay(500);
    
          if((currentMillis - previousTtMillis >= INTERVAL[j]) && (thirteenth)) {

            previousTtMillis = currentMillis;
          
            sendReactorMode(fluxLevel);
            
            
            thirteenth = false;
            fourteenth = true;            
            j++;
          }
          
          //delay(300);
    
          if((currentMillis - previousTtMillis >= INTERVAL[j]) && (fourteenth)) {

            previousTtMillis = currentMillis;

            fluxLevel++; // FLUX LEVEL 5
            
            fourteenth = false;
            fifteenth = true;            
            j++;
          }
          
          //delay(500);
    
          if((currentMillis - previousTtMillis >= INTERVAL[j]) && (fifteenth)) {

            previousTtMillis = currentMillis;
          
            sendReactorMode(fluxLevel);
            
            
            fifteenth = false;
            sixteenth = true;            
            j++;
          }
          
          //delay(1000);
    
          if((currentMillis - previousTtMillis >= INTERVAL[j]) && (sixteenth)) {

            previousTtMillis = currentMillis;

            fluxLevel++; // FLUX LEVEL 6            
            
            sixteenth = false;
            seventeenth = true;            
            j++;
          }
          
          //delay(500);               // 64 seconds
    
          if((currentMillis - previousTtMillis >= INTERVAL[j]) && (seventeenth)) {

            previousTtMillis = currentMillis;

            sendReactorMode(fluxLevel);
          
            EIGHTY8_MPH.on();               // 88mph display ON           
            
            seventeenth = false;
            eighteenth = true;            
            j++;
          }
           
          //delay(2500);
    
          if((currentMillis - previousTtMillis >= INTERVAL[j]) && (eighteenth)) {

            previousTtMillis = currentMillis;

            fluxLevel++; // FLUX LEVEL 7
            send_flux(7);            
            
            eighteenth = false;
            nineteenth = true;            
            j++;
          }
         
          //delay(500);
    
          if((currentMillis - previousTtMillis >= INTERVAL[j]) && (nineteenth)) {

            previousTtMillis = currentMillis;
          
//            FLUX_TRIGGER.off();
            
            
            nineteenth = false;
            twentieth = true;            
            j++;
          }
          
          //delay(3000);
    
          if((currentMillis - previousTtMillis >= INTERVAL[j]) && (twentieth)) {

            previousTtMillis = currentMillis;

            fluxLevel++; // FLUX LEVEL 8 
            send_flux(8);            
            
            twentieth = false;
            twentyfirst = true;        
            j++;
          }
         
          //delay(2000);                // 72 seconds
    
          if((currentMillis - previousTtMillis >= INTERVAL[j]) && (twentyfirst)) {

            previousTtMillis = currentMillis;
    
            EIGHTY8_MPH.off();                // 88mph display OFF 
            speedo.clear();
            speedo.writeDisplay(); 
            speedoOut = false;            
            fluxLevel = 0;
            sendReactorMode(fluxLevel);
            matrixOne.clear();
            matrixOne.writeDisplay();    
            
            twentyfirst = false;
            twentysecond = true;            
            j++;
          }
          
          
          //delay(100);
    
          if((currentMillis - previousTtMillis >= INTERVAL[j]) && (twentysecond)) {

            previousTtMillis = currentMillis;
            
            sendDashLight(LIGHTS_OFF);
            lightsOut();
            send_flux(9);
                
            twentysecond = false;
            fluxflash = true;        
            j++;
          }      
    
          //delay(12000);
    
          if((currentMillis - previousTtMillis >= INTERVAL[j]) && (fluxflash)) {

            previousTtMillis = currentMillis;
    
//            FLUX_ON_OFF.on();
//            FLUX_FLASH.on();
            sendReactorMode(RE_ENTRY);
            
            fluxflash = false;
            twentythird = true;        
            j++;
           } 
    
           //delay(500)
    
          if((currentMillis - previousTtMillis >= INTERVAL[j]) && (twentythird)) {

            previousTtMillis = currentMillis;     

            
            
            entryFlash();
            
            twentythird = false;
            twentyfourth = true;        
            j++;
           } 
           
            //delay(600);
    
          if((currentMillis - previousTtMillis >= INTERVAL[j]) && (twentyfourth)) {

            previousTtMillis = currentMillis;
            
            entryFlash();
            
            twentyfourth = false;
            twentyfifth = true;        
            j++;
           }
           
            //delay(600);
    
          if((currentMillis - previousTtMillis >= INTERVAL[j]) && (twentyfifth)) {

            previousTtMillis = currentMillis;
    
            entryFlash();
            DASH_BACKLIGHT.on();
            L_SIDE.on(); // Left side light ON
            R_SIDE.on(); // Right side light ON 
            DIPPED_LIGHTS.on(); //Main beam, rear lights, licence plate ON
            REAR_LIGHTS.on();
            PLATE_LIGHTS.on();
            //SW_ST[3] = 1;     // Set side lights switch ON 
            sendDashLight(MAIN_LIGHTS_ON);
            
            fluxLevel = 1;  // FLUX LEVEL 1 

            sendReactorMode(fluxLevel);

            PLUTONIUM_BACKLIGHT.on();
            TC_ON.on();            // Time circuits ON light on        
            TCD.on();            // Time circuits display ON 
            RED_AMBER_GREEN_KEYPAD.on();
            PSU.on();           
    
            speedoReturn = true;
                   
            counter = 8800;         
            
            twentyfifth = false;
            twentysixth = true;        
            j++;
           }  
    
    
          //delay(1500);                // 85 seconds
    
          if((currentMillis - previousTtMillis >= INTERVAL[j]) && (twentysixth)) {

            previousTtMillis = currentMillis;
          
            BRAKE_LIGHTS.on();
           
            twentysixth = false;
            twentyseventh = true;        
            j++;
           }
      
          //delay(6500);                // 94 seconds
    
          if((currentMillis - previousTtMillis >= INTERVAL[j]) && (twentyseventh)) {

            previousTtMillis = currentMillis;
    
            BRAKE_LIGHTS.off();                 
            
            twentyseventh = false;
            twentyeighth = true;
            j++;
          }

          //delay(1000);   INTERVAL 29

           // Plutonium empty loop
           // delay(400); INTERVAL 31
           
           if((currentMillis - previousTtMillis >= INTERVAL[j]) && (twentyeighth)) {

            previousTtMillis = currentMillis;

            P_EMPTY.on();            // Plutonium empty ON                          
            
            twentyeighth = false;
            twentyninth = true;
            
            j = 30;
          }


          //delay(500); INTERVAL 30


           if((currentMillis - previousTtMillis >= INTERVAL[j]) && (twentyninth)) {

            previousTtMillis = currentMillis;

            P_EMPTY.off();            // Plutonium empty OFF 

            if (pEmptyLoopCount++ < 6)
            {
               twentyninth = false; 
               twentyeighth = true;  
               j =31;
            }
            else
            {
              twentyninth = false;
              thirtieth = true;              
              j = 32;
              resetFluxTree();
            } 
          }       

          //delay(100); INTERVAL 32

          if((currentMillis - previousTtMillis >= INTERVAL[j]) && (thirtieth)) {

            previousTtMillis = currentMillis;

            pEmptyLoopCount = 0;   
                   
            first = true;
            STOP = true;           
            j = 0;
           }
    
       }while(!STOP);
    
       
    
          my_mp3_playend();            
          
          delay(1000);
    
          lightsOut();              // Turn all lights OFF
    
          speedo.clear();
          speedo.writeDisplay(); 
    
          TC_OFF.on();         // Time circuits OFF light on

          
    
          counter = 0; // reset speedo counter
    
          speedoInterval = speedoIntervalSettingOne; // Set speedo interval to original setting
                  
          STOP = false;  

          assSerial.write(MOV_DEMO_OFF_MSG);          
    }


    /*================END TIME TRAVEL MOVIE DEMO==================*/  



/*==============================================================================
 *        CHECK CAR SWITCHES ROUTINE                CHECK CAR SWITCHES ROUTINE        
 *==============================================================================*/
 
    //-----------------------------------------------------------------------
    void checkSwitches() {
      
      if (!ACCELERATOR_SW.readState())
      {
          accelerator();           
      }

      if(!appBrake)
      {
          if (!BRAKE_SW.readState())
          {
              BRAKE_LIGHTS.on();              
          }
          else
          {
              BRAKE_LIGHTS.off();          
          } //ブレーキ
      }
   
      
      if (!DOOR_SW_ONE.readState())
      {
          NEW_Door1=true;
          if ( !radioOn )
          {
              if (OLD_Door1==false)
              {
                  my_mp3_play(1,10);       // ドア音
                  delay(100);
                  if (SW_ST[1] == 1)
                  {
                      my_mp3_playend();
                      my_mp3_play(1,5);
                  }  // エンジン音再開
               }
           }
       }
       else
       {
          NEW_Door1=false;
       }
    
      if (!DOOR_SW_TWO.readState())
      {
          NEW_Door2=true;
          if ( !radioOn )
          {
              if (OLD_Door2==false)
              {
                  my_mp3_play(1,10);       // ドア音
                  delay(100);
                  if (SW_ST[1] == 1)
                  {
                      my_mp3_playend();
                      my_mp3_play(1,5);
                  }  // エンジン音再開
               }
          }
       }
       else
       {
          NEW_Door2=false;
       }
              
        OLD_Door1=NEW_Door1;
        OLD_Door2=NEW_Door2;
      
        if ( NEW_Door1 == true)
        {
            DOOR_1_LIGHTS.on();
        }
        else 
        {
            DOOR_1_LIGHTS.off();
        } //ドア１
    
        if ( NEW_Door2 == true)
        {
            DOOR_2_LIGHTS.on();
        }
        else 
        {
            DOOR_2_LIGHTS.off();
        } //ドア２

        if ( NEW_Door1 || NEW_Door2 )
        {
          if(doorsClosed)
          {
             COURTESY_LIGHT.on();
             sendDashLight(DOOR_OPEN);
             doorsClosed = false;
          }
                    
        }
        else
        {
          if(!doorsClosed)
          {
              COURTESY_LIGHT.off();
              sendDashLight(DOORS_CLOSED);
              doorsClosed = true;
          }
       }
          
      if (TRUNK_SW.readState())
      {
          if (FRONT_HOOD_LIGHT.readState())
          {
              FRONT_HOOD_LIGHT.on();
          }
      }
      else
      {
          FRONT_HOOD_LIGHT.off();
      } //トランク
    
        


      if (ENGINE_SPACE.readState())
      {
          if (!ENGINE_SPACE_LIGHT.readState())
          {
              ENGINE_SPACE_LIGHT.on();                
          }
      }
      else
      {
          ENGINE_SPACE_LIGHT.off();
      } //エンジンルーム
      
    }
    //-----------------------------------------------------------------------


/*==============================================================================
 *        FLUX CAPACITOR ROUTINE                FLUX CAPACITOR ROUTINE        
 *==============================================================================*/

    /*==================UPDATE FLUX CAPACITOR=======================*/

      void updateFluxCap()
      {                 
          LedSw++;
          if (LedSw > 4) {
            LedSw = 1;
          }
      
          if (LedSw == 1) 
          {
            FC_ONE.level(LedBright[fluxLevel]);
          } else {
            FC_ONE.off();
          }
          if (LedSw == 2) 
          {
            FC_TWO.level(LedBright[fluxLevel]);
          } else {
            FC_TWO.off();
          }
          if (LedSw == 3) 
          {
            FC_THREE.level(LedBright[fluxLevel]);
          } else {
            FC_THREE.off();
          }
          if (LedSw == 4) {
            FC_FOUR.level(LedBright[fluxLevel]);
          } else {
            FC_FOUR.off();
          }
          if (fluxLevel > 4) {
            if (s4Count++ > s4CountMax) {
              s4Count = s4CountMax;
              FC_MID.on();
            }
          } else {
            FC_MID.off();
          }
      
          if (fluxLevel > 6) {
            FLUX_BAND.on();
          } else {
            FLUX_BAND.off();
          }             
      }

    /*==================END UPDATE FLUX CAPACITOR=======================*/

/*==============================================================================
 *  SPECTRUM INDICATOR DISPLAY ROUTINES       SPECTRUM INDICATOR DISPLAY ROUTINES 
 *==============================================================================*/    

    /*==================UPDATE LEVEL ONE MATRIX=======================*/

    void updateLevelOneMatrix()
    {
      for (int8_t x = 0; x < 8; x++) {
          i = random(RndStart[fluxLevel], RndEnd[random(1, 4)]);
          matrixOne.drawLine(x, 0, x, 15, 0);   // Clear the column      
          if (fluxLevel > 0) {
            matrixOne.drawLine(x, 0, x, i, LED_ON);
          }
        }
        matrixOne.writeDisplay();  // write the changes we just made to the display        
    }
    /*==================END UPDATE LEVEL ONE MATRIX=======================*/
    

    /*==================UPDATE COLUMN ONE SEVEN MATRIX=======================*/

    void updateColumnOneSevenMatrix()
    {
      if(fluxLevel < 4)
        {
          if(colOneSevenCnt++ > colOneSevenCntMax)
          {
            colOneSevenCnt = 0;
            colOne++;
            colSeven++;
            if(colOne > 7)
            {
              colOne = 7;
              colSeven = 7;
            }
          }
        }
        
        matrixOne.drawPixel( 1, colOne, LED_ON);
        matrixOne.drawPixel( 7, colSeven, LED_ON);
        matrixOne.writeDisplay(); 

        if(fluxLevel > 3)
        {
          i = fluxLevel - 2;    
          if(colOne++ > matrixColumn[i][1]) {colOne = matrixColumn[i][1];}
          if(colSeven++ > matrixColumn[i][7]) {colSeven = matrixColumn[i][7];}               
        }
    }

    /*==================END UPDATE COLUMN ONE SEVEN MATRIX=======================*/


    /*==================UPDATE COLUMN THREE SIX MATRIX=======================*/

    void updateColumnThreeSixMatrix()
    {
      if(fluxLevel > 2 && fluxLevel < 5)
        {
          if(colThreeSixCnt++ > colThreeSixCntMax)
          {
            colThreeSixCnt = 0;
            colThree++;
            colSix++;
            if(colThree > 7)
            {
              colThree = 7;
              colSix = 7;
            }
          }
        }
    
        matrixOne.drawPixel( 6, colSix, LED_ON);
        matrixOne.drawPixel( 3, colThree, LED_ON);
        matrixOne.writeDisplay();

        if(fluxLevel > 4)
        {
          i = fluxLevel - 2;    
          if(colSix++ > matrixColumn[i][6]) {colSix = matrixColumn[i][6];}
          if(colThree++ > matrixColumn[i][3]) {colThree = matrixColumn[i][3];}
        }
    }

    /*==================END UPDATE COLUMN THREE SIX MATRIX=======================*/


    /*==================UPDATE COLUMN ZERO FOUR MATRIX=======================*/

    void updateColumnZeroFourMatrix()
    {
       if(fluxLevel > 2 && fluxLevel < 4)
        {
          if(colZeroFourCnt++ > colZeroFourCntMax)
          {
            colZeroFourCnt = 0;
            colZero++;
            colFour++;
            if(colZero > 9)
            {
              colZero = 9;
              colFour = 9;
            }
          }
        }
    
        matrixOne.drawPixel( 0, colZero, LED_ON);
        matrixOne.drawPixel( 4, colFour, LED_ON);
        matrixOne.writeDisplay();

        if(fluxLevel > 4 && fluxLevel < 7)
        {
          i = fluxLevel - 2;    
          if(colZero++ > matrixColumn[i][0]) {colZero = matrixColumn[i][0];}
          if(colFour++ > matrixColumn[i][4]) {colFour = matrixColumn[i][4];}
        }
    }

    /*==================END UPDATE COLUMN ZERO FOUR MATRIX=======================*/


    /*==================UPDATE COLUMN TWO FIVE MATRIX=======================*/

    void updateColumnTwoFiveMatrix()
    {
      if(fluxLevel > 4 && fluxLevel < 6)
        {
          if(colTwoFiveCnt++ > colTwoFiveCntMax)
          {
            colTwoFiveCnt = 0;
            colTwo++;
            colFive++;
            if(colTwo > 6)
            {
              colTwo = 6;
              colFive = 6;
            }
          }
        }
        matrixOne.drawPixel( 5, colFive, LED_ON);
        matrixOne.drawPixel( 2, colTwo, LED_ON);
        matrixOne.writeDisplay();

        if(fluxLevel > 5 && fluxLevel < 7)
        {
          i = fluxLevel - 2;    
          if(colFive++ > matrixColumn[i][5]) {colFive = matrixColumn[i][5];}
          if(colTwo++ > matrixColumn[i][2]) {colTwo = matrixColumn[i][2];}
        }
    }

    /*==================END UPDATE COLUMN TWO FIVE MATRIX=======================*/

    /*==================RESET MATRIX ONE========================================*/
    void resetFluxTree() {
         resetFlux();
         fluxLevel = 0;
         sendReactorMode(fluxLevel);
         resetTree();
         
    }

    /*==================END RESET MATRIX ONE====================================*/

    /*==================RESET FLUX====================================*/

    void resetFlux() {
      fluxSwitch = false;
      FC_ONE.off();
      FC_TWO.off();
      FC_THREE.off();
      FC_FOUR.off();
      FC_MID.off();
      FLUX_BAND.off();  
      s4Count = 0;       
    }

    /*==================END RESET FLUX===================================*/

    /*==================RESET TREE====================================*/

    void resetTree() {
       matrixOne.clear();
       matrixOne.writeDisplay();
       colZero  = 0;
       colOne   = 0;
       colTwo   = 0;
       colThree = 0;
       colFour  = 0;
       colFive  = 0;
       colSix   = 0;
       colSeven = 0;  
       colOneSevenCnt = 0;
       colZeroFourCnt = 0;
       colThreeSixCnt = 0;
       colTwoFiveCnt = 0;        
    }

    /*==================END RESET TREE====================================*/


    

 /*==============================================================================
 *        GET APP INSTRUCTIONS      GET APP INSTRUCTIONS       GET APP INSTRUCTIONS
 *==============================================================================*/

    void getAppInstr()
    {
      byte c = 0;

      if (assSerial.available())
      {
        c = assSerial.read();        
      }

      switch (c)
      {
        case 100:  // Power OFF
          SYSTEM_ON = false;
          powerOff();                    
          break;
     
        case 101:  // Power ON
          SYSTEM_ON = true;
          powerOn();          
          break;
     
        case 103:  // Volume UP
          my_mp3_volup();
          delay(100);
          break;
     
        case 105:  // Radio NEXT
          radioNext();
          break;
     
        case 106:  // Time Circuits OFF
          timeCircuitsOff();
          break;

        case 107:  // Time Circuits ON
          timeCircuitsOn();
          break;
     
        case 109:  // Movie Demo
          switchOffMultipleFunctions();
          timeTravelMovieDemo();                      
          break;
     
        case 111:  // Radio PREV
          radioPrev();
          break;
     
        case 113:  // Lights ON
          lightsOn();
          break;
     
        case 114:  // Volume DOWN
          my_mp3_voldown();
          delay(100);
          break;
     
        case 116:  // Lights OFF
          lightsOff();
          break;
     
        case 118:  // Roof light OFF
          roofLightOn = false;
          ROOF_LIGHT.off();
          assSerial.write(RF_LGHT_OFF_MSG);
          break;

        case 119:  // Roof light ON
          roofLightOn = true;
          ROOF_LIGHT.on();
          assSerial.write(RF_LGHT_ON_MSG);
          break;
     
        case 121:  // Passing lights
          passingLights(); 
          break;
     
        case 123:  // Horn
          horn();
          break;
     
        case 124:  // Engine STOP
          engineStop();
          break;
     
        case 125:  // Engine START
          engineStart();
          break;
     
        case 126:  // BRAKE_LIGHTS OFF
          brakeLightsOn = false;
          SW_ST[2] = 0;
          BRAKE_LIGHTS.off();
          assSerial.write(BRAKE_LIGHTS_OFF_MSG);
          appBrake = false;
          break;
     
        case 127:  // BRAKE_LIGHTS ON
          brakeLightsOn = true;
          appBrake = true;
          SW_ST[2] = 1;
          BRAKE_LIGHTS.on();
          assSerial.write(BRAKE_LIGHTS_ON_MSG);
          break;
     
        case 129:  // Accelerator
          accelerator();          
          break;
     
        case 130:  // Left Indicator OFF
          leftIndOff();
          break;
     
        case 131:  // Left Indicator ON
          leftIndOn();
          break;
     
        case 132:  // Hazard Lights OFF
          hazardLightsOff();
          break;

        case 133:  // Hazard Lights ON
          hazardLightsOn();
          break;

        case 134:  // Right Indicator OFF
          rightIndOff();
          break;
     
        case 135:  // Right Indicator ON
          rightIndOn();
          break;

        case 136:  // REVERSE_LIGHTS Lights OFF
          reverseLightsOn = false;
          REVERSE_LIGHTS.off();
          assSerial.write(REV_LGHT_OFF_MSG);
          break;
     
        case 137:  // REVERSE_LIGHTS Lights ON
          reverseLightsOn = true;
          REVERSE_LIGHTS.on();
          assSerial.write(REV_LGHT_ON_MSG);
          break;

        case 139:  // Time Travel
          switchOffMultipleFunctions();
          time_travel();
          break;

        case 140:  // Radio Stop
          radioStop();
          break;
     
        case 141:  // Radio Play
          radioPlay();
          break;
          
        default:

        break;
      }
    }


    /*===================END GET APP INSTRUCTION====================*/

    /****************************************************************
     * ==================I2C FUNCTIONS===============================
     ***************************************************************/
     
    void send_flux(int h) {
    byte c;
      c=h;
      Wire.beginTransmission(I2C_FIRE_ADDRESS);   // slave device #9
      Wire.write(c);
      Wire.endTransmission();
    }

    void sendDashLight(int d) {
    byte c;
      c=d;
      Wire.beginTransmission(I2C_DASH_ADDRESS);   // slave device #8
      Wire.write(c);
      Wire.endTransmission();
    }

    void sendReactorMode(int r) {
    byte c;
      c=r;
      Wire.beginTransmission(I2C_REACTOR_ADDRESS);   // slave device #7
      Wire.write(c);
      Wire.endTransmission();
    }


    void entryFlash() {
           DASH_BACKLIGHT.on();
           PLUTONIUM_BACKLIGHT.on();
           TCD.on();
           TC_OFF.on();         
           TC_ON.on();            
           RED_AMBER_GREEN_KEYPAD.on();
           WHITE_KEYPAD.on();
           PSU.on();           
           L_BLINK.on();
           R_BLINK.on();
           DIPPED_LIGHTS.on(); //Main beam, rear lights, licence plate ON
           F_BEAM.on();
           L_SIDE.on(); //Left side light ON
           R_SIDE.on(); //Right side light ON 
           REAR_LIGHTS.on();
           PLATE_LIGHTS.on();           
           EIGHTY8_MPH.on();
           speedo.println(8888);
           speedo.drawColon(true);
           speedo.writeDisplay();   
           matrixOne.drawBitmap(0, 0, fill_bmp, 15, 15, LED_ON);
           matrixOne.writeDisplay();
           FC_ONE.on();
           FC_TWO.on();
           FC_THREE.on();
           FC_FOUR.on();          
           FC_MID.on();
           FLUX_BAND.on();        
           sendDashLight(LIGHTS_ON);
           
           delay(200);

           DASH_BACKLIGHT.off();
           PLUTONIUM_BACKLIGHT.off();
           TCD.off();
           TC_OFF.off();         
           TC_ON.off();
           RED_AMBER_GREEN_KEYPAD.off();
           WHITE_KEYPAD.off(); 
           PSU.off();          
           L_BLINK.off();
           R_BLINK.off();
           DIPPED_LIGHTS.off(); //Main beam, rear lights, licence plate OFF
           F_BEAM.off();
           L_SIDE.off(); //Left side light OFF
           R_SIDE.off(); //Right side light OFF
           REAR_LIGHTS.off();
           PLATE_LIGHTS.off();                      
           EIGHTY8_MPH.off();
           speedo.clear();
           speedo.writeDisplay(); 
           matrixOne.clear();
           matrixOne.writeDisplay(); 
           FC_ONE.off();
           FC_TWO.off();
           FC_THREE.off();
           FC_FOUR.off();       
           FC_MID.off();
           FLUX_BAND.off();           
           sendDashLight(LIGHTS_OFF);          
      }
    
      void lightsOut() {

          for(int off = 0; off < LEDMAX; off++)
          {
            D_PIN[off].off();
          }
           sendDashLight(LIGHTS_OFF);           
       }

/*==============================================================================
 *        SPEEDO ROUTINES      SPEEDO ROUTINES        SPEEDO ROUTINES
 *==============================================================================*/

       void B8_speedoAcceleration() {
          speedo.writeDigitNum(0, (counter / 1000), false);
          speedo.writeDigitNum(1, (counter / 100) % 10, true);
          //speedo.drawColon(true);
          speedo.writeDigitNum(3, (counter / 10) % 10, false);
          speedo.writeDigitNum(4, counter % 10, false);  
          speedo.writeDisplay();
          counter+=5;
        if(counter > 8800) {counter = 8800;}
      }
    
      void speedoAcceleration() {
          speedo.writeDigitNum(0, (counter / 1000), false);
          speedo.writeDigitNum(1, (counter / 100) % 10, true);
          //speedo.drawColon(true);
          speedo.writeDigitNum(3, (counter / 10) % 10, false);
          speedo.writeDigitNum(4, counter % 10, false);  
          speedo.writeDisplay();
          counter++;
        if(counter > 8800) {counter = 8800;}
      }
    
      void speedoDecceleration() {
     
          speedo.writeDigitNum(0, (counter / 1000), false);
          speedo.writeDigitNum(1, (counter / 100) % 10, true);          
          speedo.writeDigitNum(3, (counter / 10) % 10, false);
          speedo.writeDigitNum(4, counter % 10, false);      
          speedo.writeDisplay();
          counter-=3;      
        
        if(counter < 100) {
          counter = 0;
          speedo.writeDigitNum(0, (counter / 1000), false);
          speedo.writeDigitNum(1, (counter / 100) % 10, true);          
          speedo.writeDigitNum(3, (counter / 10) % 10, false);
          speedo.writeDigitNum(4, counter % 10, false);            
          speedo.writeDisplay();
          speedoReturn = false;         
        }
        
      }
