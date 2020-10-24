setTZ(cptr)
City    *cptr;
{
#ifndef _OS_LINUX_
        char buf[80];
#endif

        if (cptr && cptr->tz) {
#ifdef _OS_LINUX_
           setenv("TZ", cptr->tz, 1);
#else
           sprintf(buf, "TZ=%s", cptr->tz);
#ifdef _OS_HPUX_
           putenv(strdup(buf));
#else
           putenv(buf);
#endif
#endif
           } 
        else
#ifdef _OS_LINUX_
           unsetenv("TZ");
#else
           {
           /* This is supposed to reset timezone to default localzone */
           strcpy(buf, "TZ");
           /* Another option would be to set LOCALZONE adequately and put:
           strcpy(buf, "TZ="LOCALZONE); */
#ifdef _OS_HPUX_
           putenv(strdup(buf));
#else
           putenv(buf);
#endif
           }
#endif
        tzset();
}
