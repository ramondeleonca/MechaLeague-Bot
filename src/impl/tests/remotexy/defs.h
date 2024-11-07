/*
   -- LA CHACALOSA --
   
   This source code of graphical user interface 
   has been generated automatically by RemoteXY editor.
   To compile this code using RemoteXY library 3.1.13 or later version 
   download by link http://remotexy.com/en/library/
   To connect using RemoteXY mobile app by link http://remotexy.com/en/download/                   
     - for ANDROID 4.15.01 or later version;
     - for iOS 1.12.1 or later version;
    
   This source code is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.    
*/

//////////////////////////////////////////////
//        RemoteXY include library          //
//////////////////////////////////////////////

// you can enable debug logging to Serial at 115200
//#define REMOTEXY__DEBUGLOG    

// RemoteXY select connection mode and include library 
#define REMOTEXY_MODE__WIFI_POINT

#include <WiFi.h>

// RemoteXY connection settings 
#define REMOTEXY_WIFI_SSID "LA CHACALOSA"
#define REMOTEXY_WIFI_PASSWORD "lachacalosabuscasuchacal"
#define REMOTEXY_SERVER_PORT 6377


#include <RemoteXY.h>

// RemoteXY GUI configuration  
#pragma pack(push, 1)  
uint8_t RemoteXY_CONF[] =   // 78 bytes
  { 255,4,0,51,0,71,0,19,0,0,0,76,65,32,67,72,65,67,65,76,
  79,83,65,0,25,1,200,84,1,1,4,0,67,76,26,52,9,1,2,51,
  5,6,25,51,51,32,2,26,31,5,142,25,51,51,32,2,26,31,129,56,
  5,92,12,64,16,76,65,32,67,72,65,67,65,76,79,83,65,0 };
  
// this structure defines all the variables and events of your control interface 
struct {

    // input variables
  int8_t leftx; // from -100 to 100
  int8_t lefty; // from -100 to 100
  int8_t rightx; // from -100 to 100
  int8_t righty; // from -100 to 100

    // output variables
  char message[51]; // string UTF8 end zero

    // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0

} RemoteXY;   
#pragma pack(pop)
 
/////////////////////////////////////////////
//           END RemoteXY include          //
/////////////////////////////////////////////