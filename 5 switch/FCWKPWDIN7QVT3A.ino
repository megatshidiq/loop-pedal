// Arduino Programable 5 Pedal Switcher

//this code is based on the code of the following link:
//https://www.instructables.com/arduino-programable-5-pedal-switcher/
//using arudio nano or uno
//using 5 relays
//using 5 leds
//using 5 switches



#include <EEPROM.h>
#include <Keypad.h>
const byte rows = 5;
const byte cols = 3;
char keys[rows][cols] = {
{'a','f','k'}, 
{'b','g','l'},
{'c','h','m'},
{'d','i','n'},
{'e','j','o'} 
};
byte rowPins[rows] = {2,3,4,5,6}; // switch pins
byte colPins[cols] = {7,8,9}; // mode pins
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, rows, cols);
int relayPin[5] = {10,11,12,13,14}; // relay pins
int ledPin[5] = {15,16,17,18,19}; // led pins
byte midiChannel = 0;

/******************************************************/
/**
 * @brief Initializes the Arduino board and sets up the initial configuration.
 * 
 * This function is called once when the Arduino board is powered on or reset.
 * It performs the following tasks:
 *  - Configures the relay and LED pins as output pins.
 *  - Turns on and off the relays and LEDs in a test phase.
 *  - Reads the preset values for the default mode.
 *  - Initializes the serial communication for MIDI communication.
 *  - Optionally erases the EEPROM memory.
 */
void setup()
{
  for(int i=0; i<5; i++) /* setup test phase */
  {
    pinMode(relayPin[i], OUTPUT);
    pinMode(ledPin[i], OUTPUT);
    digitalWrite(relayPin[i], HIGH);
    delay(300);
    digitalWrite(relayPin[i], LOW);
    digitalWrite(ledPin[i], HIGH);
    delay(300);
    digitalWrite(ledPin[i], LOW);
  }
readPreset(11, 1, 0); /* initiate  default mode */
Serial.begin(31250); /* for midi communication - pin 1 TX */
/*for (int i = 0; i < 10; i++) // erase eeprom (optional)
   // EEPROM.write(i, 0); */

   
}
/*********************************************************/
void midiProg(byte status, int data) 
 {
  Serial.write(status);
  Serial.write(data);
 }
 /*********************************************************/
void memory(int addr, int led)
{
  for(int i=0; i<5; i++)
  {
    EEPROM.write((addr) + i, digitalRead(relayPin[i]));
    digitalWrite(ledPin[i], LOW); // all leds reset
  }
  delay(100);
  digitalWrite(ledPin[led], HIGH);
  delay(100); 
  digitalWrite(ledPin[led], LOW);
  delay(100); 
  digitalWrite(ledPin[led], HIGH);
  delay(100);
  digitalWrite(ledPin[led], LOW);
  delay(100); 
  digitalWrite(ledPin[led], HIGH);
  delay(100);
  digitalWrite(ledPin[led], LOW);
  delay(100); 
  digitalWrite(ledPin[led], HIGH);
}

/*********************************************************/
void resetAllRelays()
{
  for(int i=0; i<5; i++)
  {
    digitalWrite(relayPin[i], LOW);
  }
}
/*********************************************************/
void resetAllLeds()
{
  for(int i=0; i<5; i++)
  {
    digitalWrite(ledPin[i], LOW);
  }
}
/*********************************************************/
void writeOut(int relay)
{
  resetAllLeds();
  digitalWrite(relayPin[relay], !digitalRead(relayPin[relay]));
  /*digitalWrite(ledPin[relay], !digitalRead(relayPin[relay]));
  /* thanks to  anton.efremoff.1 for this tip */
}
/*********************************************************/
void readPreset(int addr, int pcNum, int led)
{
  for(int i=0; i<5; i++)
  {
    digitalWrite(relayPin[i], EEPROM.read((addr)+i));
    digitalWrite(ledPin[i], LOW);
    digitalWrite(ledPin[led], HIGH);
  }
  midiProg(0xC0 | midiChannel , pcNum);  /* send midi change program 1 */
}
/*********************************************************/
void loop()
{
  
  char key = keypad.getKey();
  if(key)  // Check for a valid key.
  {
   switch (key)
      { 
    case 'a':  // a to x 
      writeOut(0); // relay
      break; 
    case 'b': 
      writeOut(1);
      break;
    case 'c': 
      writeOut(2);
      break;
    case 'd': 
      writeOut(3);
      break;
    case 'e': 
      writeOut(4);
      break;  
    /****************************** STORE PRESET MODE */       
    case 'f': 
      memory(11,0);  //addr, led
      break; 
    case 'g': 
      memory(21,1);
      break;
    case 'h': 
      memory(31,2);
      break;
    case 'i': 
      memory(41,3);
      break;
    case 'j': 
      memory(51,4);
      break;
    /****************************** READ PRESET MODE */      
    case 'k':  
      readPreset(11, 1, 0); // addr, pcNum, relay
      break; 
    case 'l':  
      readPreset(21, 2, 1);
      break;   
    case 'm': 
      readPreset(31, 3, 2);
      break;
    case 'n': 
      readPreset(41, 4, 3);
      break;
    case 'o': 
      readPreset(51, 5, 4);
      break;   
      }
   }
}
