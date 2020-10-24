double truephase(k, phase)
double k, phase;
{
        double t, t2, t3, pt, m, mprime, f;
        int apcor = 0;

        k += phase;                /* Add phase to new moon time */
        t = k / 1236.85;           /* Time in Julian centuries from
                                      1900 January 0.5 */
        t2 = t * t;                /* Square for frequent use */
        t3 = t2 * t;               /* Cube for frequent use */
        pt = 2415020.75933         /* Mean time of phase */
             + synmonth * k
             + 0.0001178 * t2
             - 0.000000155 * t3
             + 0.00033 * dsin(166.56 + 132.87 * t - 0.009173 * t2);

        m = 359.2242               /* Sun's mean anomaly */
            + 29.10535608 * k
            - 0.0000333 * t2
            - 0.00000347 * t3;
        mprime = 306.0253          /* Moon's mean anomaly */
            + 385.81691806 * k
            + 0.0107306 * t2
            + 0.00001236 * t3;
        f = 21.2964                /* Moon's argument of latitude */
            + 390.67050646 * k
            - 0.0016528 * t2
            - 0.00000239 * t3;
        if ((phase < 0.01) || (abs(phase - 0.5) < 0.01)) {

           /* Corrections for New and Full Moon */

           pt +=     (0.1734 - 0.000393 * t) * dsin(m)
                    + 0.0021 * dsin(2 * m)
                    - 0.4068 * dsin(mprime)
                    + 0.0161 * dsin(2 * mprime)
                    - 0.0004 * dsin(3 * mprime)
                    + 0.0104 * dsin(2 * f)
                    - 0.0051 * dsin(m + mprime)
                    - 0.0074 * dsin(m - mprime)
                    + 0.0004 * dsin(2 * f + m)
                    - 0.0004 * dsin(2 * f - m)
                    - 0.0006 * dsin(2 * f + mprime)
                    + 0.0010 * dsin(2 * f - mprime)
                    + 0.0005 * dsin(m + 2 * mprime);
           apcor = !0;
        } else if ((abs(phase - 0.25) < 0.01 || (abs(phase - 0.75) < 0.01))) {
           pt +=     (0.1721 - 0.0004 * t) * dsin(m)
                    + 0.0021 * dsin(2 * m)
                    - 0.6280 * dsin(mprime)
                    + 0.0089 * dsin(2 * mprime)
                    - 0.0004 * dsin(3 * mprime)
                    + 0.0079 * dsin(2 * f)
                    - 0.0119 * dsin(m + mprime)
                    - 0.0047 * dsin(m - mprime)
                    + 0.0003 * dsin(2 * f + m)
                    - 0.0004 * dsin(2 * f - m)
                    - 0.0006 * dsin(2 * f + mprime)
                    + 0.0021 * dsin(2 * f - mprime)
                    + 0.0003 * dsin(m + 2 * mprime)
                    + 0.0004 * dsin(m - 2 * mprime)
                    - 0.0003 * dsin(2 * m + mprime);
           if (phase < 0.5)
              /* First quarter correction */
              pt += 0.0028 - 0.0004 * dcos(m) + 0.0003 * dcos(mprime);
           else
              /* Last quarter correction */
              pt += -0.0028 + 0.0004 * dcos(m) - 0.0003 * dcos(mprime);
           apcor = !0;
        }
        if (!apcor) {
           fprintf(stderr, "TRUEPHASE called with invalid phase selector.\n");
           abort();
        }
        return pt;
}
