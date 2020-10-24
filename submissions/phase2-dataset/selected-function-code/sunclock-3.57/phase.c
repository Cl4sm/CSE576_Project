phase(gtime, lat, lon, pphase, mage, dist, angdia, sudist, suangdia)
time_t gtime;
double *lat;                       /* Latitude in degrees/Earth */
double *lon;                       /* Longitude in degrees/Earth */
double *pphase;                    /* Illuminated fraction */
double *mage;                      /* Age of moon in days */
double *dist;                      /* Distance in kilometres */
double *angdia;                    /* Angular diameter in degrees */
double *sudist;                    /* Distance to Sun */
double *suangdia;                  /* Sun's angular diameter */
{
        struct tm ctp;
	double pdate, xg, yg, zg, xe, ye, ze, ra;
        double Day, N, M, Msin, Ec, 
               Lambdasun, ml, MM, MN, Ev, Ae, A3, MmP, MmPrad,
               mEc, A4, lP, V, lPP, lPPnode, NP, Lambdamoon, BetaM,
               MoonAge, MoonPhase,
               MoonDist, MoonDFrac, MoonAng, MoonPar,
               F, SunDist, SunAng;

        /* Calculation of the Sun's position */

        ctp = *gmtime(&gtime);
        pdate = jtime(&ctp);

        Day = pdate - epoch;        /* Date within epoch */
        N = fixangle((360 / 365.2422) * Day); /* Mean anomaly of the Sun */
        M = fixangle(N + elonge - elongp);    /* Convert from perigee
                                       co-ordinates to epoch 1980.0 */
	Msin = sin(torad(M));
        Ec = kepler(M, eccent);     /* Solve equation of Kepler */
        Ec = sqrt((1 + eccent) / (1 - eccent)) * tan(Ec / 2);
        Ec = 2 * todeg(atan(Ec));   /* True anomaly */
        Lambdasun = fixangle(Ec + elongp);  /* Sun's geocentric ecliptic
                                               longitude */
        /* Orbital distance factor */
        F = ((1 + eccent * cos(torad(Ec))) / (1 - eccent * eccent));
        SunDist = sunsmax / F;      /* Distance to Sun in km */
        SunAng = F * sunangsiz;     /* Sun's angular size in degrees */


        /* Calculation of the Moon's position */

        /* Moon's mean longitude */
        ml = fixangle(13.1763966 * Day + mmlong);

        /* Moon's mean anomaly */
        MM = fixangle(ml - 0.1114041 * Day - mmlongp);

        /* Moon's ascending node mean longitude */
        MN = fixangle(mlnode - 0.0529539 * Day);

        /* Evection */
        Ev = 1.2739 * sin(torad(2 * (ml - Lambdasun) - MM));

        /* Annual equation */
        Ae = 0.1858 * Msin;

        /* Correction term */
        A3 = 0.37 * Msin;

        /* Corrected anomaly */
        MmP = MM + Ev - Ae - A3;
	MmPrad = torad(MmP);

        /* Correction for the equation of the centre */
        mEc = 6.2886 * sin(MmPrad);

        /* Another correction term */
        A4 = 0.214 * sin(2 * MmPrad);

        /* Corrected longitude */
        lP = ml + Ev + mEc - Ae + A4;

        /* Variation */
        V = 0.6583 * sin(torad(2 * (lP - Lambdasun)));

        /* True longitude */
        lPP = lP + V;

        /* Corrected longitude of the node */
        NP = MN - 0.16 * Msin;

	/* Relative Longitude with respect to the node */
        lPPnode = torad(lPP - NP);

        /* x, y, z ecliptic coordinates with respect to node */
        xe = cos(lPPnode);
        ye = sin(lPPnode) * cos(torad(minc));
	ze = sin(lPPnode) * sin(torad(minc));

        /* Ecliptic latitude */
        BetaM = todeg(asin(ze));

        /* Ecliptic longitude */
        Lambdamoon = todeg(atan2(ye, xe));
        Lambdamoon += NP;

        /* x, y, z sidereal ecliptic coordinates  */
        xe = cos(torad(Lambdamoon));
        ye = sin(torad(Lambdamoon));

        /* Calculation of the phase of the Moon */

        /* Age of the Moon in degrees */
        MoonAge = lPP - Lambdasun;

        /* Phase of the Moon */
        MoonPhase = (1 - cos(torad(MoonAge))) / 2;

        /* Calculate distance of moon from the centre of the Earth */

        MoonDist = (msmax * (1 - mecc * mecc)) /
           (1 + mecc * cos(torad(MmP + mEc)));

        /* Calculate Moon's angular diameter */

        MoonDFrac = MoonDist / msmax;
        MoonAng = mangsiz / MoonDFrac;

        /* Calculate Moon's parallax */

        MoonPar = mparallax / MoonDFrac;

        *pphase = MoonPhase;

        *mage = synmonth * (fixangle(MoonAge) / 360.0);
        *dist = MoonDist;
        *angdia = MoonAng;
        *sudist = SunDist;
        *suangdia = SunAng;

       /* Rotate to equatorial geocentric coords
          taking into account obliquity of ecliptic of date */

        xg = xe;
        yg = ye * cos(epsrad) - ze * sin(epsrad);
        zg = ye * sin(epsrad) + ze * cos(epsrad);

        ra = todeg(atan2(yg, xg));
        *lat = todeg(asin(zg));
        *lon = fixangle((ra + 180.0 - (gmst(pdate) * 15.0))) - 180.0;

        return fixangle(MoonAge) / 360.0;
}
