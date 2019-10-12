/*
    LCBUrl.cpp -  Library for handling URLs - This library will allow
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

// Include this library's description file

#include "LCBUrl.h"

// Constructor/Destructor ////////////////////////////////////////////////
// Handle the creation, setup, and destruction of instances

// Public Methods //////////////////////////////////////////////////////////////
// Functions available in sketches, this library, and other libraries

bool LCBUrl::setUrl(String newUrl) {
    if (getUrl() == "") {return false;} else {return true;}
}

String LCBUrl::getUrl() {
    if (!url) {
        url = "";
        if ((getScheme() == "") || (getHost() == "")) {
            return url;
        } else {
            url.concat(getScheme());
            url.concat(F("://"));
            url.concat(getAuthority());
            url.concat(F("/"));
            url.concat(getPath());
            if (getQuery() != "") {
                url.concat(F("?"));
                url.concat(getPath());
            }
            if (getFragment() != "") {
                url.concat(F("#"));
                url.concat(getFragment());
            }
        }
    }
    return url;
}

String LCBUrl::getScheme() { // Only finds http and https as scheme
    if (!scheme) {
        scheme = "";
        int loc = getCleanTriplets().indexOf(F(":"), 0);
        if (loc) {
            String s = getCleanTriplets().substring(0, loc - 1);
            s.toLowerCase();
            if ((s == F("http")) || (s == F("https"))) {
                scheme = s;
            }
        }
    }
    return scheme;
}

String LCBUrl::getRawAuthority() {
    // Authority is similar to "lbussy@raspberrypi.local:80"
    if (!rawauthority) {
        rawauthority = "";
        String tempUrl = getStripScheme();
        if ((tempUrl) && (tempUrl.length() > 0)) {
            int loc = tempUrl.indexOf(F("/"), 0);
            if (loc) {
                rawauthority = tempUrl.substring(0, loc - 1);
            }
        }
    }
    return rawauthority;
}

String LCBUrl::getUserInfo() {
    // UserInfo will be anything to the left of @ in authority
    if (!userinfo) {
        String tempUrl = getRawAuthority();
        if (tempUrl) {
            int loc = tempUrl.indexOf(F("@"), 0);
            if (loc) {
                return tempUrl.substring(0, loc - 1);
            }
        } else {
            return {};
        }
    }
}

String LCBUrl::getUserName() {
    // User Name will be anything to the left of : in userinfo
    if (!username) {
        String tempUrl = getUserInfo();
        if (tempUrl) {
            int loc = tempUrl.indexOf(F(":"), 0);
            if (loc) {
                return tempUrl.substring(0, loc - 1);
            }
        } else {
            return tempUrl;
        }
    }
}

String LCBUrl::getPassword() {
    // Password will be anything to the right of : in userinfo
    if (!password) {
        String tempUrl = getUserInfo();
        if (tempUrl) {
            int loc = tempUrl.indexOf(F(":"), 0);
            if (loc) {
                return tempUrl.substring(loc + 1);
            }
        } else {
            return {};
        }
    }
}

String LCBUrl::getHost() {
    // Host will be anything between @ and : or / in authority
    if (!host) {
        host = "";
        String tempUrl = getRawAuthority();
        if (tempUrl) {
            int startloc = tempUrl.indexOf(F("@"), 0);
            if (startloc) {
                tempUrl.substring(startloc + 1);
            }
            int endloc = tempUrl.indexOf(F(":"), 0);
            if (endloc) {
                tempUrl.substring(0, endloc - 1);
            }
        }
    }
    host.toLowerCase();
    return host;
}

word LCBUrl::getPort() {
    // Port will be any integer between : and / in authority
    if (!port) {
        port = 0;
        String tempUrl = getRawAuthority();
        if (tempUrl) {
            int startloc = tempUrl.lastIndexOf(F(":"));
            if (startloc) {
                tempUrl.substring(startloc + 1);
            }
            int endloc = tempUrl.lastIndexOf(F("/"));
            if (endloc) {
                tempUrl.substring(0, endloc - 1);
            }
            port = tempUrl.toDouble();
        }
    }
    if ((getScheme() == F("http")) && (port == 80)) port = 0;
    if ((getScheme() == F("https")) && (port == 443)) port = 0;
    return port;
}

String LCBUrl::getAuthority() {
    // Authority is similar to "lbussy:password@raspberrypi.local:80"
    if (!authority) {
        authority = "";
        if (getUserName().length() > 0) {
            authority = getUserName();
        }
        if (getPassword().length() > 0) {
            authority.concat(F(":"));
            authority.concat(getPassword());
        }
        authority.concat(getHost());
        if (getPort() > 0) {
            authority.concat(F(":"));
            authority.concat(String(getHost()));
        }
    }
    return authority;
}

String LCBUrl::getPath() {
    if (!path) {
        path = getPathSegment();
        // TODO:  Figure this shit out - remove dot segments
    }
    return path;
}

String LCBUrl::getQuery() {
    if (!query) {
        query="";
        String tempUrl = getAfterPath();
        int fraglog = tempUrl.lastIndexOf(F("#"));
        query = tempUrl.substring(0, fraglog - 1);
    }
    return query;
}

String LCBUrl::getFragment() {
    if (!fragment) {
        fragment = "";
        String tempUrl = getAfterPath();
        int fraglog = tempUrl.lastIndexOf(F("#"));
        fragment = tempUrl.substring(fraglog + 1);
    }
    return fragment;
}

// Private Methods /////////////////////////////////////////////////////////////
// Functions only available to other functions in this library

bool LCBUrl::parseUrl(String newUrl) {
    getUrl();
}

String LCBUrl::getStripScheme() { // Remove scheme and "://" discriminately
    if (!stripscheme) {
        stripscheme = "";
        String tempUrl = getCleanTriplets();
        int length = getCleanTriplets().length();
        int slength = getScheme().length();
        
        // Remove scheme and ://
        if ((slength > 0) && (length > slength + 3)) {
            tempUrl.remove(0, slength); // Remove scheme
            for (int i; i <=3; i++) { // Remove "://" discriminately
                if (isAlphaNumeric(tempUrl.charAt(i))) {
                    tempUrl.remove(i, tempUrl.length());
                    stripscheme = tempUrl;
                }
            }
        }
    }
    return stripscheme;
}

String LCBUrl::getAfterAuth() { // Get anything after the authority
    if (!afterauth) {
        afterauth = "";
        String tempUrl = getStripScheme();
        int length = getRawAuthority().length();
      
        if (tempUrl.length() > length + 1) {
            tempUrl.remove(length + 1); // Remove authority
            afterauth = tempUrl;
        }
    }
    return afterauth;
}

String LCBUrl::getAfterPath() { // Get anything after the path
    if (!afterpath) {
        afterpath = "";
        String tempUrl = getCleanTriplets();
        int queryloc = tempUrl.lastIndexOf(F("?"));
        if (queryloc) {
            afterpath = tempUrl.substring(queryloc + 1);
        }
    }
    return afterpath;
}

String LCBUrl::getCleanTriplets() {
    if (!cleantriplets) {
        cleantriplets = getUrl();
        int i = 0;
        while (i < getUrl().length()) {
            int loc = getUrl().indexOf(F("%"), i);
            if (loc) {
                String triplet = getUrl().substring(loc + 1, loc + 3);
                triplet.toUpperCase();
                const char* hex = triplet.c_str();

                // Convert hex string to a character
                int x;
                char *endptr;
                x = strtol(hex, &endptr, 16);
                char character = char(x);

                // Check for characters which should be decoded
                if (
                    (isAlphaNumeric(character)) ||
                    (character == '-') ||
                    (character == '.') ||
                    (character == '_') ||
                    (character == '~')){
                        String before = cleantriplets.substring(0, loc-1);
                        String after = cleantriplets.substring(loc + 3);
                        cleantriplets = before + character + after;
                }
            } else {
                i = getUrl().length();
            }
        }
    }
    return cleantriplets;
}

String LCBUrl::getPathSegment() {
    // Path will be between the / after host and ?
    // Add a trailing slash if no filename given
    if (!pathsegment) {
        String tempUrl = getStripScheme();
        int startloc = tempUrl.indexOf(F("/"));
        if (startloc) {
            tempUrl.substring(startloc + 1);
        }
        int endloc = tempUrl.lastIndexOf(F("?"));
        if (endloc) {
            tempUrl.substring(0, endloc - 1);
        }
        int lastpath = tempUrl.lastIndexOf(F("/"));
        if ((lastpath) && (lastpath < tempUrl.length())) { // Filename exists
            int dotloc = tempUrl.lastIndexOf(F("."));
            if (!dotloc) {
                tempUrl.concat(F("/"));
            }
        }
        pathsegment = tempUrl;
    }
    return pathsegment;
}
