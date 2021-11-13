# nodemcu_gps_oled
NodeMCU GPS OLED

Code for interfacing a NodeMCU (Possibly ESP32) with a GPS and Oled SSD1306 display.
Arduino code shows current location and other location details.

Created to use simple USB power for device.
Using Adafruit_SSD1306,softwareserial, adafruit_gfx, and TinyGPS++ libraries


CUSTOMGPS_WEB.INO
Added access point webserver with SSID GPS.  It serves one page which displays coordinates.  Also, added page to oled to show access point ip address to point your browser to.
Also increased font size on oled.
