/*
    LCBUrl.h -  Library for handling URLs - This library will allow
                handling and manipulation of URLs according to RFC3986.
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

// Pattern:  https://www.arduino.cc/en/uploads/Hacking/Test.zip  (DEBUG: TODO:)

// Ensure this library description is only included once
#ifndef _LCBURL_H
#define _LCBURL_H

#include <stdlib.h>
#include <Arduino.h>

// Library interface description
class LCBUrl {
    // User-accessible "public" interface
    public:
        LCBUrl() {};
        ~LCBUrl() {};
        bool setUrl(String);
        String getUrl();
        String getScheme();
        String getUserInfo();
        String getUserName();
        String getPassword();
        String getHost();
        word getPort();
        String getAuthority();
        String getPath();
        String getAfterPath();
        String getQuery();
        String getFragment();

    // Library-accessible "private" interface
    private:
        bool parseUrl(String);
        String getCleanTriplets();
        String getStripScheme();
        String getRawAuthority();
        String getAfterAuth();
        String getRemoveDotSegments();
        String url;
        String cleantriplets;
        String scheme;
        String stripscheme;
        String rawauthority;
        String afterauth;
        String userinfo;
        String username;
        String password;
        String host;
        word port;
        String authority;
        String path;
        String removedotsegments;
        String afterpath;
        String query;
        String fragment;
};

#endif // _LCBURL_H
