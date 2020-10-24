double
dayLength(gtime, lat)
time_t  gtime;
double  lat;
{
        double                  duration;
        double                  sundec, junk;
        double                  sinsun, sinpos, sinapp, num;

        sinpos = sin(torad(lat));

        /* Get Sun declination */
        (void) sunParams(gtime, &junk, &sundec, NULL);
        sinsun = sin(torad(sundec));

        /* Correct for the sun apparent diameter and atmospheric diffusion */
        sinapp = sin(torad(SUN_APPRADIUS + atm_refraction));

        num = 1 - sinsun*sinsun - sinpos*sinpos - sinapp*sinapp
                - 2*sinsun*sinpos*sinapp;
        if (num<=0) {
           if (sinsun*sinpos>0) 
             duration = 24.0;
           else
             duration = 0.0;
        } else
             duration = 12.0 + 24.0*atan((sinsun*sinpos+sinapp)/sqrt(num))/PI;

        return duration*3600;
}
