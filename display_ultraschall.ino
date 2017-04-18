
#include <NewPing.h> // For ultrasonic

#include <SPI.h>
#include <Adafruit_GFX.h> // Display
#include <Adafruit_PCD8544.h>  // Display

// Für den Drehencoder
#include <ClickEncoder.h>
#include <TimerOne.h>
ClickEncoder *encoder;
int contrastOpt = 40; // value for optimum contrast for display when powered via direct +5V, may differ from display and power source
int16_t contrastLast, contrastValue = contrastOpt;
unsigned long timeSinceChange;
void timerIsr() {
  encoder->service();
}
// Drehencoder

// Pins for ultrasonic device
#define TRIGGER_PIN  9   // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     10 // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 350 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3); // setup instance of display with the pins to attach (CLK, DIN, DC, CE, RST)
int cm = 0;
int barlength = 0;
float ledBrightness = 0;

void setup() {
 // Serial.begin(9600); // Used when communicating via serial/terminal
  display.begin();
  display.setContrast(contrastValue);
  display.clearDisplay();   // clears the screen and buffer
  display.setTextSize(2);  
  
//Drehencoder
  encoder = new ClickEncoder(A1, A0, A2);  // setup instance of decoder with the pins to attach (DT, CLK, SW)
  Timer1.initialize(1000);
  Timer1.attachInterrupt(timerIsr); 
  contrastLast = -1;
//Drehencoder
}

void loop() {
  delay(1000);                     // Wait between pings. 29ms should be the shortest delay between pings.
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
  
  if ((millis() - timeSinceChange) < 700) {    // check if contrast value was changed and show value for 700ms on screen
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
    ledBrightness = 0;
  }
  else {
    display.setCursor(0,12); display.print(cm); display.println(" cm");// Send ping, get distance in cm and print result (0 = outside set distance range)
    ledBrightness= round(((cm - 25) * 255.0) / 325.0);
    display.setTextSize(1);
    barlength=round((cm * 84.0) / 350.0);
  }
  display.drawRect(0, 32, barlength, 2, BLACK);
  display.drawRect(0, 34, barlength, 2, BLACK);
  display.display();
  analogWrite(11, round(ledBrightness));
  
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
