/*
 * Displays text sent over the serial port (e.g. from the Serial Monitor) on
 * an attached LCD.
 */
#include <Wire.h> 
//über Sketch->Bibliothek einbinden->Bibliotheken verwalten... nach "lcd i2c" suchen und die Bibliothek von "Frank de Brabander" installieren
#include <LiquidCrystal_I2C.h>

//Adresse des Displays kann anders sein
LiquidCrystal_I2C lcd(0x27, 20,4);  // set the LCD address to 0x38

//Pin belegung kann anders sein
const int buttonMan = 2;    // the number of the pushbutton pin
const int buttonWoman = 3;    // the number of the pushbutton pin
const int ledMan =  4;      // the number of the LED pin
const int ledWoman =  5;      // the number of the LED pin

int manState = LOW;         // variable for reading the pushbutton status
int ledManState = LOW;         // the current state of the output pin
int lastManState = LOW;     // previous state of the button
int womanState = LOW;         // variable for reading the pushbutton status
int ledWomanState = LOW;         // the current state of the output pin
int lastWomanState = LOW;     // previous state of the button

unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

signed short minutes;
char timeline[16];
unsigned long time, seconds;

void setup()
{
  // initialize the pushbutton pin as an input:
  pinMode(buttonMan, INPUT);
  pinMode(buttonWoman, INPUT);
  // initialize the LED pin as an output:
  pinMode(ledMan, OUTPUT);
  pinMode(ledWoman, OUTPUT);
  //LCD Settings at beginning
  lcd.init();
  //lcd.begin (16,2);
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Assalamu Aleykum");
  lcd.setCursor(0, 1);        // go to the next line
  lcd.print("Gebetsraum frei!");

  Serial.begin(9600);
}

void loop()
{
  // read the state of the pushbutton value:
  int manRead = digitalRead(buttonMan);
  int womanRead = digitalRead(buttonWoman);

  if ( (manRead != lastManState) || (womanRead != lastWomanState) ) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if ((manRead != manState) || (womanRead != womanState)) {
      manState = manRead;
      womanState = womanRead;
      // only toggle the LED if the new button state is HIGH
      if ((manState == HIGH) || (womanState== HIGH)) {
        if (manState)  {
          ledManState = !ledManState;
          
          if (ledManState) {
            Serial.println("LED Man AN");
            lcd.clear();
            lcd.setCursor (0, 0);        // go to the next line
            lcd.print("Besetzt Herren!");
            minutes = 0;
            seconds = 0;
            time = millis();
            sprintf(timeline,"%0.2d:%0.2d", minutes, seconds);
            lcd.setCursor (0, 1);        // go to the next line
            lcd.print(timeline);
          }
          else {
            //komplett frei
            Serial.println("LED man Aus");
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Assalamu Aleykum");
            lcd.setCursor(0, 1);        // go to the next line
            lcd.print("Gebetsraum frei!");
          }

          if (ledWomanState) {
            ledWomanState = LOW;
            //frau weg man bestzt
            Serial.println("LED woman Aus");
          }
        }
        if (womanState) {
          ledWomanState = !ledWomanState;
         
          if (ledWomanState) {
            Serial.println("LED woman AN");
            lcd.clear();
            lcd.setCursor (0, 0);        // go to the next line
            lcd.print("Besetzt Damen!");
            minutes = 0;
            seconds = 0;
            time = millis();
            sprintf(timeline,"%0.2d:%0.2d", minutes, seconds);
            lcd.setCursor (0, 1);        // go to the next line
            lcd.print(timeline);
          }
          else {
            //komplett frei
            Serial.println("LED woman Aus");
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Assalamu Aleykum");
            lcd.setCursor(0, 1);        // go to the next line
            lcd.print("Gebetsraum frei!");
          }
          
          if (ledManState) {
            ledManState = LOW;
            //man weg frau besetzt
            Serial.println("LED man Aus");
          }
        }
      }
    }
  }

  // set the LED:
  digitalWrite(ledMan, ledManState);
  digitalWrite(ledWoman, ledWomanState);
  // save the reading. Next time through the loop, it'll be the lastButtonState:
  lastManState = manRead;
  lastWomanState = womanRead;

  if (ledWomanState || ledManState) {
    lcd.setCursor(0, 1);
    sprintf(timeline,"%0.2d:%0.2d", minutes, seconds);
    lcd.print(timeline);
    
    seconds = (millis()-time)/1000;
    
    if (seconds >= 60)
    {
      seconds = 0;
      time = millis();
      minutes ++;
    }
  }
}
