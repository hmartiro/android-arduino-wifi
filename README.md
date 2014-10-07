android-arduino-wifi
====================

Demo of a clear and simple way to interface Android and Arduino over a
WiFi connection for serial communication.

**Features:**

 * WiFi server using the [WiFly Shield](https://www.sparkfun.com/products/9954)
 and associated library on Arduino
 * WiFi client socket implemented using AsyncTask in Android
 * UI for connecting, disconnecting, reading, and writing
 * Exception handling to prevent app crashes
 * Parsing of complete newline-delimited messages from the stream
 * Lots of comments

**Requirements:**

  * Android device with WiFi enabled.
  * Arduino with a WiFly shield.
  * Android Studio and an Arduino IDE.

**Usage:**

  * Arduino - set your WiFi network name and password in `Credentials.h`.
  * Arduino - set your desired port number in `wifi_demo.ino`.
  * Flash the Arduino, then turn it on and open the serial monitor at 115200bps.
  * Wait for the WiFly to get an IP address.
  * Open the Android app, enter the address and port, and hit connect.
  * You should now be able to send messages and have them echoed back.

**Important files:**

  * `WiFiActivity.java`
  * `activity_wi_fi.xml`
  * `wifi_demo.ino`

**Issues:**

 * WiFly takes a long time (~20s) to connect to a network on startup.
 * After disconnecting, must wait about 6s before trying to reconnect (you
will be able to tell based on the WiFly's LEDs)
