# LCBUrl - An Arduino library for handling URLs

This library was written by and for a non-programmer.  If you find some use out of it, that will make me happy, but if not, I'm still using it in my projects.

This library will parse a url, and normalize it according to the information provided in [RFC3986](https://tools.ietf.org/html/rfc3986).  For instance; passing in `http://%7EFoo:%7Ep@$$word@servername.local:80/%7Ethis/is/a/./path/test.php?foo=bar#frag` will allow you to access the hostname, user name, password, path, query string, fragment, etc. as follows:

- getUrl() = http://~Foo:~p@$$word@<span/>servername.local/~this/is/a/./path/test.php?foo=bar#frag
- getIPUrl() = http://~Foo:~p@$$word@<span/>192.168.0.1/~this/is/a/./path/test.php?foo=bar#frag
- getScheme() = http
- getUserInfo() = ~Foo:~p@$$word
- getUserName() = ~Foo
- getPassword() = ~p@$$word
- getHost() = servername.local
- getIP() = 192.168.0.1
- getPort() = 80
- getAuthority() = ~Foo:~p@$$word@<span/>servername.local
- getIPAuthority() = ~Foo:~p@$$word@<span/>192.168.0.1
- getPath() = ~this/is/a/./path/test.php
- getAfterPath() = ?foo=bar#frag
- getQuery() = foo=bar
- getFragment() = frag
- isMDNS

:exclamation: IMPORTANT: Using any of the IP-based methods in a timer will crash or hang your program. This is not a shortcoming of the library, it is how radio-functions (networking being one) work.

## Public Methods

- `bool setUrl(String)` - Pass the URL to be handled to the class
- `bool isMDNS(String)` - Return true/false based on whether URL is mDNS-based (*.local) or not
- `String getUrl()` - Return a processed URI in the following format: `scheme:[//authority]path[?query][#fragment]`
- `String getIPUrl()` - Return a processed URI with the host replaced by the IP address in the following format: `scheme:[//authority]path[?query][#fragment]` (useful for mDNS URLs)
- `String getScheme()` - Get the scheme (currently only handles http and https)
- `String getUserInfo()` - Return username and password (if present)
- `String getUserName()` - Returns username (if present)
- `String getPassword()` - Returns password (if present)
- `String getHost()` - Return host name
- `String getIP()` - Return IP address of host
- `word getPort()` - Return port (if present) if non-standard
- `String getAuthority()` - Return the authority (if present) in the following format: `[userinfo@]host[:port]`
- `String getIPAuthority()` - Return the authority (if present) in the following format: `[userinfo@]XXX.XXX.XXX.XXX[:port]` (useful for mDNS URLs)
- `String getPath()` - Returns the path segment (if present) with any query or fragment removed
- `String getAfterPath()` - Returns query and fragment segments (if present)
- `String getQuery()` - Returns query (if present)
- `String getFragment()` - Returns fragment (if present)

## Progress:

- [X] Convert percent-encoded triplets to uppercase
- [X] Convert the scheme and host to lowercase
- [X] Decode percent-encoded triplets of unreserved characters
- [ ] Remove dot-segments
- [X] Convert an empty path to a "/" path
- [X] Remove the default port
- [X] Add a trailing "/" to a non-empty path (may remove this)

## Installation
--------------------------------------------------------------------------------

To install this library, place this entire folder as a subfolder in your
Arduino/lib/targets/libraries folder.

When installed, this library should look like:

```
Arduino/lib/targets/libraries/LCBUrl                    (this library's folder)
Arduino/lib/targets/libraries/LCBUrl/examples           (the examples in the "open" menu)
Arduino/lib/targets/libraries/LCBUrl/keywords.txt       (the syntax coloring file)
Arduino/lib/targets/libraries/LCBUrl/library.properties (properties of this libraary)
Arduino/lib/targets/libraries/LCBUrl/LICENSE            (the license for this library)
Arduino/lib/targets/libraries/LCBUrl/README.md          (this file)
Arduino/lib/targets/libraries/LCBUrl/src/LCBUrl.cpp     (the library implementation file)
Arduino/lib/targets/libraries/LCBUrl/src/LCBUrl.h       (the library description file)
```

## Building
--------------------------------------------------------------------------------

After you install this library, you are done. 

You may see a few warning messages as is builds.

To use this library in a sketch, go to the Sketch | Import Library menu and
select LCBUrl.  Selecting the library will add a corresponding line to the top of your sketch:

`#include <LCBUrl.h>`

To stop using this library, delete that line from your sketch.
