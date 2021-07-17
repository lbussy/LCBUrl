/*
    Basic.ino - Test sketch to demonstrate handling URLs via LCBUrl - This
                library will allow handling and manipulation of URLs
                according to RFC3986.
*/

/*
    MIT License

    Copyright (c) 2019-2021 Lee C. Bussy

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/

#include <LCBUrl.h>

#include <Arduino.h>

#ifdef ESP8266
#include <ESP8266WiFi.h>
#ifdef LCBURL_MDNS
#include <ESP8266mDNS.h>
#endif
#endif
#ifdef ESP32
#include <WiFi.h>
#ifdef LCBURL_MDNS
#include <ESPmDNS.h>
#endif
#endif

// LCBUrl does not require ArduinoLog.  It is used here to
// facilitate the examples.
#include <ArduinoLog.h>
#define LOG_LEVEL LOG_LEVEL_VERBOSE

#include "secret.h" // Include for AP_NAME and PASSWD below
const char* ssid = AP_NAME;
const char* password = PASSWRD;

// URL to test
String myUrl = "http://%7EFoo:%7Ep@$$wOrd@kegcop.local:8000/%7EthIs/is/A/./Path/test.php?foo=bar#frag";

void setup() {
    Serial.begin(BAUD);
    Serial.println();
    Serial.flush();
    Log.begin(LOG_LEVEL, &Serial, false);
    Log.notice(F("Starting test run of LCBUrl." CR CR));

    WiFi.disconnect();
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Log.notice(F("Establishing connection to WiFi.."));
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(100);
        Serial.print(F("."));
    }
    Serial.println();
    Log.notice(F("Connected, IP address: %s" CR), WiFi.localIP().toString().c_str());

#ifdef LCBURL_MDNS
    MDNS.begin(WiFi.getHostname());
#endif

    LCBUrl url;

    if (!url.setUrl(myUrl)) {
        Log.fatal(F("Failure in setUrl();" CR CR));
    } else {
        Log.notice(F("Return from setUrl():" CR));

        Log.notice(F(CR "Normal member functions:" CR));
        Log.notice(F("\tgetUrl(); = %s" CR), url.getUrl().c_str());
        Log.notice(F("\tgetIPUrl(); = %s" CR), url.getIPUrl().c_str());
        Log.notice(F("\tgetScheme(); = %s" CR), url.getScheme().c_str());
        Log.notice(F("\tgetUserInfo(); = %s" CR), url.getUserInfo().c_str());
        Log.notice(F("\tgetUserName(); = %s" CR), url.getUserName().c_str());
        Log.notice(F("\tgetPassword(); = %s" CR), url.getPassword().c_str());
        Log.notice(F("\tgetHost(); = %s" CR), url.getHost().c_str());
        Log.notice(F("\tgetPort(); = %l" CR), url.getPort());
        Log.notice(F("\tgetAuthority(); = %s" CR), url.getAuthority().c_str());
        Log.notice(F("\tgetPath(); = %s" CR), url.getPath().c_str());
        Log.notice(F("\tgetFileName(); = %s" CR), url.getFileName().c_str());
        Log.notice(F("\tgetQuery(); = %s" CR), url.getQuery().c_str());
        Log.notice(F("\tgetFragment(); = %s" CR), url.getFragment().c_str());

        Log.notice(F(CR "Helper functions:" CR));
        Log.notice(F("\tHostname isMDNS(): %T" CR), url.isMDNS(url.getHost().c_str()));
        Log.notice(F("\tgetIP(); = %s" CR), url.getIP(url.getHost()).toString().c_str());
        Log.notice(F("\tHostname isValidHostName(): %T" CR), url.isValidHostName(url.getHost().c_str()));
        Log.notice(F("\tHostname isValidIP(): %T" CR CR), url.isValidIP(url.getIP(url.getHost()).toString().c_str()));
    }

    Log.notice(F("LCBUrl test run complete." CR));
}

void loop() {
#ifdef ESP8266
        yield();
#endif
}
