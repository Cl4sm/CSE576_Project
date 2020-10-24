sunParams(gtime, sunlong, sundec, city)
time_t gtime;
double *sunlong;
double *sundec;
City *city;
{
        struct tm               ctp, stp;
        time_t                  stime;
        double                  jt, gt;
        double                  sunra, sunrv, junk;
        long                    diff;

        ctp = *gmtime(&gtime);
        jt = jtime(&ctp);
        sunpos(jt, False, &sunra, sundec, &sunrv, &junk);
        gt = gmst(jt);
        *sunlong = fixangle(180.0 + (sunra - (gt * 15))); 
        
        if (city) {
           stime = (long) ((city->lon - *sunlong) * 240.0);
           stp = *gmtime(&stime);
           diff = stp.tm_sec-ctp.tm_sec
                  +60*(stp.tm_min-ctp.tm_min)+3600*(stp.tm_hour-ctp.tm_hour);
           if (city->lon>0.0) while(diff<-40000) diff += 86400;
           if (city->lon<0.0) while(diff>40000) diff -= 86400;
           stime = gtime+diff;
        } else
           stime = 0;
        return(stime);
}
