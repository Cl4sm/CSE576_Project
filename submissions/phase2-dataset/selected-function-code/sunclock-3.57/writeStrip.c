void
writeStrip(Context)
struct Sundata * Context;
{
        register struct tm      ltp;
        register struct tm      gtp;
        register struct tm      stp;
        time_t                  gtime;
        time_t                  stime;
        int                     i, l;
        char            s[128];
        char            slat[20], slon[20], slatp[20], slonp[20];
        double          dist;
#ifdef NEW_CTIME
        struct timeb            tp;

        if (ftime(&tp) == -1) {
                fprintf(stderr, "%s: ftime failed: ", ProgName);
                perror("");
                exit(1);
        }
#endif

	if (!Context->flags.mapped) return;

        time(&Context->time);
        gtime = Context->time + Context->jump;

        if (!Context->wintype) {
                char num[80];
                int hour; 
                char ampm;

                setTZ(NULL);
                ltp = *localtime(&gtime);
                gtp = *gmtime(&gtime);	   
	   
	        hour = ltp.tm_hour;
	        if (hour<12)
	           ampm = 'A';
	        else
	           ampm = 'P';
                if (hour > 12)
                   hour -= 12;
                l = strlen(DateFormat[Context->flags.clock_mode]);
                *s = '\0';
                for (i=0; i<l; i++) {
                   char c = DateFormat[Context->flags.clock_mode][i];
                   if (c != '%') { 
                      num[0] = c;
                      num[1] = '\0'; 
                   }
                   if (c == '%' && i<l-1) {
                      ++i; 
                      c = DateFormat[Context->flags.clock_mode][i];
                      switch(c) {
                        case 'G': sprintf(num, "%02d", gtp.tm_hour); break;			 
                        case 'H': sprintf(num, "%02d", ltp.tm_hour); break;
                        case 'M': sprintf(num, "%02d", ltp.tm_min); break;
                        case 'N': sprintf(num, "%02d", gtp.tm_min); break;			 
                        case 'P': num[0]=ampm; num[1]='\0'; break;
                        case 'S': sprintf(num, "%02d", ltp.tm_sec); break;
#ifdef NEW_CTIME
                        case 'Z': strcpy(num, ltp.tm_zone); break;
#else
                        case 'Z': strcpy(num, tzname[ltp.tm_isdst]); break;
#endif
                        case 'a': strcpy(num, Day_name[ltp.tm_wday]); break;
                        case 'd': sprintf(num, "%02d", ltp.tm_mday); break;
                        case 'h': sprintf(num, "%02d", hour); break;			 
                        case 'j': sprintf(num, "%02d", 1+ltp.tm_yday); break;
                        case 'b': strcpy(num, Month_name[ltp.tm_mon]); break;
                        case 'm': sprintf(num, "%02d", 1+ltp.tm_mon); break;
                        case 't': {
                           int w = ltp.tm_year+1900;
                           if (w % 4==0 && (w % 100!=0 || w % 400 == 0))
                             w = 366;
                           else
                             w = 365;
                           sprintf(num, "%d", w);
                           break;
                           }
                        case 'y': sprintf(num, "%02d", ltp.tm_year%100); break;
                        case 'Y': sprintf(num, "%d", ltp.tm_year+1900); break;
                        case 'U': {
                           struct tm ftm;
                           time_t ftime;
                           int w;
                           /*
                            * define weeknumber
                            * week #1 = week with the first thursday
                            */
                           /* set reference date to 1st of january, 12:00:00 */
                           (void) memset(&ftm, 0, sizeof(struct tm));
                           ftm.tm_isdst = -1;
                           ftm.tm_mon = 0;
                           ftm.tm_mday = 1;
                           ftm.tm_year = ltp.tm_year;
                           ftm.tm_hour = 12;
                           ftm.tm_min = 0;
                           ftm.tm_sec = 0;
                           ftime = mktime(&ftm);
                           ftm = *localtime(&ftime);
                           /* get first sunday (start of week) */
                           if (ftm.tm_wday < 5)
                              w = 1 - ftm.tm_wday;
                           else
                              w = 8 - ftm.tm_wday;
                           /* get weeknumber */
                           sprintf(num, "%02d", 
                                ((ltp.tm_yday+1-ltp.tm_wday-w)/7)+1);
                           break; 
                           }
                        case '_': c = ' ';
                        default: num[0] = c; num[1] = '\0'; break;
                      }
                   }
                   strcat(s, num);
                }
                l = strlen(s);
                if (l<72) {
                  for (i=l; i<72; i++) s[i] = ' ';
                  s[72] = '\0';
                  l = 72;
                }
                drawTextStrip(Context, s, l);
		return;
        }

        switch(Context->flags.map_mode) {

        case LEGALTIME:
           gtp = *gmtime(&gtime);
           setTZ(Context->mark1.city);
           ltp = *localtime(&gtime);
           sprintf(s,
                " %s %02d:%02d:%02d %s %s %02d %s %04d    %s %02d:%02d:%02d UTC %s %02d %s %04d",
                Label[L_LEGALTIME], ltp.tm_hour, ltp.tm_min,
                ltp.tm_sec,
#ifdef NEW_CTIME
                ltp.tm_zone,
#else
                tzname[ltp.tm_isdst],
#endif
                Day_name[ltp.tm_wday], ltp.tm_mday,
                Month_name[ltp.tm_mon], 1900 + ltp.tm_year ,
                Label[L_GMTTIME],
                gtp.tm_hour, gtp.tm_min,
                gtp.tm_sec, Day_name[gtp.tm_wday], gtp.tm_mday,
                Month_name[gtp.tm_mon], 1900 + gtp.tm_year);
           break;

        case COORDINATES:
           setTZ(Context->mark1.city);
           ltp = *localtime(&gtime);
           if (ltp.tm_mday != Context->local_day) 
              setDayParams(Context);
           Context->local_day = ltp.tm_mday;
           if ((Context->mark1.city) && Context->mark1.full)
           sprintf(s,
                " %s (%s,%s)  %02d:%02d:%02d %s %s %02d %s %04d   %s %02d:%02d:%02d   %s %02d:%02d:%02d",
                Context->mark1.city->name, 
                num2str(Context->mark1.city->lat, slat, Context->flags.dms), 
                num2str(Context->mark1.city->lon, slon, Context->flags.dms),
                ltp.tm_hour, ltp.tm_min, ltp.tm_sec,
#ifdef NEW_CTIME
                ltp.tm_zone,
#else
                tzname[ltp.tm_isdst],
#endif
                Day_name[ltp.tm_wday], ltp.tm_mday,
                Month_name[ltp.tm_mon], 1900 + ltp.tm_year ,
                Label[L_SUNRISE],
                Context->mark1.sr.tm_hour, Context->mark1.sr.tm_min, Context->mark1.sr.tm_sec,
                Label[L_SUNSET], 
                Context->mark1.ss.tm_hour, Context->mark1.ss.tm_min, Context->mark1.ss.tm_sec);
                else
           if ((Context->mark1.city) && !Context->mark1.full)
           sprintf(s,
                " %s (%s,%s)  %02d:%02d:%02d %s %s %02d %s %04d   %s %02d:%02d:%02d",
                Context->mark1.city->name, 
                num2str(Context->mark1.city->lat, slat, Context->flags.dms), 
                num2str(Context->mark1.city->lon, slon, Context->flags.dms),
                ltp.tm_hour, ltp.tm_min, ltp.tm_sec,
#ifdef NEW_CTIME
                ltp.tm_zone,
#else
                tzname[ltp.tm_isdst],
#endif
                Day_name[ltp.tm_wday], ltp.tm_mday,
                Month_name[ltp.tm_mon], 1900 + ltp.tm_year ,
                Label[L_DAYLENGTH],
                Context->mark1.dl.tm_hour, Context->mark1.dl.tm_min, Context->mark1.dl.tm_sec);
                else
                  sprintf(s," %s", Label[L_CLICKCITY]);
                break;

        case SOLARTIME:
           if (Context->mark1.city) {
             double junk;
             stime = sunParams(gtime, &junk, &junk, Context->mark1.city);
             stp = *gmtime(&stime);
             if (stp.tm_mday != Context->solar_day) 
                setDayParams(Context);
             Context->solar_day = stp.tm_mday;
             sprintf(s, " %s (%s,%s)  %s %02d:%02d:%02d   %s %02d %s %04d   %s %02d:%02d:%02d", 
                  Context->mark1.city->name, 
                  num2str(Context->mark1.city->lat,slat, Context->flags.dms), 
                  num2str(Context->mark1.city->lon,slon, Context->flags.dms),
                  Label[L_SOLARTIME],
                  stp.tm_hour, stp.tm_min, stp.tm_sec,
                  Day_name[stp.tm_wday], stp.tm_mday,
                  Month_name[stp.tm_mon], 1900 + stp.tm_year,
                  Label[L_DAYLENGTH], 
                  Context->mark1.dl.tm_hour, Context->mark1.dl.tm_min, Context->mark1.dl.tm_sec);
           } else
                  sprintf(s," %s", Label[L_CLICKLOC]);
           break;

        case DISTANCES:
           if(Context->mark1.city && Context->mark2.city) {
             dist = sin(torad(Context->mark1.city->lat)) * sin(torad(Context->mark2.city->lat))
                    + cos(torad(Context->mark1.city->lat)) * cos(torad(Context->mark2.city->lat))
                           * cos(torad(Context->mark1.city->lon-Context->mark2.city->lon));
             if (dist >= 1.0) 
                dist = 0.0;
             else
             if (dist <= -1.0) 
                dist = M_PI;
             else
                dist = acos(dist);
             sprintf(s, " %s (%s,%s) --> %s (%s,%s)     "
                      "%d km  =  %d miles", 
               Context->mark2.city->name, 
               num2str(Context->mark2.city->lat,slatp, Context->flags.dms), 
               num2str(Context->mark2.city->lon, slonp, Context->flags.dms),
               Context->mark1.city->name, 
               num2str(Context->mark1.city->lat, slat, Context->flags.dms), 
               num2str(Context->mark1.city->lon, slon, Context->flags.dms),
               (int)(EARTHRADIUS_KM*dist), (int)(EARTHRADIUS_ML*dist));
           } else
             sprintf(s, " %s", Label[L_CLICK2LOC]);
           break;

        case EXTENSION:
           showHours(Context);
	   return;

        default:
           break;
        }

        l = strlen(s);
        if (l<125) {
          for (i=l; i<125; i++) s[i] = ' ';
          s[125] = '\0';
          l = 125;
        }

      drawTextStrip(Context, s, l);
}
