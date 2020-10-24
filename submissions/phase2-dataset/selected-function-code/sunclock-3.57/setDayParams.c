void
setDayParams(Context)
struct Sundata * Context;
{
        struct tm               gtm, ltm;
        double                  duration, junk;
        time_t                  gtime, stime, sr, ss, dl;

        menu_lasthint = ' ';

        if (!Context->mark1.city) return;

        time(&Context->time);
        gtime = Context->time + Context->jump;

        /* Get local time at given location */
        setTZ(Context->mark1.city);
        ltm = *localtime(&gtime);

        /* Get solar time at given location */
        stime = sunParams(gtime, &junk, &junk, Context->mark1.city);

        /* Go to time when it is noon in solartime at Context->mark1.city */
        gtime += 43200 - (stime % 86400);
        gtm = *gmtime(&gtime);

        if (gtm.tm_mday < ltm.tm_mday) gtime +=86400;
        if (gtm.tm_mday > ltm.tm_mday) gtime -=86400;
       
        /* Iterate, just in case of a day shift */
        stime = sunParams(gtime, &junk, &junk, Context->mark1.city);

        gtime += 43200 - (stime % 86400);

        /* get day length at that time and location*/
        duration = dayLength(gtime, Context->mark1.city->lat);
  
        /* compute sunrise and sunset in legal time */
        sr = gtime - (time_t)(0.5*duration);
        ss = gtime + (time_t)(0.5*duration);
        dl = ss-sr;
        Context->mark1.full = 1;
        if (dl<=0) {dl = 0; Context->mark1.full = 0;}
        if (dl>86380) {dl=86400; Context->mark1.full = 0;}

        Context->mark1.dl = *gmtime(&dl);
        Context->mark1.dl.tm_hour += (Context->mark1.dl.tm_mday-1) * 24;

        setTZ(Context->mark1.city);
        Context->mark1.sr = *localtime(&sr);
        Context->mark1.ss = *localtime(&ss);
}
