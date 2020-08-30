/*
    LCBUrl.cpp -  Library for handling URLs - This library will allow
                  handling and manipulation of URLs according to RFC3986.
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

// Include this library's description file

#include "LCBUrl.h"

// Constructor/Destructor ////////////////////////////////////////////////
// Handle the creation, setup, and destruction of instances

LCBUrl::LCBUrl()
{
    rawurl = "";
    url = "";
    workingurl = "";
    scheme = "";
    stripscheme = "";
    rawauthority = "";
    afterauth = "";
    userinfo = "";
    username = "";
    password = "";
    host = "";
    port = 65535;
    authority = "";
    pathsegment = "";
    path = "";
    removedotsegments = "";
    afterpath = "";
    query = "";
    fragment = "";
}

// Public Methods //////////////////////////////////////////////////////////////
// Functions available in sketches, this library, and other libraries

bool LCBUrl::setUrl(const String &newUrl)
{
    rawurl = newUrl;
    if (getUrl().length() == 0)
    {
        return false;
    }
    else
    {
        return true;
    }
}

String LCBUrl::getUrl()
{

    if (url.length() == 0)
    {
        url = "";
        url.concat(getScheme());
        url.concat(F("://"));
        url.concat(getAuthority());
        url.concat(F("/"));
        url.concat(getPath());
        if (getQuery() != "")
        {
            url.concat(F("?"));
            url.concat(getQuery());
        }
        if (getFragment() != "")
        {
            url.concat(F("#"));
            url.concat(getFragment());
        }
        if ((getScheme() == "") || (getHost() == ""))
        {
            return url;
        }
    }
    return url;
}

String LCBUrl::getScheme()
{ // Currrently only finds http and https as scheme
    if (scheme.length() == 0)
    {
        scheme = "";
        int loc = getCleanTriplets().indexOf(F(":"));
        if (loc > 0)
        {
            String s = getCleanTriplets().substring(0, loc);
            s.toLowerCase();
            if ((s == F("http")) || (s == F("https")))
            {
                scheme = s;
            }
        }
    }
    return scheme;
}

String LCBUrl::getUserInfo()
{
    // UserInfo will be anything to the left of the last @ in authority
    if (userinfo.length() == 0)
    {
        userinfo = "";
        String tempUrl = getRawAuthority();
        if (tempUrl)
        {
            int loc = tempUrl.lastIndexOf(F("@"));
            if (loc > 0)
            {
                userinfo = tempUrl.substring(0, loc);
            }
        }
    }
    return userinfo;
}

String LCBUrl::getUserName()
{
    // User Name will be anything to the left of : in userinfo
    if (username.length() == 0)
    {
        username = "";
        String tempUrl = getUserInfo();
        if (tempUrl)
        {
            int loc = tempUrl.lastIndexOf(F(":"));
            if (loc > 0)
            {
                username = tempUrl.substring(0, loc);
            }
        }
    }
    return username;
}

String LCBUrl::getPassword()
{
    // Password will be anything to the right of : in userinfo
    if (password.length() == 0)
    {
        password = "";
        String tempUrl = getUserInfo();
        if (tempUrl)
        {
            int loc = tempUrl.lastIndexOf(F(":"));
            if (loc > 0)
            {
                password = tempUrl.substring(loc + 1);
            }
        }
    }
    return password;
}

String LCBUrl::getHost()
{
    // Host will be anything between @ and : or / in authority
    if (host.length() == 0)
    {
        host = "";
        String tempUrl = getRawAuthority();
        if (tempUrl)
        {
            int startloc = tempUrl.lastIndexOf(F("@"));
            if (startloc)
            {
                tempUrl = tempUrl.substring(startloc + 1, tempUrl.length());
            }
            int endloc = tempUrl.indexOf(F(":"), 0);
            if (endloc)
            {
                tempUrl = tempUrl.substring(0, endloc);
            }
            host = tempUrl;
        }
    }
    host.toLowerCase();
    return host;
}

word LCBUrl::getPort()
{
    // Port will be any integer between : and / in authority
    if (port == 65535)
    {
        port = 0;
        String tempUrl = getRawAuthority();
        if (tempUrl.length() > 0)
        {
            int startloc = tempUrl.lastIndexOf(F(":"));
            int endloc = tempUrl.lastIndexOf(F("/"));
            if (startloc != -1)
                tempUrl = tempUrl.substring(startloc + 1);
            if (endloc != -1)
                tempUrl = tempUrl.substring(0, endloc - 1);
            if ((startloc != -1) && (endloc != -1))
                port = tempUrl.toDouble();
        }
    }
    if (port == 0)
    {
        if (getScheme() == F("http"))
            port = 80;
        if (getScheme() == F("https"))
            port = 443;
    }
    return port;
}

String LCBUrl::getAuthority()
{
    if (authority.length() == 0)
    {
        authority = "";
        if (getUserName().length() > 0)
        {
            authority = getUserName();
        }
        if (getPassword().length() > 0)
        {
            authority.concat(F(":"));
            authority.concat(getPassword());
        }
        if (authority.length() > 0)
        {
            authority.concat(F("@"));
        }
        authority.concat(getHost());
        if (getPort() > 0)
        {
            if (
                ((getScheme() == F("http")) && (port != 80)) ||
                ((getScheme() == F("https")) && (port != 443)))
            {
                authority.concat(F(":"));
                authority.concat(String(getPort()));
            }
        }
    }
    return authority;
}

String LCBUrl::getPath()
{
    if (path.length() == 0)
    {
        path = getPathSegment();
        // TODO: Remove dot segments per 5.2.4
    }
    //path.toLowerCase(); // Remove per #11
    return path;
}

String LCBUrl::getQuery()
{
    if (query.length() == 0)
    {
        query = "";
        String tempUrl = getAfterPath();
        int queryloc = tempUrl.lastIndexOf(F("#"));
        if (tempUrl.startsWith(F("?")))
            query = tempUrl.substring(1, queryloc);
        else
            query = tempUrl.substring(0, queryloc);
    }
    return query;
}

String LCBUrl::getFragment()
{
    if (fragment.length() == 0)
    {
        fragment = "";
        String tempUrl = getAfterPath();
        int fragloc = tempUrl.lastIndexOf(F("#"));
        if (fragloc != -1)
        {
            fragment = tempUrl.substring(fragloc + 1);
        }
    }
    return fragment;
}

// Private Methods /////////////////////////////////////////////////////////////
// Functions only available to other functions in this library

String LCBUrl::getStripScheme()
{ // Remove scheme and "://" discriminately
    if (stripscheme.length() == 0)
    {
        stripscheme = "";
        String tempUrl = getCleanTriplets();
        int length = getCleanTriplets().length();
        int slength = getScheme().length();
        // Remove scheme and ://
        if ((slength > 0) && (length > slength + 3))
        {
            tempUrl = tempUrl.substring(slength); // Remove scheme;
            for (int i = 0; i <= 3; i++)
            { // Remove "://" discriminately
                if (
                    (isAlphaNumeric(tempUrl.charAt(i))) ||
                    (tempUrl.charAt(i) == '-') ||
                    (tempUrl.charAt(i) == '.') ||
                    (tempUrl.charAt(i) == '_') ||
                    (tempUrl.charAt(i) == '~'))
                {
                    tempUrl = tempUrl.substring(i);
                    stripscheme = tempUrl;
                }
            }
        }
    }
    return stripscheme;
}

String LCBUrl::getRawAuthority()
{
    // Authority is similar to "lbussy@raspberrypi.local:80"
    if (rawauthority.length() == 0)
    {
        rawauthority = "";
        String tempUrl = getStripScheme();
        if ((tempUrl) && (tempUrl.length() > 0))
        {
            int loc = tempUrl.indexOf(F("/"), 0);
            if (loc > 0)
            {
                rawauthority = tempUrl.substring(0, loc);
            }
        }
    }
    return rawauthority;
}

String LCBUrl::getAfterAuth()
{ // Get anything after the authority
    if (afterauth.length() == 0)
    {
        afterauth = "";
        String tempUrl = getStripScheme();
        unsigned int length = getRawAuthority().length();

        if (tempUrl.length() > length + 1)
        {
            tempUrl.remove(length + 1); // Remove authority
            afterauth = tempUrl;
        }
    }
    return afterauth;
}

String LCBUrl::getAfterPath()
{ // Get anything after the path
    if (afterpath.length() == 0)
    {
        afterpath = "";
        String tempUrl = getCleanTriplets();
        int queryloc = tempUrl.lastIndexOf(F("?"));
        if (queryloc != -1)
        {
            afterpath = tempUrl.substring(queryloc);
        }
        else
        {
            int queryloc = tempUrl.lastIndexOf(F("#"));
            if (queryloc != -1)
            {
                afterpath = tempUrl.substring(queryloc);
            }
        }
    }
    return afterpath;
}

String LCBUrl::getCleanTriplets()
{
    if (workingurl.length() == 0)
    {
        workingurl = rawurl;
        unsigned int i = workingurl.length();
        while (i != 0)
        {
            int loc = workingurl.lastIndexOf(F("%"), i);
            if (loc != -1)
            {
                String triplet = rawurl.substring(loc + 1, loc + 3);
                triplet.toUpperCase();
                const char *hex = triplet.c_str();

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
                    (character == '~'))
                {
                    String before = workingurl.substring(0, loc);
                    String after = workingurl.substring(loc + 3);
                    workingurl = before + character + after;
                }
                i--;
            }
            else
            {
                i = 0;
            }
        }
    }
    return workingurl;
}

String LCBUrl::getPathSegment()
{
    // Path will be between the / after host and ?
    // Add a trailing slash if no filename given
    if (pathsegment.length() == 0)
    {
        String tempUrl = getStripScheme();
        int startloc = tempUrl.indexOf(F("/"));
        if (startloc)
        {
            tempUrl = tempUrl.substring(startloc + 1);
        }
        int endloc = tempUrl.lastIndexOf(F("?"));
        if (endloc != -1)
        {
            tempUrl = tempUrl.substring(0, endloc);
        }
        else
        {
            int endloc = tempUrl.lastIndexOf(F("#"));
            if (endloc != -1)
            {
                tempUrl = tempUrl.substring(0, endloc - 1);
            }
        }
        unsigned int lastpath = tempUrl.lastIndexOf(F("/"));
        if ((lastpath) && (lastpath < tempUrl.length()))
        { // Filename exists
            int dotloc = tempUrl.lastIndexOf(F("."));
            if ((dotloc == -1) || ((dotloc < (int)lastpath) && (!tempUrl.endsWith(F("/")))))
            {
                tempUrl.concat(F("/"));
            }
        }
        pathsegment = tempUrl;
    }
    return pathsegment;
}
