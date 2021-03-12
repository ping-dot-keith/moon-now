/* cc moon.c -lm -o moon */
/* alt az and phase of moon *now* on a Unix-like OS */

#include <stdio.h>
#include <time.h>
#include <math.h>

/* Your latitude and (east) longitude */
#define LONG -1.903
#define LAT  52.4797

/* Some constants */
#define J2000 946728000 /* J2000.0 in seconds since Unix epoch */    
#define RADS 0.0174532925199433
#define DEGS 57.2957795130823
#define PI 3.1415927

double range(double y);

int main(void) {

time_t now;
double d, t, lambda, beta, paralx, ut, utdays;
double l, m, n, alpha, delta;
double x, y, z, x1, y1, z1;
double theta, talpha, tdelta, r, tr, tparalx;
double ha, x2, y2, z2, xh, yh, zh, alt, az;
double L, g, slambda;
double elong, pa, phase;

/*  Get Unix time in seconds UT then find
    days and Julian centuries since J2000.0 */

    now = time(NULL);
    if (now == -1) {
        puts("The time() function failed");
    }

    d = (now - J2000)/(86400.0); /* days since J2000.0 */
    t = d / 36525.0;


/*  Most of calculations below from Astronomical Almanac
    page D46 'Low-precision formulae for geocentric coodinates
    of the Moon */

/*  Geocentric ecliptic coords of Moon */

    lambda = 218.32 + 481267.883 * t;
    lambda += 6.29 * sin(RADS * range(134.9 + 477198.85 * t));
    lambda -= 1.27 * sin(RADS * range(259.2 - 413335.38 * t));
    lambda += 0.66 * sin(RADS * range(235.7 + 890534.23 * t));    
    lambda += 0.21 * sin(RADS * range(269.9 + 954397.70 * t));
    lambda -= 0.19 * sin(RADS * range(357.5 +  35999.05 * t));
    lambda -= 0.11 * sin(RADS * range(186.6 + 966404.05 * t));


    beta += 5.13 * sin(RADS * range( 93.3 + 483202.03 * t));
    beta += 0.28 * sin(RADS * range(228.2 + 960400.87 * t));
    beta -= 0.28 * sin(RADS * range(318.3 +   6003.18 * t));
    beta -= 0.17 * sin(RADS * range(217.6 - 407332.20 * t));


    paralx += 0.9508;
    paralx += 0.0518 * cos(RADS * range(134.9 + 477198.85 * t));
    paralx += 0.0095 * cos(RADS * range(259.2 - 413335.38 * t));
    paralx += 0.0078 * cos(RADS * range(235.7 + 890534.23 * t));    
    paralx += 0.0028 * cos(RADS * range(269.9 + 954397.70 * t));


/*  Geocentric RA and Dec of Moon
    The D46 page uses fixed value 23.44 deg for 
    obliquity of ecliptic */

    l = cos(RADS * beta) * cos(RADS * lambda);
    m = 0.9175 * cos(RADS * beta) * sin(RADS * lambda); 
    m -= 0.3978 * sin(RADS * beta);
    n = 0.3978 * cos(RADS * beta) * sin(RADS * lambda); 
    n += 0.9175 * sin(RADS * beta);
    delta = asin(n);
    alpha = atan2(m,l);


/*  Topocentric RA and Dec of Moon
    From Astronomical Almanac, D46
    'Low precision formulae for topocentric
    coordinates of the Moon'              */

    r = 1 / sin(RADS*paralx);
    x = r * l;
    y = r * m;
    z = r * n;

/*  Local sidereal time needed for next stage       */
/*  UT from the Unix time count as fraction of day
    utdays isn't used anywhere                      */
    ut = modf(now / 86400.0, &utdays);
/*  D46 page simplified formula for LST             */
    theta = range(100.46 + 36000.77 * t + LONG + ut*360);

/*  Vector subtraction for our position on Earth sphere */
    x1 = x - cos(LAT * RADS) * cos (theta * RADS);
    y1 = y - cos(LAT * RADS) * sin (theta * RADS);
    z1 = z - sin(LAT * RADS);
    tr = sqrt(x1 * x1 + y1 * y1 + z1 * z1);

/*  Topocentric Dec and RA */
    tdelta = asin(z1 / tr);
    talpha = atan2(y1, x1);

/*  Horizon coordinates of Moon. See   
    https://stjarnhimlen.se/comp/ppcomp.html#12b
    for formulas */

    ha = theta * RADS - talpha;
    x2 = cos(ha) * cos(tdelta);
    y2 = sin(ha) * cos(tdelta);
    z2 = sin(tdelta);
    xh = x2 * sin(LAT * RADS) - z2 * cos(LAT * RADS);
    yh = y2;
    zh = x2 * cos(LAT * RADS) + z2 * sin(LAT * RADS);
    alt = asin(zh);
    az  = atan2(yh , xh) + PI;
    printf(">   alt: %.1f   az: %.1f", alt * DEGS, az * DEGS);

/*  Ecliptic longitude of Sun using page C24 from AA */
    
    L = range(280.461 + 0.9856474 * d); 
    g = range(357.528 + 0.9856003 * d);
    slambda = L + 1.915 * sin(RADS * g) + 0.020 * sin(RADS * 2 * g);

/*  Phase of Moon from
    https://stjarnhimlen.se/comp/ppcomp.html#15 */
    elong = acos(cos(RADS * (slambda - lambda)) * cos(beta * RADS));
    pa = PI - elong;
    phase = ( 1 + cos(pa) ) / 2;
    printf("   pa: %.0f    phase: %.2f\n", pa * DEGS, phase);

    return 0;
}

/* returns an angle in degrees in the range 0 to 360 */
double
range(double x) {
        double a, b;
        b = x / 360;
    a = 360 * (b - floor(b));
    if (a < 0)
                a = 360 + a;
    return(a);
        }
