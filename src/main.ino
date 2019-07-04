/* This program is free software. It comes without any warranty, to
     * the extent permitted by applicable law. You can redistribute it
     * and/or modify it under the terms of the Do What The Fuck You Want
     * To Public License, Version 2, as published by Sam Hocevar. See
     * http://www.wtfpl.net/ for more details. */

#include "EspMQTTClient.h"
#include <IRremoteESP8266.h>
#include <IRsend.h>

EspMQTTClient client(
    "Home-WiFi",       // wifi SSID
    "meem47301",       // wifi password
    "111.222.333.444", // mqtt broker ip
    "user1",           // mqtt broker username
    "p$$wd",           // mqtt broker password
    "esp32aircond"     // mqtt client name
    1883               // mqtt broker port
);

const uint16_t kIrLed = 4; // GPIO4 (D2).
const char* topic_sub = "aircond/command";
const char* topic_pub = "aircond/feedback";

#define KHZ38 38

IRsend irsend(kIrLed);

// raw data found from IRremoteESP8266/Examples/IRrecvDemo.ino
uint16_t raw_cmd_panasonic_ac_len = 271;
uint16_t raw_cmd_panasonic_ac[271] = {3492, 3482,  862, 2614,  860, 2614,  862, 2616,  860, 874,  862, 876,  860, 2614,  860, 2614,  860, 878,  860, 2612,  862, 2614,  860, 2614,  862, 874,  862, 876,  862, 2614,  860, 2612,  862, 876,  860, 876,  860, 2614,  860, 876,  862, 876,  860, 876,  862, 874,  862, 878,  860, 876,  862, 876,  862, 2614,  860, 876,  860, 878,  860, 876,  860, 876,  860, 876,  860, 876,  3468, 3480,  862, 2614,  860, 2612,  862, 2614,  860, 876,  862, 876,  860, 2614,  862, 2612,  862, 876,  862, 2612,  862, 2614,  862, 2618,  856, 876,  860, 876,  860, 2614,  862, 2612,  862, 876,  860, 876,  860, 2612,  862, 876,  862, 878,  858, 876,  862, 876,  862, 876,  860, 876,  860, 878,  860, 2612,  862, 876,  860, 876,  860, 876,  860, 876,  862, 876,  860, 876,  3466, 3482,  862, 14178,  3470, 3484,  860, 876,  860, 876,  862, 878,  860, 878,  860, 2614,  860, 876,  860, 876,  862, 2614,  860, 876,  860, 876,  862, 876,  860, 878,  860, 2614,  860, 876,  860, 876,  862, 2612,  862, 876,  862, 2614,  862, 2614,  860, 876,  860, 2614,  860, 2614,  862, 876,  862, 876,  860, 876,  860, 2614,  862, 2614,  860, 876,  862, 2614,  860, 2612,  862, 876,  860, 876,  3468, 3482,  862, 876,  860, 874,  862, 876,  862, 876,  862, 2612,  862, 876,  860, 876,  860, 2614,  860, 876,  860, 876,  862, 876,  862, 876,  860, 2614,  862, 876,  862, 876,  862, 2612,  862, 876,  862, 2614,  860, 2614,  862, 876,  860, 2614,  860, 2614,  862, 876,  860, 878,  860, 876,  860, 2612,  862, 2614,  862, 876,  860, 2612,  862, 2616,  860, 874,  862, 876,  3468, 3482,  860};

void setup()
{
    irsend.begin();
    Serial.begin(115200);
}

void onConnectionEstablished()
{
    client.publish(topic_pub, "connected");
    
    client.subscribe(topic_sub, [](const String & payload) {
        irsend.sendRaw(raw_cmd_panasonic_ac,
                       raw_cmd_panasonic_ac_len,
                       KHZ38);
        client.publish(topic_pub, payload);
        
        Serial.println(payload);
    });
}

void loop()
{
    client.loop();
}
