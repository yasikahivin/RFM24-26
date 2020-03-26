//******************************************************************************** PQ Training Kit, ORION Space *********************************************************************************************
//******************************************************************************** Morse Using RFM26W           *********************************************************************************************
// This program is an example to test morse using RFM98W transceiver module
// This program is based on the example to encode/decode morse created by: Matthias Esterl.

#include <SPI.h>
#include <RH_RF95.h>

// Singleton instance of the radio driver
RH_RF95 rf95;

int DIO2=7;
#define UNIT_LENGTH  60 //For 50 WPM, Dit= 1200/50=24 ms
// 60 for 20 WPM

static const struct {const char letter, *code;} MorseMap[] =
{
  { 'A', ".-" },
  { 'B', "-..." },
  { 'C', "-.-." },
  { 'D', "-.." },
  { 'E', "." },
  { 'F', "..-." },
  { 'G', "--." },
  { 'H', "...." },
  { 'I', ".." },
  { 'J', ".---" },
  { 'K', ".-.-" },
  { 'L', ".-.." },
  { 'M', "--" },
  { 'N', "-." },
  { 'O', "---" },
  { 'P', ".--." },
  { 'Q', "--.-" },
  { 'R', ".-." },
  { 'S', "..." },
  { 'T', "-" },
  { 'U', "..-" },
  { 'V', "...-" },
  { 'W', ".--" },
  { 'X', "-..-" },
  { 'Y', "-.--" },
  { 'Z', "--.." },
  { ' ', "     " }, //Gap between word, seven units 
    
  { '1', ".----" },
  { '2', "..---" },
  { '3', "...--" },
  { '4', "....-" },
  { '5', "....." },
  { '6', "-...." },
  { '7', "--..." },
  { '8', "---.." },
  { '9', "----." },
  { '0', "-----" },
    
  { '.', "·–·–·–" },
  { ',', "--..--" },
  { '?', "..--.." },
  { '!', "-.-.--" },
  { ':', "---..." },
  { ';', "-.-.-." },
  { '(', "-.--." },
  { ')', "-.--.-" },
  { '"', ".-..-." },
  { '@', ".--.-." },
  { '&', ".-..." },
};
String MyID="9N1PO";
String morseWord;

void setup() 
{
  pinMode(DIO2,OUTPUT);
  Serial.begin(9600);

  while (!Serial) ; // Wait for serial port to be available
  if (!rf95.init())
    Serial.println("init failed");
//rf95.setFrequency(868);
rf95.setTxPower(13);
Morse_Setup(); 
}

void Morse_Setup()
{
  rf95.spiWrite(0x01, 0x00); //Sleep Mode
  delay(20);
  rf95.spiWrite(0x01, 0x20); //Modulation OOK Mode
  delay(20);
  rf95.spiWrite(0x31,0x00); //Set to Continous Transmit mode
  delay(20);
  rf95.spiWrite(0x01,0x22); //OOK FS Tx Mode
  delay(10);
  rf95.spiWrite(0x01,0x23); //OOK Tx Mode
  delay(20);
}

void loop()
{
/*
  delay(20);
  rf95.spiWrite(0x01,0x23);
  digitalWrite(4,HIGH);
  delay(500);
  rf95.spiWrite(0x01,0x23);
  digitalWrite(4,LOW);
  delay(500);
  */
  
  Morse_Setup();
  Send_Morse();
  rf95.spiWrite(0x01,0x21);
  delay(2000);
}

void Send_Morse()
{
  morseWord=encode("HELLO, THIS IS 9N1AA");
  for(int i=0; i<=morseWord.length(); i++)
  {
    switch( morseWord[i] )
    {
      case '.': //dit
        
        Serial.println("Dit");
        rf95.spiWrite(0x01,0x23);
        digitalWrite(DIO2,HIGH);
        rf95.spiWrite(0x40,0x40);
        //rf95.spiWrite(0x01,0x23);
        delay( UNIT_LENGTH );
        rf95.spiWrite(0x01,0x23);
        digitalWrite(DIO2,LOW);
         rf95.spiWrite(0x40,0x40);
        //rf95.spiWrite(0x01,0x23);
        delay( UNIT_LENGTH );
        break;

      case '-': //dah
        rf95.spiWrite(0x01,0x23);
        digitalWrite(DIO2,HIGH);
         rf95.spiWrite(0x40,0x40);
        //rf95.spiWrite(0x01,0x23);
        Serial.println("Dah");
        delay( UNIT_LENGTH*3 );
        rf95.spiWrite(0x01,0x23);
        digitalWrite(DIO2,LOW);
         rf95.spiWrite(0x40,0x40);
        //rf95.spiWrite(0x01,0x23);
        delay( UNIT_LENGTH );
        break;

      case ' ': //gap
        delay( UNIT_LENGTH );
    }
  }
  
}



String encode(const char *string)
{
  size_t i, j;
  String morseWord = "";
  
  for( i = 0; string[i]; ++i )
  {
    for( j = 0; j < sizeof MorseMap / sizeof *MorseMap; ++j )
    {
      if( toupper(string[i]) == MorseMap[j].letter )
      {
        morseWord += MorseMap[j].code;
        break;
      }
    }
    morseWord += " "; //Add tailing space to seperate the chars
  }

  return morseWord;  
}
