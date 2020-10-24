static void draw_timedigit() {
    int hour = timeinfos->tm_hour, dx = 0, dy = 0;

    if (backlight == LIGHTON) {
        dx = style.sdigitw * 10; /* AM/PM/ALRM parts positions */
        dy = style.bdigith; /* colon seconds */
    }

    if (h12) {
        if (hour == 0)
            hour = 12;
        else
            hour = (hour > 12) ? hour - 12 : hour;
    }

    if (style.hbig)
        draw_bigdigit(hour, style.hposx, style.hposy);
    else
        draw_smalldigit(hour, style.hposx, style.hposy);

    if (style.mbig)
        draw_bigdigit(timeinfos->tm_min, style.mposx, style.mposy);
    else
        draw_smalldigit(timeinfos->tm_min, style.mposx, style.mposy);

    if (style.csec) {
        if (timeinfos->tm_sec % 2 == 1) {
            if (style.sbig) {
                dockapp_copyarea(parts, pixmap, 4, dy, 2, 2, style.sposx, style.sposy);
                dockapp_copyarea(parts, pixmap, 4, dy, 2, 2, style.sposx, style.sposy + 9);
            } else {
                dockapp_copyarea(parts, pixmap, 4, dy, 1, 1, style.sposx, style.sposy);
                dockapp_copyarea(parts, pixmap, 4, dy, 1, 1, style.sposx, style.sposy + 4);
            }
        }
    } else {
        if (style.sbig)
            draw_bigdigit(timeinfos->tm_sec, style.sposx, style.sposy);
        else
            draw_smalldigit(timeinfos->tm_sec, style.sposx, style.sposy);
    }

    if (h12) {
        /* Some say pm is when h>12 or h==0 but others (and my watch) told me
         * that the good way to handle am/pm is what is below
         */
        if (timeinfos->tm_hour >= 12) /* PM */
            dockapp_copyarea(parts, pixmap, 36 + dx, 49, 12, 7, style.pposx, style.pposy);
        else
            dockapp_copyarea(parts, pixmap, 23 + dx, 49, 12, 7, style.aposx, style.aposy);
    }
    if (alarms_on(alarms))
        dockapp_copyarea(parts, pixmap, dx, 49, 22, 7, style.lposx, style.lposy);
}