# moon-now
Where is the Moon right now?

This program is designed to run in a Unix-like environment (tested on Linux, OpenBSD). When run, the program will query the UNIX time on the system and calculate the following information about the Moon.

* Altitude - number of degrees above the horizon
* Azimuth - number of degrees clockwise from North
* Phase Angle - angle between the Sun and Moon
* Phase - the percentage of the Moon's face that is illuminated

You will need to edit the <code>moon.c</code> file to set your geographical longitude and latitude (nearest degree will do) as the program includes a rough topocentric correction (which can be as high as 1 degree).

I've used the 'Low-precision formulae for geocentric coodinates of the Moon' algorithm that can be found on page D46 of any Astronomical Almanac published in the last thirty years or so. This algorithm gives the RA and DEC of the moon to within about half the Moon's diameter (0.25 degree).  

## Compile

Edit the <code>moon.c</code> to provide your latitude and East longitude (lines 9 and 10) and save

Compile with...

<pre>
$ cc moon.c -lm -o moon
</pre>

## Installation

Just copy the <code>moon</code> executable to anywhere on your path (or run from the directory you compiled it in with <code>./moon</code>).

## ToDo

* Test behaviour for Southern hemisphere and for time zones other than London
* Add a conditional statement to warn if Moon well below the horizon
* Add Rukl map numbers for the terminator (sub-solar point)
* Add a <code>.config</code> file for geographical long and lat
