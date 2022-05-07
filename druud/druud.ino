#include <NTPClient.h>
#include <Timezone.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Adafruit_NeoPixel.h>

#include "config.h"

WiFiUDP ntpUDP;

// By default 'pool.ntp.org' is used with 60 seconds update interval and no offset
NTPClient timeClient(ntpUDP);

// Timezone configuration
TimeChangeRule CEST = {"CEST", Last, Sun, Mar, 2, 120};     // Central European Summer Time
TimeChangeRule CET = {"CET ", Last, Sun, Oct, 3, 60};       // Central European Standard Time
Timezone CE(CEST, CET);
TimeChangeRule *tcr;        // pointer to the time change rule, use to get TZ abbrev

// NeoPixel configuration
#define LED_PIN   D2
#define LED_COUNT 1
Adafruit_NeoPixel led(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

// state: 0=sleep (red), 1=awakening (yellow), 2=awake (green)
uint previousState;
uint state;

uint roge;
uint gegr;
uint grro;

void setup() {
  Serial.begin(115200);
  
  WiFi.begin(ssid, password);
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print (".");
  }
  Serial.println (" Connected :-)");

  // tijden omzetten van String naar seconden
  roge  = s2t(ontwaken);  // van rood -> geel
  gegr  = s2t(druud);  // van geel -> groen
  grro  = s2t(slapen);  // van groen -> rood
  showConfig();

  led.begin();

  timeClient.begin();
  timeClient.update();
  time_t now = CE.toLocal(timeClient.getEpochTime(), &tcr) % 86400;
  state = getDesiredState(now);
  previousState = state;
  setState(state);

  String nowAsStr = t2s(now);
  Serial.print("Init at ");
  Serial.print(nowAsStr);
  Serial.print(" with state: ");
  Serial.println(state);
}

void loop() {
  uint timer;
  timeClient.update();
  time_t now = CE.toLocal(timeClient.getEpochTime(), &tcr) % 86400;
  state = getDesiredState(now);

  if (previousState != state) {
    String nowAsStr = t2s(now);
    Serial.print("Start at ");
    Serial.print(nowAsStr);
    Serial.print(" transition from state: ");
    Serial.print(previousState);
    Serial.print(" to state: ");
    Serial.println(state);

    transform2State(state);
    previousState = state;
  }

  uint d = getDelay2NextStateChange(now);
  if (d < maxDelay) {
    timer = d * 1000;
    Serial.println("*");
  } else {
    timer = maxDelay * 1000;
    Serial.print(".");
  }
  delay(timer);
}

uint getDesiredState(time_t t) {
  if (t < roge) return 0;
  else if (t < gegr) return 1;
  else if (t < grro) return 2;
  else return 0;
}

uint getDelay2NextStateChange(time_t t) {
  if (t < roge) return roge - t;
  else if (t < gegr) return gegr - t;
  else if (t < grro) return grro - t;
  else return 86400 - t + roge;
}

void setState(uint state) {
  switch (state) {
    case 0: led.setPixelColor(0, led.Color(helderheid, 0, 0)); break;
    case 1: led.setPixelColor(0, led.Color(helderheid / 2, helderheid / 2, 0)); break;
    case 2: led.setPixelColor(0, led.Color(0, helderheid, 0)); break;
  }
  led.show();
}

void transform2State(uint state) {
  switch (state) {
    case 0: led.setPixelColor(0, led.Color(helderheid, 0, 0)); led.show(); break;
    case 1: transform(helderheid, 0); break;
    case 2: transform(helderheid / 2, helderheid / 2); break;
  }
}

void transform(int ro, int gr) {
  float timer = 2000 * duur / helderheid;
  float steps = helderheid / 2;
  for (int i = 0; i < steps; ++i) {
    ro--; gr++;
    led.setPixelColor(0, led.Color(ro, gr, 0));
    led.show();
    delay(timer);
  }
}

String t2s(uint rawTime) {
  uint hours = (rawTime % 86400L) / 3600;
  String hoursStr = hours < 10 ? "0" + String(hours) : String(hours);

  uint minutes = (rawTime % 3600) / 60;
  String minuteStr = minutes < 10 ? "0" + String(minutes) : String(minutes);

  uint seconds = rawTime % 60;
  String secondStr = seconds < 10 ? "0" + String(seconds) : String(seconds);

  return hoursStr + ":" + minuteStr + ":" + secondStr;
}

uint s2t(String timeAsStr) {
  uint found = timeAsStr.indexOf(":");
  uint hour = timeAsStr.substring(0, found).toInt();
  uint minute = timeAsStr.substring(found + 1).toInt();
  return hour * 3600 + minute * 60;
}

void showConfig() {
  Serial.println("Configuratie");
  Serial.print("Ontwaken: " + ontwaken + " ");
  Serial.println(roge);
  Serial.print("Druud: " + druud + " ");
  Serial.println(gegr);
  Serial.print("Slapen: " + slapen + " ");
  Serial.println(grro);
  Serial.print("Helderheid: ");
  Serial.println(helderheid);
  Serial.print("Duur: ");
  Serial.println(duur);
  Serial.print("MaxDelay: ");
  Serial.println(maxDelay);
}
