/*
    LCBUrl.cpp -  Library for handling URLs - This library will allow
                  handling and manipulation of URLs according to RFC3986.
*/

/*
    MIT License

    Copyright (c) 2019-2022 Lee C. Bussy

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

LCBUrl::LCBUrl(const String &newUrl)
{
    initRegisters();
    if (!newUrl.isEmpty())
        setUrl(newUrl);
}

// Public Methods //////////////////////////////////////////////////////////////
// Functions available in sketches, this library, and other libraries

bool LCBUrl::setUrl(const String &newUrl)
{
    bool retVal = false;
    if (!newUrl.isEmpty())
    {
        initRegisters();
        rawurl = newUrl;
    }
    if (!rawurl.isEmpty())
    {
        if (!getUrl().isEmpty())
        {
            retVal = true;
        }
    }
    return retVal;
}

String LCBUrl::getUrl() // Returned parsed/normalized URL
{
    return getUrl(false, url);
}

String LCBUrl::getIPUrl() // Return cleaned URL with IP instead of FQDN
{
    return getUrl(true, ipurl);
}

String LCBUrl::getUrl(bool ipaddr, String &url_string)
{
    if (url_string.isEmpty())
    {
        url_string = "";
        url_string.concat(getScheme());
        url_string.concat(F("://"));
        if (ipaddr)
            url_string.concat(getIPAuthority());
        else
            url_string.concat(getAuthority());
        url_string.concat(F("/"));
        if (!getPath().isEmpty())
        {
            url_string.concat(getPath());
        }
        if (!getQuery().isEmpty())
        {
            url_string.concat(F("?"));
            url_string.concat(getQuery());
        }
        if (getFragment() != "")
        {
            url_string.concat(F("#"));
            url_string.concat(getFragment());
        }
        if ((getScheme() == "") || (getHost() == ""))
        {
            return url_string;
        }
    }
    return url_string;
}

String LCBUrl::getScheme() // Returns URL scheme
{                          // Currrently only finds http and https as scheme
    if (scheme.isEmpty())
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

String LCBUrl::getUserInfo() // Return username:passsword
{
    // UserInfo will be anything to the left of the last @ in authority
    if (userinfo.isEmpty())
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

String LCBUrl::getUserName() // Return username from authority
{
    // User Name will be anything to the left of : in userinfo
    if (username.isEmpty())
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

String LCBUrl::getPassword() // Return password from authority
{
    // Password will be anything to the right of : in userinfo
    if (password.isEmpty())
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

String LCBUrl::getHost() // Return FQDN
{
    // Host will be anything between @ and : or / in authority
    if (host.isEmpty())
    {
        host = "";
        String tempUrl = getRawAuthority();
        if (tempUrl)
        {
            int startloc = tempUrl.lastIndexOf(F("@"));
            if (startloc > 0)
            {
                tempUrl = tempUrl.substring(startloc + 1, tempUrl.length());
            }
            int endloc = tempUrl.indexOf(F(":"), 0);
            if (endloc > 0)
            {
                tempUrl = tempUrl.substring(0, endloc);
            }
            host = tempUrl;
        }
    }
    host.toLowerCase();
    return host;
}

unsigned int LCBUrl::getPort() // Port will be any integer between : and / in authority
{
    if (port == 0)
    {
        String tempUrl = getRawAuthority();
        if (!tempUrl.isEmpty())
        {
            int startloc = tempUrl.lastIndexOf(F(":"));
            int endloc = tempUrl.lastIndexOf(F("/"));
            if (startloc != -1)
                tempUrl = tempUrl.substring(startloc + 1);
            if (endloc != -1)
                tempUrl = tempUrl.substring(0, endloc - 1);
            if (startloc != -1)
                port = tempUrl.toInt();
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

String LCBUrl::getAuthority() // Returns {username (optional)}:{password (optional)}@{fqdn}
{
    return getAuthority(false, authority);
}

String LCBUrl::getIPAuthority() // Returns {username (optional)}:{password (optional)}@{ip_address}
{
    return getAuthority(true, ipauthority);
}

String LCBUrl::getAuthority(bool ipaddr, String &authority_string)
{
    if (authority_string.isEmpty())
    {
        authority_string = "";
        if (!getUserName().isEmpty())
        {
            authority_string = getUserName();
        }
        if (!getPassword().isEmpty())
        {
            authority_string.concat(F(":"));
            authority_string.concat(getPassword());
        }
        if (!authority_string.isEmpty())
        {
            authority_string.concat(F("@"));
        }
        if (ipaddr)
        {
            ipaddress = getIP(getHost());

            if (ipaddress == (IPAddress)IPADDR_NONE || ipaddress.toString().equalsIgnoreCase("(IP unset)"))
            {
                authority_string.concat("255.255.255.255");
            }
            else
            {
                authority_string.concat(ipaddress.toString());
            }
        }
        else
        {
            authority.concat(getHost());
        }

        if (getPort() > 0)
        {
            if (
                ((getScheme() == F("http")) && (port != 80)) ||
                ((getScheme() == F("https")) && (port != 443)))
            {
                authority_string.concat(F(":"));
                authority_string.concat(String(getPort()));
            }
        }
    }
    return authority_string;
}

String LCBUrl::getPath() // Get all after host and port, before query and frag
{
    if (path.isEmpty())
    {
        if (!getPathSegmentNoFile().isEmpty())
        {
            path = getPathSegmentNoFile();
            if (!getFileName().isEmpty())
            {
                path.concat(F("/"));
                path.concat(getFileName());
            }
        }
        path = getPathSegment();
        // TODO: Remove dot segments per 5.2.4
    }
    return path;
}

String LCBUrl::getFileName()
{
    if (filename.isEmpty())
    {
        String tempUrl = getStripScheme();

        int startloc = tempUrl.lastIndexOf(F("/"));
        if (startloc > 0)
        {
            tempUrl = tempUrl.substring(startloc + 1);
        }
        else
        {
            tempUrl = "";
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
        if (tempUrl.indexOf(".") != -1)
            filename = tempUrl;
    }
    return filename;
}

String LCBUrl::getQuery() // Get text after '?' and before '#'
{
    if (query.isEmpty())
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

String LCBUrl::getFragment() // Get all after '#'
{
    if (fragment.isEmpty())
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

bool LCBUrl::hasEnding(std::string const &fullString, std::string const &ending)
{
    if (fullString.length() >= ending.length())
    {
        return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
    }
    else
    {
        return false;
    }
}

String LCBUrl::getStripScheme() // Remove scheme and "://" discriminately
{
    if (stripscheme.isEmpty())
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

String LCBUrl::getRawAuthority() // Authority is similar to "lbussy@raspberrypi.local:80"
{
    if (rawauthority.isEmpty())
    {
        rawauthority = "";
        String tempUrl = getStripScheme();
        if (tempUrl && !tempUrl.isEmpty())
        {
            int loc = tempUrl.indexOf(F("/"), 0);
            if (loc > 0)
            {
                rawauthority = tempUrl.substring(0, loc);
            }
            else
            {
                rawauthority = tempUrl;
            }
        }
    }
    return rawauthority;
}

String LCBUrl::getDotSegmentsClear()
{
    // TODO:  https://tools.ietf.org/html/rfc3986#section-5.2.4
    return "TODO";
}

String LCBUrl::getAfterAuth() // Get anything after the authority
{
    if (afterauth.isEmpty())
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

String LCBUrl::getCleanTriplets() // Convert URL encoded triplets
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
    return workingurl;
}

String LCBUrl::getPathSegment() // Path will be between the / after host and ?
{
    if (pathsegment.isEmpty())
    {
        String tempUrl = getStripScheme();

        int startloc = tempUrl.indexOf(F("/"));
        if (startloc > 0)
        {
            tempUrl = tempUrl.substring(startloc + 1);
        }
        else
        {
            tempUrl = "";
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
        pathsegment = tempUrl;
    }
    // TODO: Remove dot segments per 5.2.4
    return pathsegment;
}

String LCBUrl::getAfterPath() // Get anything after the path
{
    if (afterpath.isEmpty())
    {
        afterpath = "";
        String tempUrl = getStripScheme();
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

String LCBUrl::getAfterPathNoFile() // Get anything after the proper RFC-defined path
{
    if (afternofilepath.isEmpty())
    {
        afternofilepath = "";
        String tempUrl = getStripScheme();
        int queryloc = tempUrl.lastIndexOf(F("/"));
        if (queryloc != -1)
        {
            afternofilepath = tempUrl.substring(queryloc + 1);
        }
        else
        {
            int queryloc = tempUrl.lastIndexOf(F("?"));
            if (queryloc != -1)
            {
                afternofilepath = tempUrl.substring(queryloc);
            }
            else
            {
                int queryloc = tempUrl.lastIndexOf(F("#"));
                if (queryloc != -1)
                {
                    afternofilepath = tempUrl.substring(queryloc);
                }
            }
        }
    }
    return afternofilepath;
}

String LCBUrl::getPathSegmentNoFile() // Path will be between the / after host and ?
{
    if (nofilepathsegment.isEmpty())
    {
        String tempUrl = getStripScheme();

        int startloc = tempUrl.indexOf(F("/"));
        if (startloc > 0)
        {
            tempUrl = tempUrl.substring(startloc + 1);
        }
        else
        {
            tempUrl = "";
        }

        int endloc = tempUrl.lastIndexOf(F("/"));
        if (endloc != -1)
        {
            tempUrl = tempUrl.substring(0, endloc + 1);
        }
        else
        {
            int endloc = tempUrl.lastIndexOf(F("#"));
            if (endloc != -1)
            {
                tempUrl = tempUrl.substring(0, endloc - 1);
            }
        }
        nofilepathsegment = tempUrl;
    }
    // TODO: Remove dot segments per 5.2.4
    return nofilepathsegment;
}

void LCBUrl::initRegisters() // Clear out the internals to allow the object to be re-used
{
    rawurl = "";
    url = "";
    ipurl = "";
    workingurl = "";
    scheme = "";
    stripscheme = "";
    rawauthority = "";
    afterauth = "";
    userinfo = "";
    username = "";
    password = "";
    host = "";
    ipaddress = IPADDR_NONE;
    port = 0;
    authority = "";
    ipauthority = "";
    pathsegment = "";
    nofilepathsegment = "";
    path = "";
    afterpath = "";
    afternofilepath = "";
    filename = "";
    query = "";
    fragment = "";
}

// Utility Methods //////////////////////////////////////////////////////////////
// These do not directly influence or change the core library properties

bool LCBUrl::isMDNS() // (deprecated) Determine if FQDN is mDNS
{
    return isMDNS(getHost().c_str());
}

bool LCBUrl::isMDNS(const char *fqdn) // Determine if FQDN is mDNS
{
    // Check for a valid mDNS name
    int lCount = labelCount(fqdn);
    if (lCount == 0 || lCount > 3)
        // Cannot have more than two labels (plus "local")
        // https://github.com/lathiat/nss-mdns/blob/master/README.md#etcmdnsallow
        return false;

    if (!hasEnding(fqdn, ".local"))
        return false;
    else
        return true;
}

IPAddress LCBUrl::getIP() // (deprecated) Return IP address of FQDN (helpful for mDNS)
{
    return getIP(getHost().c_str());
}

IPAddress LCBUrl::getIP(String fqdn) // Return IP address of FQDN (helpful for mDNS)
{
    return getIP(fqdn.c_str());
}

IPAddress LCBUrl::getIP(const char *fqdn) // Return IP address of FQDN (helpful for mDNS)
{
    IPAddress returnIP = IPADDR_NONE;
    // First try to resolve the address fresh
    if (isMDNS(fqdn))
    { // Host is an mDNS name
#ifdef LCBURL_MDNS
#ifdef ESP8266
        int result = WiFi.hostByName(fqdn, &returnIP); // TODO: This is broken

        if (result == 1)
        {
            if (returnIP != IPADDR_NONE)
                ipaddress = returnIP;
        }
        else
            return ipaddress;
#else // ESP32
      // May be able to use mDNS here for ESP32
      //      https://www.tutorialfor.com/questions-324359.htm

#if defined(ESP_ARDUINO_VERSION) && defined(ESP_ARDUINO_VERSION_VAL)
#define WM_ARDUINOVERCHECK ESP_ARDUINO_VERSION >= ESP_ARDUINO_VERSION_VAL(2, 0, 0)
#endif

#if WM_ARDUINOVERCHECK
      // Arduino Core 2.x
        esp_ip4_addr addr;
#else
      // Arduino Core 1.x
        struct ip4_addr addr;
#endif

        mdns_init();
        if (!hasEnding(fqdn, ".local"))
        {
            ipaddress = MDNS.queryHost(fqdn, 5000 /* ms */);
        }
        else
        {
            std::string fqdnString(fqdn);
            fqdnString = fqdnString.substr(0, fqdnString.find_last_of("."));
            ipaddress = MDNS.queryHost(fqdnString.c_str(), 5000 /* ms */);
        }

#endif // ESP32
#endif // LCBURL_MDNS
    }
    else
    {
        // Host is not an mDNS name
        if (WiFi.hostByName(fqdn, returnIP) == 1)
            ipaddress = returnIP;
    }

    // If we got a new IP address, we will use it.  Otherwise
    // we will use last known good (if there is one), falls back
    // to INADDR_NONE
    return ipaddress;
}

bool LCBUrl::isValidIP(const char *address)
{
    // Check if address is a valid IP address
    IPAddress tempAddress;
    tempAddress.fromString(address);
    if (tempAddress == (IPAddress)IPADDR_NONE || tempAddress == (IPAddress)IPADDR_LOOPBACK || tempAddress == (IPAddress)IPADDR_ANY)
        return false;
    else
        return true;
}

int LCBUrl::labelCount(const char *fqdn)
{
    // Return count of labels in a fqdn
    char *label;
    int labelCount = 0;
    std::string tempFqdn = std::string(fqdn);
    label = strtok((char *)tempFqdn.c_str(), ".");
    while (label != NULL)
    {
        labelCount++;
        label = strtok(NULL, ".");
    }
    return labelCount;
}

bool LCBUrl::isANumber(const char *str)
{
    char *p;
    strtol(str, &p, 10);
    if (*p)
    {
        return false;
    }
    return true;
}

bool LCBUrl::isValidLabel(const char *label)
{
    // Check that hostname label is valid

    // Is at least 1 and no more than 63
    if (strlen(label) < 1 || strlen(label) > 63)
        return false;

    // Does not begin or end with hyphen
    if (label[0] == '-' || label[strlen(label) - 1] == '-')
        return false;

    // Does not contain all numbers
    if (isANumber(label))
        return false;

    // Contains only letters, numbers and hyphen
    for (unsigned int i = 0; i < strlen(label); i++)
    {
        if (!isalnum(label[i]) && label[i] != '-')
            return false;
    }
    return true;
}

bool LCBUrl::isValidHostName(const char *fqdn)
{
    // This will generally follow RFC1123 and RFC1034

    // Check for min/max length (remember root label and octet count)
    if (strlen(fqdn) < 1 || strlen(fqdn) > 253)
        return false;

    // Check if this is a valid IP address
    if (isValidIP(fqdn))
        return true;

#ifdef LCBURL_MDNS
    // Next check for mDNS
    if (isMDNS(fqdn))
        return true;
#endif

    // Next, check to see if each label is valid
    char *label;
    label = strtok((char *)fqdn, ".");
    while (label != NULL)
    {
        if (!isValidLabel(label))
            return false;
        label = strtok(NULL, ".");
    }

    return true;
}
