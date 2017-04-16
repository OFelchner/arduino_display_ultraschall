
#include <NewPing.h>

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

//Für den Drehencoder
#include <ClickEncoder.h>
#include <TimerOne.h>

ClickEncoder *encoder;
int contrastOpt = 40;
int16_t contrastLast, contrastValue = contrastOpt;
unsigned long timeSinceChange;

void timerIsr() {
  encoder->service();
}
// Drehencoder


#define TRIGGER_PIN  11   // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     12 // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 350 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);
int cm = 0;
int barlength = 0 ;

void setup() {
  Serial.begin(9600); // Communications with display
  display.begin();
  display.setContrast(contrastValue);
  display.clearDisplay();   // clears the screen and buffer
  display.setTextSize(2);  
  
//Drehencoder
  encoder = new ClickEncoder(A1, A0, A2);
  Timer1.initialize(1000);
  Timer1.attachInterrupt(timerIsr); 
  contrastLast = -1;
//Drehencoder
}

void loop() {
  delay(200);                     // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
  display.clearDisplay();
  display.drawRect(0, 36, 1 ,4, BLACK);
  display.drawRect(42, 36, 1 ,2, BLACK);
  display.drawRect(83, 36, 1 ,4, BLACK);  
  display.drawLine(0, 35, 84 ,35, BLACK);
  display.setTextSize(1);  
  display.setTextColor(BLACK);
  display.setCursor(1,41); display.print("25");
  display.setCursor(65,41); display.print("350");
  display.setCursor(0,0); display.println("Entfernung");
  
  if ((millis() - timeSinceChange) < 700) {
    display.setCursor(37,41); display.println(contrastValue - contrastOpt); 
  }
  else {
    display.setCursor(65,41); display.println("  "); 
  }

  display.setTextSize(2);
  cm=int(sonar.ping_cm());

  if (cm <= 25) {
    display.setCursor(0,12); display.println("Range!");
    barlength = 0;
  }
  else {
    display.setCursor(0,12); display.print(cm); display.println(" cm");// Send ping, get distance in cm and print result (0 = outside set distance range)
    barlength=round((cm * 84) / 350);
  }
  
  display.drawRect(0, 32, barlength, 2, BLACK);
  display.drawRect(0, 34, barlength, 2, BLACK);
  display.display();
  
//Drehencoder
  contrastValue += encoder->getValue();
  
  if (contrastValue != contrastLast) {
    timeSinceChange = millis();
    contrastLast = contrastValue;
    if (contrastValue >= contrastOpt - 15) {
      if (contrastValue <= contrastOpt + 15) {
        display.setContrast(contrastValue);
      }
      else {
      contrastValue = contrastOpt + 15;
      }
    }
    else {
    contrastValue = contrastOpt - 15;
    }    
  }
//Drehencoder
  
}
