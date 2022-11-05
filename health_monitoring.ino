#define BLYNK_TEMPLATE_ID "TMPLu3o_ATPs"
#define BLYNK_DEVICE_NAME "health monitoring"
#define BLYNK_AUTH_TOKEN "lRNlTePEeyUjlMy0i1aPEVQlIaNTu_SF"

// library temp sensor
#include <OneWire.h>
#include <DallasTemperature.h>

#define USE_ARDUINO_INTERRUPTS true    
#include <PulseSensorPlayground.h> 

const int PulseWire = 36;       
int Threshold = 550;

#define ONE_WIRE_BUS 4

OneWire oneWire(ONE_WIRE_BUS); 
DallasTemperature sensors(&oneWire);

PulseSensorPlayground pulseSensor;

float temp = 0;
int myBPM = 0;


// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial


#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "YourNetworkName";
char pass[] = "YourPassword";

BlynkTimer timer;

// This function sends Arduino's up time every second to Virtual Pin (5).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.
void myTimerEvent()
{
  sensors.requestTemperatures(); 
  Serial.print(sensors.getTempCByIndex(0)); 

  myBPM = pulseSensor.getBeatsPerMinute();

  temp = sensors.getTempCByIndex(0);

  if (pulseSensor.sawStartOfBeat()) {            
     Blynk.virtualWrite(V1, myBPM);                 
  }
  
  Blynk.virtualWrite(V0, temp);
}

void setup()
{
  // Debug console
  Serial.begin(115200);
  sensors.begin();
  pulseSensor.analogInput(PulseWire);
  pulseSensor.setThreshold(Threshold);

  if (pulseSensor.begin()) {
    Serial.println("We created a pulseSensor Object !");
  }

  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);

  // Setup a function to be called every second
  timer.setInterval(1000L, myTimerEvent);
}

void loop()
{
  Blynk.run();
  timer.run(); // Initiates BlynkTimer
}
