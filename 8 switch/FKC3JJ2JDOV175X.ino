//thid code url: https://www.instructables.com/Arduino-Programable-8-Pedal-Switcher/
//using arudio mega


#include <EEPROM.h>
#include <Keypad.h>
int numberOfPedal = 8; /*adapt this number to your needs = number of loop pedals */
const byte rows = 8; /*number of switches variable */
const byte cols = 3; /*the three mode of selector*/
char keys[rows][cols] = {
{'a','i','q'},
{'b','j','r'},
{'c','k','s'},
{'d','l','t'},
{'e','m','u'},
{'f','n','v'},
{'g','o','w'},
{'h','p','x'} /* {'y','z','0'}, {'1','2','3'}, {'*','%','!'} add more characters if needed */
};

byte rowPins[rows] = {22, 23, 24,25, 26,27, 28, 29}; /*switch pins 1 to 8 */
byte colPins[cols] = {30, 31, 32}; /*selector pins a to h/ i to p/ q to x */
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, rows, cols);
int relayPin[8] = {33,34,35,36,37,38,39,40};
int ledPin[8] = {41,42,43,44,45,46,47,48};
byte midiChannel = 0;
int i;
int readOut;

/******************************************************/

void setup()
{
  for(i=0; i<numberOfPedal; i++)
   {
    pinMode(relayPin[i], OUTPUT);
    pinMode(ledPin[i], OUTPUT);
    digitalWrite(relayPin[i],HIGH); //pullup all relay outputs in case off low level relayboard
   }
Serial.begin(31250); /* for midi communication - pin 1 TX */
delay(200);
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
  for(i=0; i<numberOfPedal; i++)
    {
      EEPROM.write((addr) + i, digitalRead(relayPin[i]));
      digitalWrite(ledPin[i], LOW);
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

void writeOut(int relay)
{
  digitalWrite(relayPin[relay], !digitalRead(relayPin[relay]));
  digitalWrite(ledPin[relay], !digitalRead(relayPin[relay]));
}

/*********************************************************/

void readPreset(int addr, int pcNum, int led)
{
  for(i=0; i<numberOfPedal; i++)
  {
    digitalWrite(relayPin[i], EEPROM.read((addr)+i));
    digitalWrite(ledPin[i], LOW);
    digitalWrite(ledPin[led], HIGH);
   }
  midiProg(0xC0, pcNum +1); /* send midi change program 1 */
}

/******************************************************/

void loop()
{
char key = keypad.getKey();
if(key) // Check for a valid key.
  {
   switch (key)
    {
     case 'a': /* 'a' to 'h' for 8 pedals- adapt it to the number you needs */
      writeOut(0);
      break;
     case 'b':
      writeOut(1); /* (relay number,led number) */
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
     case 'f':
      writeOut(5);
      break;
     case 'g':
      writeOut(6);
      break;
     case 'h':
      writeOut(7);
      break;
/****************************** STORE PRESET MODE */
     case 'i': /* same remark as previous */
      memory(11,0);
      break;
     case 'j':
      memory(21,1); /* (EEPROM address, led) */
      break;
     case 'k':
      memory(31,2);
      break;
     case 'l':
      memory(41,3);
      break;
     case 'm':
      memory(51,4);
      break;
     case 'n':
      memory(61,5);
      break;
     case 'o':
      memory(71,6);
      break;
     case 'p':
      memory(81,7);
      break;
/****************************** READ PRESET MODE (EEProm address,PcNum,led number)*/
     case 'q':
      readPreset(11, 1, 0); 
      break;
     case 'r':
      readPreset(21, 2, 1);
      break;
     case 's':
      readPreset(31, 3, 2);
      break;
     case 't':
      readPreset(41, 4, 3);
      break;
     case 'u':
      readPreset(51, 5, 4);
      break;
     case 'v':
      readPreset(61, 6, 5);
      break;
     case 'w':
      readPreset(71, 7, 6);
      break;
     case 'x':
      readPreset(81, 8, 7);
      break;
      }
    }
}
