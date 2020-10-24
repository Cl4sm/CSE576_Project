void jyear(td, yy, mm, dd)
double td;
int *yy, *mm, *dd;
{
        double j, d, y, m;

        td += 0.5;                 /* Astronomical to civil */
        j = floor(td);
        j = j - 1721119.0;
        y = floor(((4 * j) - 1) / 146097.0);
        j = (j * 4.0) - (1.0 + (146097.0 * y));
        d = floor(j / 4.0);
        j = floor(((4.0 * d) + 3.0) / 1461.0);
        d = ((4.0 * d) + 3.0) - (1461.0 * j);
        d = floor((d + 4.0) / 4.0);
        m = floor(((5.0 * d) - 3) / 153.0);
        d = (5.0 * d) - (3.0 + (153.0 * m));
        d = floor((d + 5.0) / 5.0);
        y = (100.0 * y) + j;
        if (m < 10.0)
           m = m + 3;
        else {
           m = m - 9;
           y = y + 1;
        }
        *yy = y;
        *mm = m;
        *dd = d;
}
