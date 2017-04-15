
#include <NewPing.h>

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

#define TRIGGER_PIN  11   // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     12 // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 350 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);
int cm = 0;
int barlength =0 ;

void setup() {
  Serial.begin(9600); // Communications with display
  display.begin();
  display.setContrast(53);
  display.clearDisplay();   // clears the screen and buffer
  display.setTextSize(2);  
}

void loop() {
  delay(200);                     // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
  display.clearDisplay();
  display.drawRect(0, 39, 1 ,4, BLACK);
  display.drawRect(42, 39, 1 ,2, BLACK);
  display.drawRect(83, 39, 1 ,4, BLACK);  
  display.drawLine(0, 38, 84 ,38, BLACK);  
  display.setTextColor(BLACK);
  display.setTextSize(1);
  display.setCursor(0,0); display.println("Entfernung");
  display.setTextColor(BLACK);
  display.setTextSize(2);
  cm=int(sonar.ping_cm());
  barlength=round((cm * 84) / 350);
  //Hier noch if rein, wenn 0 dann outof range, wenn <25 dann under range
  display.setCursor(0,13); display.print(cm); display.println(" cm");// Send ping, get distance in cm and print result (0 = outside set distance range)
  
  display.drawRect(0, 35, barlength, 2, BLACK);
  display.drawRect(0, 37, barlength, 2, BLACK);
  display.display();
  
}
