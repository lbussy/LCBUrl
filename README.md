# LCBUrl - An Arduino library for handling URLs

This library is absolutely not ready for anyone to even look at it let alone try to do something with it.

Installation
--------------------------------------------------------------------------------

To install this library, just place this entire folder as a subfolder in your
Arduino/lib/targets/libraries folder.

When installed, this library should look like:
```
Arduino/lib/targets/libraries/LCBUrl                    (this library's folder)
Arduino/lib/targets/libraries/LCBUrl/src/LCBUrl.cpp     (the library implementation file)
Arduino/lib/targets/libraries/LCBUrl/src/LCBUrl.h       (the library description file)
Arduino/lib/targets/libraries/LCBUrl/keywords.txt       (the syntax coloring file)
Arduino/lib/targets/libraries/LCBUrl/examples           (the examples in the "open" menu)
Arduino/lib/targets/libraries/LCBUrl/LICENSE            (the license for this library)
Arduino/lib/targets/libraries/LCBUrl/README.md          (this file)
```

Building
--------------------------------------------------------------------------------

After this library is installed, you just have to start the Arduino application.
You may see a few warning messages as it's built.

To use this library in a sketch, go to the Sketch | Import Library menu and
select LCBUrl.  This will add a corresponding line to the top of your sketch:
#include <LCBUrl.h>

To stop using this library, delete that line from your sketch.

Geeky information:
After a successful build of this library, a new file named "LCBUrl.o" will appear
in "Arduino/lib/targets/libraries/LCBUrl". This file is the built/compiled library
code.

If you choose to modify the code for this library (i.e. "LCBUrl.cpp" or "LCBUrl.h"),
then you must first 'unbuild' this library by deleting the "LCBUrl.o" file. The
new "LCBUrl.o" with your code will appear after the next press of "verify"
