/*
    LCBUrl.ino -  Test sketch to demonstrate handling URLs via LCBUrl - This
                  library will allow handling and manipulation of URLs
                  according to RFC3986.
*/

/*
    Copyright (C) 2019 Lee C. Bussy (@LBussy)

    This file is part of Lee Bussy's LCBUrl (LCB URL).

    LCBUrl is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation, either version 3 of the License, or (at your
    option) any later version.

    LCBUrl is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License along
    with LCBUrl. If not, see <https://www.gnu.org/licenses/>.
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
    yield();
}
