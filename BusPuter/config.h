/****************************************************
 * Busputer Configuration
 * for detailed informations see 
 * https://github.com/brvdg/busputer/wiki
 ****************************************************/

// Enable Display
#define LCD
#define LCD_LED_MAX 255
#define LCD_LED_MIN 0

// Enable SD Card
#define SDCARD

// Enable Fona Libary
#define FONA

#define SMS_Keyword "PWD"
#define MyNumber "+4915141284285"
// enable Internet tracking
//#define tracking  // ???
//#define tracking_ON // enable permanent tracking ???
#define GPRS_APN "internet.t-mobile"
#define GPRS_user "t-mobile"
#define GPRS_pw "tm"
#define TRACKING_URL "brun.rhoencouch.de/gps.php?visor=false&latitude=%s&longitude=%s&altitude=%s&time=%s&satellites=%s&speedOTG=%s&course=%s"


// for Dalles DS18B20 Temperatursensor
#define ONEWIRE


// enable Watchdog
//#define WD

// enable Debugging
#define INFO
//#define DEBUG
//#define TRACE


// add an offset to the real speed
#define SPEED_OFFSET 5


// define which funkction whil be used for default variables
// Time
#define GPS_TIME // use GPS Time as default time
//#define DS1303_TIME //??
//Speed
#define GPS_SPEED
//#define SPEEDPULE_SPEED
// Distance
#define GPS_DISTANCE
//#define SPEEDPULSE_DISTANCE



#if defined (GPS_TIME) && defined (DS1303_TIME)
//#pragma message ( "Debug configuration - OK" )
#error "Only one time source is possible"
#endif

/*
 * Hardware configuration
 */

#define CUSTOMBOARD



/*
 * Custom defined configuration
 */

#ifdef CUSTOMBOARD
// LCD configuration
#define LCD_LED 11
#define LCD_CLOCK 12
#define LCD_DATA 13

// FONA
#define FONA_RST 5

// OneWire
#define ONE_WIRE_BUS 6

#define BUTTON_PIN_1 9
#define X_Kontakt A0
#define DIMMER A1

#define FeatherLED8 8

#endif // CUSTOMBOARD



// U8Glib

#ifdef LCD
#ifdef ARDUINO_ARCH_SAMD
#include <U8g2lib.h>
#else
#include <U8glib.h>
#endif //ARDUINO_ARCH_SAMD

#define LCD_UPDATE_TIMER 200 // 200ms
unsigned long lcd_update_timer = 0;
boolean lcd_update_timer_lock = false;

int MainMenuPos = 0;

#endif //LCD

// SD Card configuration
#define cardSelect 4
#define KMLLOG    //???
#ifdef SDCARD
//#include <SPI.h>
//#include <SD.h>
#include <SPI.h>
#include "SdFat.h"

SdFat SD;
SdFile logfile;

char filename[16];
boolean SDmount = false;
boolean SDerror = false;
uint16_t lastfile = 0;

#endif //SDCARD

// FONA Library configuration
#ifdef FONA
//#define SMS_Commands //???
#define SPEED //this enables speedinformations

//#ifdef FONA
#include "Adafruit_FONA.h"


#ifdef ARDUINO_ARCH_SAMD
//#include <avr/dtostrf.h>
HardwareSerial *fonaSerial = &Serial1;
Adafruit_FONA fona = Adafruit_FONA(FONA_RST);
#else //ARDUINO_ARCH_SAMD
//#include <stdlib.h>
#define FONA_RX 2
#define FONA_TX 3
#define FONA_RST 4
#include <SoftwareSerial.h>
SoftwareSerial fonaSS = SoftwareSerial(FONA_TX, FONA_RX);
SoftwareSerial *fonaSerial = &fonaSS;
Adafruit_FONA fona = Adafruit_FONA(FONA_RST);
#endif //ARDUINO_ARCH_SAMD

//#define FONA_LOOP_TIME 10000 // 1s ???
#define FONA_GPS_TIMER 1000 // 1s
#define FONA_SMS_TIMER 5000 // 5s
#define FONA_BATT_TIMER 30000 // 30s
#define FONA_GPS_STATUS_TIMER 3000 // 3s
//Fona Variablen
unsigned long fona_gps_timer = 0;
unsigned long fona_sms_timer = 0;
unsigned long fona_batt_timer = 0;
unsigned long fona_gps_status_timer = 0;
boolean battstatus_send = false;

//SMS Alarm
boolean alarm_active = false;
char alarm_activator[32]; //number who activates the alarm
boolean alarmtatus_send = false;


int8_t gps_fixstatus;
int16_t gps_year;
uint8_t gps_day;
int8_t gps_month;
int8_t gps_hour;
int8_t gps_minute;
int8_t gps_second;
//float gps_date;
uint8_t gps_speed;
uint8_t gps_speed_max = 0;
uint8_t gps_speed_max_trip = 0;
uint8_t gps_speed_avg = 0;
uint8_t gps_speed_avg_trip = 0;
//uint8_t gps_speed_avg_start = 0;
//uint8_t gps_speed_avg_today = 0;
uint16_t gps_course;
uint16_t gps_altitude;
uint8_t gps_view_satellites;
uint8_t gps_used_satellites;
float gps_latitude, gps_longitude;
float gps_latitude_old = 0;
float gps_longitude_old = 0;
float gps_latitude_lasttrack = 0;
float gps_longitude_lasttrack = 0;
float gps_latitude_lastlog = 0;
float gps_longitude_lastlog = 0;
boolean gps_success = false;
boolean gps_fix = false;
uint32_t gps_distance_trip = 0;
uint32_t gps_distance = 0;
//uint32_t gps_distance_start = 0;
//uint32_t gps_distance_today = 0;

uint8_t fona_type;
char fona_time[23];
uint16_t fona_batt = 0;

String str_mode;
String str_fixstatus;
String str_utctime;
String str_latitude;
String str_longitude;
String str_altitude;
String str_speed;
String str_course;
String str_view_satellites;
String str_used_satellites;

String str_year;
String str_month;
String str_day;
String str_hour;
String str_minute;
String str_second;
char utc_time[15];

boolean gprs_tracking = false;
uint32_t gprs_lasttrack = 0; //???

#endif // FONA


/*
 * DS18B20 Temperatursensor
 */
//#define DS18B20_PIN 10
#define ONEWIRE_TIMER 10000 // 10s
unsigned long onewire_timer = 0;


/*
 *  Input/Output
 */

#define BUTTON_TIMER 200 // 200ms
unsigned long button_timer = 0;
bool button_timer_lock = false;

uint8_t button_1 = 0;
uint8_t button_1_low = 0;
uint8_t button_1_high = 0;
uint8_t button_1_double = 0;








#ifdef INFO
 #define INFO_PRINT(x)  Serial.print (x)
 #define INFO_PRINTLN(x)  Serial.println (x)
#else
 #define INFO_PRINT(x)
 #define INFO_PRINTLN(x)
#endif
#ifdef DEBUG
 #define DEBUG_PRINT(x)  Serial.print (x)
 #define DEBUG_PRINTLN(x)  Serial.println (x)
#else
 #define DEBUG_PRINT(x)
 #define DEBUG_PRINTLN(x)
#endif
#ifdef TRACE
 #define TRACE_PRINT(x)  Serial.print (x)
 #define TRACE_PRINTLN(x)  Serial.println (x)
#else
 #define TRACE_PRINT(x)
 #define TRACE_PRINTLN(x)
#endif


/*
 * Compiler Checks
 */
#if defined (GPS_TIME) && defined (DS1303_TIME)
//#pragma message ( "Debug configuration - OK" )
#error "Only one time source is possible"
#endif
