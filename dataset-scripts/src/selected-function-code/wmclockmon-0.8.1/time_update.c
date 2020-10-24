static void time_update() {
    time_t     tnow;

    time(&tnow);
    timeinfos = localtime(&tnow);

    if (time_mode == INTERNET) {
        long localtmzone;
        swtime = timeinfos->tm_hour * 3600
               + timeinfos->tm_min * 60
               + timeinfos->tm_sec;
#ifdef BSDTIMEZONE
        localtmzone = timeinfos->tm_gmtoff;
#else
        localtmzone = timezone;
#endif
        swtime += localtmzone+3600;
        if (timeinfos->tm_isdst) swtime -= 3600;
        swtime *= 1000;
        swtime /= 86400;
        if (swtime >= 1000)
            swtime -= 1000;
        else
            if (swtime < 0) swtime += 1000;
    }
}