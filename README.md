# LCBUrl - An Arduino library for handling URLs

[![Arduino Library](https://www.ardu-badge.com/badge/LCBUrl.svg?style=plastic)](https://www.ardu-badge.com/LCBUrl)
[![GitHub Release](https://img.shields.io/github/v/release/lbussy/LCBUrl.svg?style=plastic)](https://github.com/lbussy/LCBUrl/releases)
[![MIT License](https://img.shields.io/github/license/lbussy/LCBUrl?style=plastic)](https://github.com/lbussy/LCBUrl/blob/master/LICENSE)
[![GitHub Issues](https://img.shields.io/github/issues/lbussy/LCBUrl?style=plastic)](http://github.com/lbussy/LCBUrl/issues)
[![GitHub Pull Requests](https://img.shields.io/github/issues-pr/lbussy/LCBUrl?style=plastic)](http://github.com/lbussy/LCBUrl/pulls)
[![Contributors Welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=plastic)](#Contributing)

This library was written by and for a non-programmer.  If you find some use out of it, that will make me happy, but if not, I'm still using it in my projects.

This library will parse a URL, and normalize it according to the information provided in [RFC3986](https://tools.ietf.org/html/rfc3986).  For instance, passing in `http://%7EFoo:%7Ep@$$word@servername.local:80/%7Ethis/is/a/./path/test.php?foo=bar#frag` will allow you to access the hostname, user name, password, path, query string, fragment, etc..

If you are using mDNS in your projects, you may have discovered many microcontroller libraries do not support mDNS in all areas.  This library will additionally re-parse a URL and replace the `*.local` hostname with the resolved IP address.

- getUrl() = http://~Foo:~p@$$word@<span/>servername.local/~this/is/a/./path/test.php?foo=bar#frag
- getIPUrl() = http://~Foo:~p@$$word@<span/>XXX.XXX.XXX.XXX/~this/is/a/./path/test.php?foo=bar#frag
- getScheme() = http
- getUserInfo() = ~Foo:~p@$$word
- getUserName() = ~Foo
- getPassword() = ~p@$$word
- getHost() = servername.local
- getIP() = XXX.XXX.XXX.XXX
- getPort() = 80
- getAuthority() = ~Foo:~p@$$word@<span/>servername.local
- getIPAuthority() = ~Foo:~p@$$word@<span/>XXX.XXX.XXX.XXX
- getPath() = ~this/is/a/./path/test.php
- getAfterPath() = ?foo=bar#frag
- getQuery() = foo=bar
- getFragment() = frag
- isMDNS() = Boolean

:exclamation: IMPORTANT: Using any of the IP-based methods in a timer will crash or hang your program. This hang is not a shortcoming of this library; it is how radio-functions (networking being one) work.

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
- `String getIP()` - Return IP address of host (always does lookup)
- `word getPort()` - Return port (if present) if non-standard
- `String getAuthority()` - Return the authority (if present) in the following format: `[userinfo@]host[:port]`
- `String getIPAuthority()` - Return the authority (if present) in the following format: `[userinfo@]XXX.XXX.XXX.XXX[:port]` (useful for mDNS URLs, will use cached IPs if they exist)
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
- [X] ~~Add a trailing "/" to a non-empty path (may remove this)~~ (removed this after some thought)

## Installation

Installation is particular to the platform with which you are developing:

### PlatformIO

This library is available as `LCBUrl` in [PlatformIO's library manager](https://platformio.org/lib/show/6778/LCBUrl).

### Arduino IDE

LCBUrl is published via the [Arduino Library Manager](https://www.ardu-badge.com/LCBUrl).  You can include this library in your project by navigating to Sketch -> Include Libraries -> Manage Libraries (or Ctrl - Shift - I). Type `LCBUrl` in the search bar:

### Manual

To install this library, place this entire folder as a subfolder in your
`./lib/targets/libraries` folder.

When installed, this library should contain the following files:

```
./lib/targets/libraries/LCBUrl                    (this library's folder)
./lib/targets/libraries/LCBUrl/examples           (the examples in the "open" menu)
./lib/targets/libraries/LCBUrl/keywords.txt       (the syntax coloring file)
./lib/targets/libraries/LCBUrl/library.properties (properties of this library)
./lib/targets/libraries/LCBUrl/LICENSE            (the license for this library)
./lib/targets/libraries/LCBUrl/README.md          (this file)
./lib/targets/libraries/LCBUrl/src/LCBUrl.cpp     (the library implementation file)
./lib/targets/libraries/LCBUrl/src/LCBUrl.h       (the library description file)
```

## Building

Ensure that you add a corresponding line to the top of your sketch:

`#include <LCBUrl.h>`

To stop using this library, delete that line from your sketch.

## Contributing

If you would like to contribute to this project:

- Report bugs and errors
- Ask for enhancements
- Create issues and pull requests
- Tell other people about this library


## License

The library is licensed under the [MIT License](https://github.com/lbussy/LCBUrl/blob/master/LICENSE).


## Copyright

LCBUrl is Copyright &copy; 2020 by Lee C Bussy.
