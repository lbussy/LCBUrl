/*
    Basic.ino -  Test sketch to demonstrate handling URLs via LCBUrl - This
                  library will allow handling and manipulation of URLs
                  according to RFC3986.
*/

/*
    MIT License

    Copyright (c) 2019-2020 Lee C. Bussy

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

void setup() {
    Serial.begin(74880);
    delay(1000);
    Serial.println(F("Starting test run of LCBUrl:"));

    String myUrl = "http://%7EFoo:%7Ep@$$word@servername.local:80/%7Ethis/is/a/./path/test.php?foo=bar#frag";
    LCBUrl url;

    if (!url.setUrl(myUrl)) {
        Serial.println(F("Failure in setUrl();"));
    } else {
        Serial.print(F("Return from setUrl()"));
        Serial.print(F("\tgetUrl(); = "));
        Serial.println(url.getUrl().c_str());
        Serial.print(F("\tgetScheme(); = "));
        Serial.println(url.getScheme().c_str());
        Serial.print(F("\tgetUserInfo(); = "));
        Serial.println(url.getUserInfo().c_str());
        Serial.print(F("\tgetUserName(); = "));
        Serial.println(url.getUserName().c_str());
        Serial.print(F("\tgetPassword(); = "));
        Serial.println(url.getPassword().c_str());
        Serial.print(F("\tgetHost(); = "));
        Serial.println(url.getHost().c_str());
        Serial.print(F("\tgetPort(); = "));
        Serial.println(url.getPort());
        Serial.print(F("\tgetAuthority(); = "));
        Serial.println(url.getAuthority().c_str());
        Serial.print(F("\tgetPath(); = "));
        Serial.println(url.getPath().c_str());
        Serial.print(F("\tgetAfterPath(); = "));
        Serial.println(url.getAfterPath().c_str());
        Serial.print(F("\tgetQuery(); = "));
        Serial.println(url.getQuery().c_str());
        Serial.print(F("\tgetFragment(); = "));
        Serial.println(url.getFragment().c_str());
    }

    Serial.println(F("LCBUrl test run complete."));
}

void loop() {
#ifdef ESP8266
        yield();
#endif
}
