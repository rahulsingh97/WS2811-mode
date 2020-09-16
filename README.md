# :heart::green_heart::blue_heart: light!!

## led with modes.ino
this code makes use an external button to change the light modes here the `WS2811` is attached to `5v`,`gnd` & `pin no 2` on the esp32
and a button is attachesd to `pin 12` on the esp32
here the code cycled to different function as the button is triggered

## led with modes and esp.ino
here the code is same as `led with modes.ino` plus the code to make it compatable with alex is added 
dont forget change the `ssid` and `password` 
```c
#define WIFI_SSID "YOUR SSID"
#define WIFI_PASS "YOUR PASSWORD"
```
upload the code and say `ALEXA DISCOVER DEVICES` to start paring the led to alexa
after alexa discovers say `ALEXA NEXT' for next mode and `ALEXA PREVIOUS` for privious mode. This can be changed is the alexa app and also in the code
```c
// Add virtual devices
fauxmo.addDevice("next");
fauxmo.addDevice("previous");
```
replace `"next"` with what you want for next and replace `"previous"` with what you want for previous mode



