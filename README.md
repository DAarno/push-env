# Push Env for Arduino MKRENV

Sketch to push environment data to server periodically.
    
This sketch will push environment data to a HTTP server periodically
(every 10 min by default), and then (depending on flag), either sleep
or enter a low power mode (to make it possible to use with battery
power).
    
The three functions in network-info.h must be implemented to provide
WiFi name/password and IP address for the server to push to.

## Why this approach?

The idea here is to be able to run the Arduino on battery power for an
extended period of time, periodically sampling data about the
environment and uploadign it to a server.

When entering deep sleep, the Arduino consumes very little power and
can thus run on battery power for an extended period of time. In
contrast, if the CPU and WiFi module is constantly active - the
battery will drain quickly (< 1 day).

If power consumtion is not a concern, an alternative approach could be
to simply setup the Arduino as a web server that the data can be
polled from (or simply skip the deep sleep of this sketch).

## Deep Sleep

When entering deep sleep mode, the Arduino basically becomes
unresponsive. This means even after a power-cycle, you will not be
able to reprogram the board. To resolve this, power on the board and
"double tap" on the "reset" button. If working on the code, it is best
to set `shouldDeepSleep` to `false`.
