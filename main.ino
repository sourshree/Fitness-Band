#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
BlynkTimer timer;
#define BLYNK_AUTH_TOKEN "" //Enter your blynk auth token
int steps = 0;
int lasthr=0;
int delta=0;
int lastval=0;
int newval=0;
char auth[] = "2a0f8aa7bUCBoQgBcCXAM9FceRkjsYCm";
char ssid[] = "Shreetama's iPhone";//Enter your WIFI name
char pass[] = "shreetamaaaa";//Enter your WIFI password
bool notif1 = false;
bool notif2 = false;
int PulseSensorPurplePin = 0;
int Signal;           
int Threshold = 580;
int LED=LED_BUILTIN;
bool chec=false;
bool ren=false;
void heart(){
  if (chec==true){
  Signal = analogRead(PulseSensorPurplePin);  

  Blynk.virtualWrite(V7,Signal);
   if(Signal > Threshold){                         
     digitalWrite(LED,HIGH);
   } else {
     digitalWrite(LED,LOW);  
   }             
   }
}
BLYNK_WRITE(V0){
  int buttonState=param.asInt();
  digitalWrite(16,buttonState);
  
  if (buttonState==1){
    chec=true;
  timer.setInterval(1L,heart);
  
  }
  else {
    chec=false;
  }
  
}
void notif(){
  if (steps-lasthr<40){
    Blynk.logEvent("low_steps");
  }
  lasthr= steps;
}

void pol(){
  if (steps>=250 && notif1==false){
    notif1 = true;
    Blynk.logEvent("steps_250");
  }
  else if (steps>=500 && notif2==false){
    notif2=true;
    Blynk.logEvent("steps_500");
  }
}

void myTimerEvent()
{
  if (chec==false){
  newval=analogRead(A0);
  delta = newval-lastval;
  lastval=newval;
  if (delta>-20 & delta<20){
    steps=steps;
    }
  else {
    steps++;
   }
  Blynk.virtualWrite(V6,steps*0.04);
  Blynk.virtualWrite(V5, steps);
  }
}
void setup() {
  pinMode(16,OUTPUT);
  pinMode(A0,INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  lastval= analogRead(A0);
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  timer.setInterval(800L, myTimerEvent);
  timer.setInterval(3600000L, notif);
  timer.setInterval(5000L,pol);
}

void loop() {
  Blynk.run();
  timer.run();
}
