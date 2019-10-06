/*
    LCBUrl.cpp -  Library for handling URLs - This library will allow
                handling and manipulation of URLs according to RFC3986.
            
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

// include this library's description file
#include "LCBUrl.h"

// Going to be receiving a URL that looks like this:
// http://b97945a0-dffc-427f-b81c-cf811e96faf1.mock.pstmn.io/status
//
// TODO:
//  [ ] Normalize URL: https://tools.ietf.org/html/rfc3986
//      [ ] Convert percent-encoded triplets to uppercase: https://tools.ietf.org/html/rfc3986#section-6.2.2.1
//      [ ] Convert the scheme and host to lowercase (not User@) : https://tools.ietf.org/html/rfc3986#section-6.2.2.1
//      [ ] Decode percent-encoded triplets of unreserved characters: https://tools.ietf.org/html/rfc3986#section-6.2.2.2
//          Percent-encoded triplets of the URI in the ranges of ALPHA
//          (%41–%5A and %61–%7A), DIGIT (%30–%39), hyphen (%2D), period
//          (%2E), underscore (%5F), or tilde (%7E) do not require 
//          percent-encoding and should be decoded to their corresponding
//          unreserved characters.
//      [ ] Remove dot-segments: https://tools.ietf.org/html/rfc3986#section-6.2.2.3
//      [ ] Convert an empty path to a "/" path: : https://tools.ietf.org/html/rfc3986#section-6.2.3
//      [ ] Remove the default port : https://tools.ietf.org/html/rfc3986#section-6.2.3
//  [ ] Get scheme
//  [ ] Get host (up to first : or /)
//  [ ] Get port if it exists
//  [ ] Get path if it exists
//  [ ] Get user if it exists

// Constructor/Destructor ////////////////////////////////////////////////
// Handle the creation, setup, and destruction of instances

LCBUrl::LCBUrl() {
    // Initialize this instance's variables
    url = "";
    host = "";
    port = 80;
    path = "";
    userinfo = "";

    // Initialize the library

}

LCBUrl::~LCBUrl() { }

// Public Methods //////////////////////////////////////////////////////////////
// Functions available in sketches, this library, and other libraries

bool LCBUrl::setUrl(String newUrl) {
    return parseUrl(newUrl);
}

String LCBUrl::getUrl() {
    return url;
}

String LCBUrl::getScheme() {
    return scheme;
}

String LCBUrl::getAuthority() {
    return authority;
}

String LCBUrl::getUserInfo() {
    return userinfo;
}

String LCBUrl::getHost() {
    return host;
}

word LCBUrl::getPort() {
    return port;
}

String LCBUrl::getPath() {
    return path;
}

String LCBUrl::getQuery() {
    return query;
}

String LCBUrl::getFragment() {
    return fragment;
}

// Private Methods /////////////////////////////////////////////////////////////
// Functions only available to other functions in this library

bool LCBUrl::parseUrl(String newUrl) {
    // http://lbussy@b97945a0-dffc-427f-b81c-cf811e96faf1.mock.pstmn.io:80/status
    url = "http://b97945a0-dffc-427f-b81c-cf811e96faf1.mock.pstmn.io/status"; // Normalize according to RFC3986
    scheme = "http"; // Normalize to http or https, lowercase
    authority = "lbussy@b97945a0-dffc-427f-b81c-cf811e96faf1.mock.pstmn.io:80";
    userinfo = "lbussy"; // Do not normalize, may include :password
    host = "b97945a0-dffc-427f-b81c-cf811e96faf1.mock.pstmn.io"; // Normalize to lowercase
    port = 80;
    path = "status";
    query="";
    fragment = "";
    return true; // Validate URL was able to be parsed
}
