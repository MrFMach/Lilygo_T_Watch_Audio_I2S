#include <Arduino.h>
#include <WiFi.h>
#include "Audio.h"

// T-Watch config
#define LILYGO_WATCH_2020_V3
#include <LilyGoWatch.h>

TTGOClass *watch;
TFT_eSPI *tft;

const char *ssid = " ";
const char *password = " ";

//I2S T-Watch connection
#define I2S_DOUT 33
#define I2S_BCLK 26
#define I2S_LRC 25

// audio object create
Audio audio;

void startWatch()
{
  watch = TTGOClass::getWatch(); // Get TTGOClass instance
  watch->begin();                // Initialize the hardware
  watch->openBL();               // Turn on the backlight
  tft = watch->tft;              // Receive objects for easy writing
  tft->setTextFont(2);
  tft->setTextColor(TFT_LIGHTGREY, TFT_BLACK);
  tft->println("T-Watch started");
}

void startWifi()
{
  tft->setTextColor(TFT_ORANGE, TFT_BLACK);
  tft->print("Connecting Wi-Fi ");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    tft->print(".");
    delay(1000);
  }
  tft->println(" connected!");
  tft->print("Watch IP address: ");
  tft->println(WiFi.localIP());
}

void setAudio()
{
  audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
  audio.setVolume(12); // 0...21
}

void audioSpeech()
{
  tft->setTextColor(TFT_GREEN, TFT_BLACK);
  tft->println("Starting Google Text-to-Speech...");
  audio.connecttospeech("Hello. Text to Speech is working", "en");
  while (!audio.endWebfile)
  {
    audio.loop();
  }
  tft->println("End of Text-to-Speech");
  Serial.println("endWebfile = TRUE");
}

void audioSpiffs()
{
  tft->setTextColor(TFT_SKYBLUE, TFT_BLACK);
  tft->println("Starting SPIFFS audio file...");
  audio.connecttoFS(SPIFFS, "/siren.mp3");
  while (!audio.endFile)
  {
    audio.loop();
  }
  tft->println("End of SPIFFS audio file");
  Serial.println("endFile = TRUE");
}

void audioWeb()
{
  tft->setTextColor(TFT_PINK, TFT_BLACK);
  tft->println("Starting Web Audio...");
  //audio.connecttohost("https://www.chosic.com/wp-content/uploads/2021/04/dcpoke__birds-singing-03.mp3");
  audio.connecttohost("https://www.chosic.com/wp-content/uploads/2021/04/burghrecords__birds-singing-forest-scotland.mp3");
  while (!audio.endWebstream)
  {
    audio.loop();
  }
  tft->println("End Web Audio");
  Serial.println("endWebstream = TRUE");
}

void setup()
{
  Serial.begin(115200);
  startWatch();
  startWifi();
  setAudio();
  audioSpeech();
  SPIFFS.begin();
  audioSpiffs();
  audioWeb();
}

void loop()
{
}

void audio_info(const char *info)
{
  Serial.print("info        ");
  Serial.println(info);
}

void audio_id3data(const char *info)
{ //id3 metadata
  Serial.print("id3data     ");
  Serial.println(info);
}

void audio_eof_mp3(const char *info)
{ //end of file
  Serial.print("eof_mp3     ");
  Serial.println(info);
}

void audio_showstation(const char *info)
{
  Serial.print("station     ");
  Serial.println(info);
}

void audio_showstreaminfo(const char *info)
{
  Serial.print("streaminfo  ");
  Serial.println(info);
}

void audio_showstreamtitle(const char *info)
{
  Serial.print("streamtitle ");
  Serial.println(info);
}

void audio_bitrate(const char *info)
{
  Serial.print("bitrate     ");
  Serial.println(info);
}

void audio_commercial(const char *info)
{ //duração
  Serial.print("commercial  ");
  Serial.println(info);
}

void audio_icyurl(const char *info)
{ //homepage
  Serial.print("icyurl      ");
  Serial.println(info);
}

void audio_lasthost(const char *info)
{ //stream URL played
  Serial.print("lasthost    ");
  Serial.println(info);
}

void audio_eof_speech(const char *info)
{
  Serial.print("eof_speech  ");
  Serial.println(info);
}