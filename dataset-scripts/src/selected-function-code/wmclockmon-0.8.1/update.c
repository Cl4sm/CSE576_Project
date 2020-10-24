static void update() {
    static Light pre_backlight;
    static Bool  in_alarm_mode = False;

    /* check config_file modifications */
    if (load_cfgfile()) graphics_init();

    /* get current time */
    time_update();

    /* alarm mode */
    if (raise_alarm()) {
        if (!in_alarm_mode) {
            in_alarm_mode = True;
            my_system(command, NULL);
            if (message) my_system(msgcmd, message);
            pre_backlight = backlight;
        }
        if ( (switch_authorized) ||
             ( (switch_authorized) && (backlight != pre_backlight) ) ) {
            switch_light();
            return;
        }
    } else {
        if (in_alarm_mode) {
            in_alarm_mode = False;
            if (backlight != pre_backlight) {
                switch_light();
                return;
            }
        }
    }
    draw_dockapp();
}