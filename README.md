android-arduino-wifi
====================

Demo of a clear and robust way of creating a serial communication
link between an Arduino and an Android device using WiFi.

**Features:**

 * WiFi server using the [WiFly Shield](https://www.sparkfun.com/products/9954)
 and associated library on Arduino
 * WiFi client socket implemented using AsyncTask in Android
 * UI for connecting, disconnecting, reading, and writing
 * Exception handling and error management to prevent crashes
 * Parsing of complete newline-delimited messages from the stream
 * Timeout detection using a ping system
 * Lots of comments

**Requirements:**

  * Android device with WiFi enabled
  * Arduino with a WiFly shield
  * Android Studio and an Arduino IDE
  * A WiFi network to connect to and a not-blocked port

**Usage:**

  * Arduino - set your WiFi network name and password in `Credentials.h`.
  * Arduino - set your desired port number in `wifi_demo.ino`.
  * Flash the Arduino, then turn it on and open the serial monitor at 115200bps.
  * Wait for the WiFly to get an IP address.
  * Open the Android app, enter the address and port, and hit connect.
  * You should now be able to send messages and have them echoed back.
  * You can also type messages into the Aruino serial monitor to send them
  to the Android device (select newline endings).

**Important files:**

  * `WiFiActivity.java`
  * `activity_wi_fi.xml`
  * `wifi_demo.ino`

**Issues:**

 * WiFly takes a long time (~20s) to connect to a network on startup (watch the LEDs).
 * After disconnecting from a client, the WiFly takes about 6 seconds before
 it can connect to another client (again, watch the LEDs).
 * I had an issue with a version of the WiFly library where it would not
 initialize until it heard a `*READY*` message from the chip, which never came.
 If you get stuck at WiFly.begin(), look into this.
