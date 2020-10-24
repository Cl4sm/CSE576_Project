static Bool raise_alarm() {
    if ((timeinfos->tm_hour == 0) &&
        (timeinfos->tm_min  == 0) &&
        (timeinfos->tm_sec  == 0)) {
        if (showcal) show_cal();
        if (calalrms) reload_alarms();
    }
    if (alarms) {
        Alarm  *alrm = alarms;
        char    thistime[MAXSTRLEN + 1];
        char    thisdate[MAXSTRLEN + 1];

        strftime(thistime, MAXSTRLEN, "%H:%M", timeinfos);
        strftime(thisdate, MAXSTRLEN, "%u", timeinfos);
        while (alrm) {
            if (alrm->on && (strcmp(thistime, alrm->alarm_time) == 0)) {
                message = alrm->message;
                if (alrm->alarm_date) {
                    if (strcmp(thisdate, alrm->alarm_date) == 0)
                        return True;
                    else
                        message = NULL;
                } else {
                    return True;
                }
            }
            alrm = alrm->next;
        }
    }
    return False;
}