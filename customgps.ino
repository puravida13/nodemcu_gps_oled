//Created by Alex Arocha 2021

#include <TinyGPS++.h>                                  // Tiny GPS Plus Library
#include <SoftwareSerial.h>                             // Software Serial Library so we can use other Pins for communication with the GPS module
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>                               //Adafruit_SSD1306 Library
#include <Adafruit_SSD1306.h>                           //Adafruit_SSD1306 Library

#define SCREEN_WIDTH 128                                // OLED display width, in pixels
#define SCREEN_HEIGHT 64                                // OLED display height, in pixels

#define OLED_RESET     -1                               // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C                             //< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16
static const unsigned char PROGMEM logo_bmp[] =         //Adafruit Logo...I like it
{ 0b00000000, 0b11000000,
  0b00000001, 0b11000000,
  0b00000001, 0b11000000,
  0b00000011, 0b11100000,
  0b11110011, 0b11100000,
  0b11111110, 0b11111000,
  0b01111110, 0b11111111,
  0b00110011, 0b10011111,
  0b00011111, 0b11111100,
  0b00001101, 0b01110000,
  0b00011011, 0b10100000,
  0b00111111, 0b11100000,
  0b00111111, 0b11110000,
  0b01111100, 0b11110000,
  0b01110000, 0b01110000,
  0b00000000, 0b00110000 };

static const int RXPin = 12, TXPin = 13;                // GPS module to pins 12 and 13
static const uint32_t GPSBaud = 9600;                   // GPS default Baud Rate is 9600

TinyGPSPlus gps;                                        // Create an Instance of the TinyGPS++ object called gps
SoftwareSerial ss(RXPin, TXPin);                        // The serial connection to the GPS device

void setup() {
  Serial.begin(9600);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(3000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();

  display.setTextSize(1);                                  // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);                     // Draw white text
  display.setCursor(0, 0);                                 // Start at top-left corner
  display.println("TinyGPS Version:");                     //Display text
  display.println(TinyGPSPlus::libraryVersion());          //Display tinygps library version
  display.display();                                      //Display buffer to screen
  delay(1500);                                            // Pause 1.5 seconds  
  ss.begin(GPSBaud);                                      // Set Software Serial Comm Speed to 9600  
}

void loop() {

  display.clearDisplay();                                      // Clear OLED display
  display.setCursor(0,0);                                      //Set cursor to upper left
  display.print("Latitude  : ");                               //Display text
  display.println(gps.location.lat(), 5);                      //Display TinyGPS location lat
  display.print("Longitude : ");                               //Display text
  display.println(gps.location.lng(), 4);                      //Display TinyGPS location lon
  display.print("Satellites: ");                               //Display text
  display.println(gps.satellites.value());                     //Display TinyGPS number of sats
  display.print("Elevation : ");                               //Display text
  display.print(gps.altitude.feet());                          //Display TinyGPS altitude
  display.println("ft");                                       //Display text
  display.display();                                           //Display buffer to screen
  delay(4000);                                                 //4 second pause
  display.clearDisplay();                                      //Clear display
  display.setCursor(0,0);                                      //Set cursor to upper left
  display.print("Time UTC  : ");                               //Display text
  display.print(gps.time.hour());                              // GPS time UTC 
  display.print(":");                                          //Display text
  display.print(gps.time.minute());                           // Display tinygps Minutes
  display.print(":");                                         //Display text
  display.println(gps.time.second());                         // display tinygps Seconds
  display.print("Heading   : ");                              //Display text
  display.println(gps.course.deg());                          //display tinygps course in degrees
  display.print("Speed     : ");                              //Display text
  display.println(gps.speed.mph());                           //display tinygps speed in mph
  display.display();                                           //Display buffer to screen
  delay(4000);                                                 //4 second pause
  
  smartDelay(500);                                            // Run Procedure smartDelay
  
  display.clearDisplay();                                     //clear display
 
  if (millis() > 5000 && gps.charsProcessed() < 10)           //if unable to get gps serial data run the code and display error
    display.println(F("No GPS data received: check wiring"));
    display.display();
    delay(2000);
  
}

static void smartDelay(unsigned long ms)                // This custom version of delay() ensures that the gps object is being "fed".
{
  unsigned long start = millis();
  do 
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}
