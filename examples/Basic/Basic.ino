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

// URL to test
String myUrl = "http://%7EFoo:%7Ep@$$wOrd@kegcop.local:8000/%7EthIs/is/A/./Path/test.php?foo=bar#frag";

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

#if __has_include("./secret.h")
#include "secret.h" // Include for AP_NAME and PASSWD below
const char *ssid = AP_NAME;
const char *password = PASSWRD;
#else
const char *ssid = "my_ap";
const char *password = "passsword";
#endif

// Called last from the variadic template function
void printLine()
{
    Serial.println();
}

template <typename T, typename... Types>
void printLine(T first, Types... other)
{
    Serial.print(first);
    printLine(other...);
}

void setup()
{
    Serial.begin(BAUD);
    Serial.println();
    Serial.flush();
    Serial.println("Starting test run of LCBUrl.");

    WiFi.disconnect();
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.println("Establishing connection to WiFi..");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(100);
        Serial.print(F("."));
    }
    Serial.println();
    printLine("Connected, IP address: ", WiFi.localIP().toString());

#ifdef LCBURL_MDNS
    MDNS.begin(WiFi.getHostname());
#endif

    LCBUrl url;

    if (!url.setUrl(myUrl))
    {
        printLine("Failure in setUrl();\n");
    }
    else
    {
        printLine("Return from setUrl():");

        printLine("\nNormal member functions:");
        printLine("\tgetUrl(); = ", url.getUrl());
        printLine("\tgetIPUrl(); = ", url.getIPUrl());
        printLine("\tgetScheme(); = ", url.getScheme());
        printLine("\tgetUserInfo(); = ", url.getUserInfo());
        printLine("\tgetUserName(); = ", url.getUserName());
        printLine("\tgetPassword(); = ", url.getPassword());
        printLine("\tgetHost(); = ", url.getHost());
        printLine("\tgetPort(); = ", url.getPort());
        printLine("\tgetAuthority(); = ", url.getAuthority());
        printLine("\tgetPath(); = ", url.getPath());
        printLine("\tgetFileName(); = ", url.getFileName());
        printLine("\tgetQuery(); = ", url.getQuery());
        printLine("\tgetFragment(); = ", url.getFragment());

        printLine("\nHelper functions:");
        printLine("\tHostname isMDNS(): ", url.isMDNS(url.getHost().c_str()) ? "True" : "False");
        printLine("\tgetIP(); = ", url.getIP(url.getHost()).toString().c_str());
        printLine("\tHostname isValidHostName(): ", url.isValidHostName(url.getHost().c_str()) ? "True" : "False");
        printLine("\tHostname isValidIP(): ", url.isValidIP(url.getIP(url.getHost()).toString().c_str()) ? "True" : "False");
    }

    printLine("LCBUrl test run complete.");
}

void loop()
{
#ifdef ESP8266
    yield();
#endif
}
