double sdate, phase;
double *usek;
{
        int yy, mm, dd;
        double k, t, t2, t3, nt1;

        jyear(sdate, &yy, &mm, &dd);

        k = (yy + ((mm - 1) * (1.0 / 12.0)) - 1900) * 12.3685;

        /* Time in Julian centuries from 1900 January 0.5 */
        t = (sdate - 2415020.0) / 36525;
        t2 = t * t;                /* Square for frequent use */
        t3 = t2 * t;               /* Cube for frequent use */

        *usek = k = floor(k) + phase;
        nt1 = 2415020.75933 + synmonth * k
              + 0.0001178 * t2
              - 0.000000155 * t3
              + 0.00033 * dsin(166.56 + 132.87 * t - 0.009173 * t2);

        return nt1;
}
