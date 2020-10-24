double sdate;
double phases[5];
{
        double adate, k1, k2, nt1, nt2;

        adate = sdate - 45;
        nt1 = meanphase(adate, 0.0, &k1);
        while (1) {
           adate += synmonth;
           nt2 = meanphase(adate, 0.0, &k2);
           if (nt1 <= sdate && nt2 > sdate)
              break;
           nt1 = nt2;
           k1 = k2;
        }
        phases[0] = truephase(k1, 0.0);
        phases[1] = truephase(k1, 0.25);
        phases[2] = truephase(k1, 0.5);
        phases[3] = truephase(k1, 0.75);
        phases[4] = truephase(k2, 0.0);
}
