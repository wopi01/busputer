/***************************************************
 *  This code is based on the U8G2 library from olikraus.
 *  GIT: https://github.com/olikraus/u8g2
 *       https://github.com/olikraus/U8g2_Arduino
 *  Wiki: https://github.com/olikraus/u8g2/wiki
 *
 *  Author: Brun von der Gönne
 *
 *  This module will only used by 32Bit processors.
 * 
 ****************************************************/


#ifdef DISPLAY
#ifdef ARDUINO_ARCH_SAMD
#ifdef DOGS102

//Menu defination
#define MENU_clock 1
#define MENU_speed 2
#define MENU_trip 3
#define MENU_gpsinfo 4
#define MENU_power 5
#define MENU_sdcard 6
#define MENU_water_temp 7
#define MENU_clima 8
#define MENU_info 9
#define MENU_
#define MENU_custom 10


#define SubMenu1Nr 101
#define SubMenu1 menu_sub_1
#define SubMenu2Nr 102
#define SubMenu2 menu_sub_2
#define SubMenu3Nr 104
#define SubMenu3 menu_gpsinfos_sub1



/*
 *  https://github.com/olikraus/u8glib/wiki/fontsize
 *  ATTENTION!!! Fonts need space
 */

#define small_font u8g2_font_profont10_tf //2311
#define medium_font u8g2_font_profont12_tf //2311
#define big_font u8g2_font_profont29_mn //2719

// this defines the starting possition.
#define Xpos 0
#define Ypos 16


int last_menu = 0;


void display_init(void) {
  if ( !SPI_lock ) {
    SPI_lock = true;
    u8g2.begin();
    //u8g2.setContrast(220);
    pinMode(DISPLAY_BG_LED, OUTPUT);
    analogWrite(DISPLAY_BG_LED, 128);
    SPI_lock = false;

    digitalWrite(DIMMER, HIGH);
    
  }
}

void display_draw(void) {

  u8g2.setFont(small_font);
  
  u8g2.setFontPosTop();
  u8g2.setCursor(Xpos, Ypos);

  switch (MainMenuPos) {
    case 0: bootscreen(); break;
    case MENU_clock: menu_clock(); break; // 1
    case MENU_speed: menu_speed(); break; // 2
    case MENU_trip: menu_trip(); break; // 3
    case MENU_gpsinfo: menu_gpsinfos(); break; // 4
    case MENU_power: menu_power(); break; // 5
    case MENU_sdcard: menu_sdcard(); break; // 6
    case MENU_water_temp: menu_water_temp(); break; // 7
    case MENU_clima: menu_clima(); break; // 8
    case MENU_info: menu_info(); break; // 8
    
    #ifdef CUSTOM
    case MENU_custom: custom_menu(); break; // display the custom menu
    #endif // CUSTOM

    default: {
      menu_clock();
      MainMenuPos = 1;
      break;
    }
  }
}


void display_set_led() {
  int mood = 0;
  
  //calculate Dimmer
  mood = analogRead(DIMMER);
  mood *= 15.2; // convert to mV
  //Serial.println(dimmer);
  mood -= DIMMER_MIN_mV;
  //Serial.println(dimmer);

  //int dimmer_ranger = DIMMER_MAX_mV - DIMMER_MIN_mV;
  int mood_pct = ((100000 / (DIMMER_MAX_mV - DIMMER_MIN_mV)) * mood) / 1000;
  //Serial.println(dimmer_pct);

  
  if ( mood_pct < 0 ) mood_pct= 0;

  int led = DOGS102_LED_MIN + (((DOGS102_LED_MAX - DOGS102_LED_MIN) / 10 ) * float(mood_pct)/10) ;
  //Serial.println(lcd_led);
  if ( mood_pct > 100 ) led = DOGS102_LED_MAX;
  analogWrite(DISPLAY_BG_LED, led); 
}



/*
 * Display MENUs
 */
void bootscreen() {
  //TRACE_PRINTLN(F("#->bootscreen"));
  //u8g.drawXBMP( 48, 0, vw_width, vw_height, vw_bits);
  u8g2.setFontPosTop();
  //u8g2.setCursor(24, 0);
  u8g2.setCursor(Xpos, Ypos);
  u8g2.print(bootmsg1);
  //TRACE_PRINTLN(bootmsg1);
  u8g2.setCursor(Xpos, Ypos + 8);
  u8g2.print(bootmsg2);
  u8g2.setCursor(Xpos, Ypos + 16);
  u8g2.print(bootmsg3);

}

void menu_speed() {
  #ifdef SPEED
  char buf[24]; //???

  // Speed
  u8g2.setFont(big_font);
  u8g2.setFontPosTop();
  //u8g2.setCursor(36, -2);
  u8g2.setCursor(Xpos + 18, Ypos);
  if (gps_success) {
    if (gps_speed >= 5) {
      sprintf (buf, "%3d", gps_speed + SPEED_OFFSET);
      u8g2.print(buf);
    }
    else {
      u8g2.print("  0");
    }
    //u8g2.print(buf);
  }
  else {
    u8g2.print(F("---"));
  }

  u8g2.setFont(small_font);
  u8g2.setFontPosTop();
  u8g2.setCursor(Xpos + 74, Ypos);
  u8g2.print(F("km/h"));

  //Distance
  /*u8g2.setFont(medium_font);
    u8g2.setFontPosTop();
    u8g2.setCursor(Xpos, Ypos + 24);
    //u8g.print(F("dist:"));
    uint16_t m = gps_distance - ( gps_distance/1000 ) * 1000;
    sprintf (buf, "%04d.%1d km", gps_distance/1000, m/100);
    u8g2.print(buf);*/

  //Temperature
  u8g2.setFont(medium_font);
  u8g2.setFontPosTop();
  u8g2.setCursor(Xpos, Ypos + 24);
  //u8g.print(F("dist:"));
  //uint16_t m = temp_out * 10;
  uint16_t m = temp_out - ( temp_out/10 ) * 10;
  sprintf (buf, "%02d.%1d", temp_out/10, m);
  //u8g2.print(F("\xb0:"));

  u8g2.print(buf);
  u8g2.print(F("\xb0"));
  u8g2.print(F("C"));

  //time
  u8g2.setFontPosTop();
  u8g2.setCursor(Xpos + 68, Ypos + 24);
  sprintf(buf, "%02d:%02d", hour, minute);
  u8g2.print(buf);

  switch (button_1) {
    case 1: MainMenuPos++; break;
    case 2: MainMenuPos--; break;
  }
  button_1 = 0;
  #else //SPEED
  MainMenuPos++;
  #endif // SPEED
}

/*
 * simple clock with date
 */
void menu_clock() {
  char buf[24];

  u8g2.setFont(big_font);
  u8g2.setFontPosTop();
  u8g2.setCursor(Xpos + 10, Ypos);
  sprintf(buf, "%02d:%02d", hour, minute);
  u8g2.print(buf);

  u8g2.setFont(medium_font);
  u8g2.setFontPosTop();
  u8g2.setCursor(Xpos + 22, Ypos + 24);
  sprintf(buf, "%02d.%02d.%04d", day, month, year);
  u8g2.print(buf);

  switch (button_1) {
    case 1: MainMenuPos++; break;
    case 2: MainMenuPos--; break;
  }
  button_1 = 0;
}


/*
 * GPS informations
 */
void menu_gpsinfos() {
  #ifdef FONA
  char buf[24]; //???

  u8g2.setFont(small_font);
  u8g2.setFontPosTop();
  u8g2.setCursor(Xpos, Ypos);
  u8g2.print(F("GPS Infos: "));
  u8g2.setCursor(Xpos, Ypos + 8);
  u8g2.print(F("Lat : "));
  u8g2.print(gps_latitude, 5);
  u8g2.setCursor(Xpos, Ypos + 16);
  u8g2.print(F("Long: "));
  u8g2.print(gps_longitude, 5);
  u8g2.setCursor(Xpos, Ypos + 24);
  u8g2.print(F("Alt : "));
  u8g2.print(gps_altitude, DEC);
  u8g2.setCursor(Xpos, Ypos + 32);
  u8g2.print(F("Sattalites: "));
  u8g2.print(gps_view_satellites, DEC);
  u8g2.print(F(" ( "));
  u8g2.print(gps_used_satellites, DEC);
  u8g2.print(F(" )"));

  switch (button_1) {
    case 1: MainMenuPos++; break;
    case 2: MainMenuPos = SubMenu3Nr; break;
  }
  button_1 = 0;
  #else // FONE
  MainMenuPos++;
  #endif // FONA

}

void menu_speedinfos() {
  #ifdef FONA
  char buf[24];

  u8g2.setFont(medium_font);
  u8g2.setFontPosTop();
  u8g2.setCursor(Xpos, Ypos);
  u8g2.print(F("Avg. Speed: "));
  u8g2.print(gps_speed_avg, DEC);
  u8g2.setCursor(Xpos, Ypos + 10);
  u8g2.print(F("Avg. Today: "));
  //u8g2.print(gps_speed_avg_today, DEC);
  u8g2.setCursor(Xpos, Ypos + 20);
  u8g2.print(F("Max. Speed: "));
  u8g2.print(gps_speed_max, DEC);

  switch (button_1) {
    case 1: MainMenuPos++; break;
      //case 2: MainMenuPos = SubMenu3Nr; break;
  }
  button_1 = 0;
  #else //FONA
  MainMenuPos++;
  #endif //FONA

}

void menu_trip() {
  #ifdef FONA
  char buf[24]; //???

  u8g2.setFont(small_font);
  u8g2.setFontPosTop();
  u8g2.setCursor(Xpos, Ypos);
  u8g2.print(F("Trip Infos: "));
  u8g2.setCursor(Xpos, Ypos + 8);
  //u8g2.print(gps_trip_distance/1000, DEC);
  u8g2.print(F("\x97\x3e "));
  uint16_t m = gps_distance_trip - ( gps_distance_trip / 1000 ) * 1000;
  sprintf (buf, "%04d.%1d km", gps_distance_trip / 1000, m / 100);
  //u8g2.print(F(" km "));

  u8g2.print(buf);
  u8g2.setCursor(Xpos, Ypos + 16);
  u8g2.print(F("\xf8 "));
  u8g2.print(gps_speed_avg_trip, DEC);
  u8g2.print(F("km/h"));

  u8g2.setCursor(Xpos + 48, Ypos + 16);
  u8g2.print(F("\x86:"));
  u8g2.print(gps_speed_max_trip, DEC);
  u8g2.print(F("km/h"));

  u8g2.setCursor(Xpos, Ypos + 24);
  u8g2.print(engine_running_trip_last + engine_running_sec, DEC);

  switch (button_1) {
    case 1: MainMenuPos++; break;
    case 2:
      //gps_distance = 0;
      gps_distance_trip = 0;
      gps_speed_max_trip = 0;
      //gps_speed_trip_avg = 0;
      engine_running_trip_last = 0 - engine_running_sec;
      break;
  }
  button_1 = 0;
  #else //FONA
  MainMenuPos++;
  #endif //FONA

}

void menu_sdcard() {
  #ifdef SDCARD
  char buf[24];

  u8g2.setFont(small_font);
  u8g2.setFontPosTop();
  u8g2.setCursor(Xpos, Ypos);
  u8g2.print(F("SD Card Infos: "));

  u8g2.setCursor(Xpos, Ypos + 8);
  u8g2.print(F("Card: "));
  if ( SDmount && !SDerror) {
    u8g2.print(F("ready"));
  }
  else {
    u8g2.print(F("not ready"));
  }

  u8g2.setCursor(Xpos, Ypos + 16);
  u8g2.print(F("File: "));
  u8g2.print(filename);

  switch (button_1) {
    case 1: MainMenuPos++; break;
    case 2:
      button_1 = 0;
      if (SDmount) {
        close_file();
        SDmount = false;
      } else {
        MainMenuPos = 0;
        Serial.println(F("#remount_sdcard"));
        SDmount = false;
        delay(1000);
        enable_sdcard();
        MainMenuPos = 7;
      }
      break;
  }
  button_1 = 0;
  #else //SDCard
  MainMenuPos++;
  #endif //SDCARD
}

void menu_clima() {
  char buf[24];
  int m = 0;

  u8g2.setFont(small_font);
  u8g2.setFontPosTop();
  u8g2.setCursor(Xpos, Ypos);
  u8g2.print(F("Klima:"));

  #ifdef TEMPERATURE_OUT
  u8g2.setCursor(Xpos, Ypos + 8);
  u8g2.print(F("Aussen: "));
 
  //uint16_t m = temp_out * 10;
  m = temp_out - ( temp_out/10 ) * 10;
  sprintf (buf, "%02d.%1d", temp_out/10, m);
  u8g2.print(buf);
  u8g2.print(F("\xb0"));
  #endif // TEMP_OUT
  
  #ifdef HUMIDITY_OUT
  u8g2.print(F(" ("));
  sprintf (buf, "%02d", hum_out);
  u8g2.print(buf);
  u8g2.print(F("%)"));
  #endif // HUMIDITY_OUT
  
  #ifdef TEMPERATURE_IN
  u8g2.setCursor(Xpos, Ypos + 16);
  u8g2.print(F("Innen : "));
  
  m = temp_in - ( temp_in/10 ) * 10;
  sprintf (buf, "%02d.%1d", temp_in/10, m);
  u8g2.print(buf);
  u8g2.print(F("\xb0"));
  #endif // TEMP_IN
  
  #ifdef HUMIDITY_IN
  u8g2.print(F(" ("));
  sprintf (buf, "%02d", hum_in);
  u8g2.print(buf);
  u8g2.print(F("%)"));
  #endif // HUMIDITY_IN


  
  

  switch (button_1) {
    case 1: MainMenuPos++; break;
      //case 2: MainMenuPos = SubMenu3Nr; break;
  }
  button_1 = 0;
  //#else //FONA
  //MainMenuPos++;
  //#endif //FONA
}

void menu_water_temp() {
  #ifdef VW_WATER_TEMP
  char buf[24];
  u8g2.setFont(small_font);
  u8g2.setFontPosTop();
  u8g2.setCursor(Xpos, Ypos);
  u8g2.print(F("Wasser Temperatur:"));

  u8g2.setFont(big_font);
  u8g2.setCursor(Xpos+16, Ypos+32);
  //Temperature
  //uint16_t m = temp_out - ( temp_out/10 ) * 10;
  sprintf (buf, "%3d", water_temp);
  //u8g2.print(F("\xb0:"));
  u8g2.print(buf);
  u8g2.setFont(medium_font);
  u8g2.setCursor(Xpos+64, Ypos+22);
  u8g2.print(F("\xb0"));
  u8g2.print(F("C"));

  //u8g2.setCursor(Xpos, Ypos+16);
  //u8g2.print(water_temp_V, 1);
  //u8g2.print(F(" mV"));

  switch (button_1) {
    case 1: MainMenuPos++; break;
      //case 2: MainMenuPos = SubMenu3Nr; break;
  }
  button_1 = 0;
  #else //VW_WATER_TEMP
  MainMenuPos++;
  #endif //VW_WATER_TEMP
  
  
}
void menu_gpsinfos_sub1() {
  #ifdef FONA
  char buf[24];

  u8g2.setFont(small_font);
  u8g2.setFontPosTop();
  u8g2.setCursor(Xpos, Ypos);
  u8g2.print(F("Satellites"));
  u8g2.setCursor(Xpos + 6, Ypos + 8);
  u8g2.print(F("view  : "));
  u8g2.print(gps_view_satellites, DEC);
  u8g2.setCursor(Xpos + 6, Ypos + 16);
  u8g2.print(F("used  : "));
  u8g2.print(gps_used_satellites, DEC);
  u8g2.setCursor(Xpos + 6, Ypos + 24);
  u8g2.print(F("status: "));
  //u8g2.print(gps_fixstatus, DEC);
  if (gps_fixstatus == 1) {
    u8g2.print(F("fixed"));
  } else {
    u8g2.print(F("not fixed"));
  }
  //u8g2.print(F(")"));

  switch (button_1) {
    case 1: MainMenuPos = 3; break;
      //case 2: MainMenuPos = 3; break;
  }
  button_1 = 0;
  #endif // FONA
}


void menu_power() {
  char buf[24];

  u8g2.setFont(small_font);
  u8g2.setFontPosTop();
  u8g2.setCursor(Xpos, Ypos);
  u8g2.print(F("Bordspannung:"));
  u8g2.setFont(big_font);
  u8g2.setFontPosTop();
  u8g2.setCursor(Xpos + 10, Ypos + 10);
  u8g2.print(bord_voltage, 1);
  u8g2.setFont(medium_font);
  u8g2.setFontPosTop();
  u8g2.setCursor(Xpos + 80, Ypos + 20);
  u8g2.print("V");

  switch (button_1) {
    case 1: MainMenuPos++; break;
    case 2: MainMenuPos--; break;
  }
  button_1 = 0;
}
void menu_dimmer() {
  /*char buf[24];

  u8g2.setFont(small_font);
  u8g2.setFontPosTop();
  u8g2.setCursor(Xpos, Ypos);
  u8g2.print(F("Dimmer Pref"));
  sprintf(buf, "%3d%%", fona_batt);
  u8g2.print(buf);
  u8g2.setCursor(Xpos, Ypos + 10);
  u8g2.print(F("Hold Button to change"));
  //sprintf(buf, "%4d%V", X_pin);
  u8g2.print(bord_voltage, 2);
  u8g2.setCursor(Xpos, Ypos + 16);

  switch (button_1) {
    case 1: MainMenuPos++; break;
    case 2: MainMenuPos--; break;
  }
  button_1 = 0;*/
}
void menu_info() {
  char buf[24];

  u8g2.setFont(small_font);
  u8g2.setFontPosTop();
  u8g2.setCursor(Xpos, Ypos);
  u8g2.print(F("T3 bord Computer"));
  u8g2.setCursor(Xpos, Ypos + 8);
  u8g2.print(F("Designed by Brun"));
  u8g2.setCursor(Xpos, Ypos + 16);
  u8g2.print(F("Version: "));
  u8g2.print(F(VERSION));
  #ifdef FONA
  u8g2.setCursor(Xpos, Ypos + 24);
  u8g2.print(F("Bckp Batt:"));
  sprintf(buf, "%3d%%", fona_batt);
  u8g2.print(buf);
  #endif // FONA

  /*if (button_1 == 2 ) {
    MainMenuPos = SubMenu1Nr;
    button_1 = 0;
    last_menu = 90;
    }*/

  switch (button_1) {
    case 1: MainMenuPos++; break;
    case 2: MainMenuPos--; break;
  }
  button_1 = 0;
}

void menu_sub_1() {
  char buf[24];

  u8g2.setFont(medium_font);
  u8g2.setFontPosTop();
  u8g2.setCursor(Xpos, Ypos);
  u8g2.print(F("Sub Menue 1"));

  switch (button_1) {
    case 1: MainMenuPos++; break;
    case 2: MainMenuPos--; break;
  }
  button_1 = 0;
}
void menu_sub_2() {
  char buf[24];

  u8g2.setFont(medium_font);
  u8g2.setFontPosTop();
  u8g2.setCursor(Xpos, Ypos);
  u8g2.print(F("Sub Menue 2"));

  switch (button_1) {
    case 1: MainMenuPos++; break;
    case 2: MainMenuPos--; break;
  }
  button_1 = 0;
}




#endif // DOGS102
#endif // ARDUINO_ARCH_SAMD
#endif // DISPLAY
