static Bool load_cfgfile() {
    FILE *file;
    int  i = 0, ok = True;
    char line[MAXSTRLEN + 1];
    char *value;

    if ((file = fopen(config_file, "r")) == NULL) {
        if (strstr(config_file, "/"DEFAULT_CFGFILE) == NULL)
            printf("Unable to open configuration file \"%s\".\n", config_file);
        ok = False;
    }
    if (ok && (! filestat(config_file, &config_mtime, MTIME))) {
        fclose(file);
        ok = False;
    }
    if (ok) {
        if (alarms) free_alrm(&alarms);
        while (! feof(file)) {
            bzero(line, MAXSTRLEN + 1);
            fgets(line, MAXSTRLEN, file);
            i++;
            if (line[strlen(line) - 1] == '\n') line[strlen(line) - 1] = 0;
            if ((line[0] == '#') || (line[0] == 0)) continue;
            value = strchr(line, '=');
            if (! value) continue;
            value++;
            while ((value[0] != 0) && (value[0] == ' ')) value++;
            if (value[0] == 0) continue;

            if (strncmp(line, "Backlight", 9) == 0) {
                backlight = getbool(value) ? LIGHTON : LIGHTOFF;
            } else if (strncmp(line, "Color", 5) == 0) {
                SET_STRING(light_color, value);
            } else if (strncmp(line, "Alarm", 5) == 0) {
                alrm_add(&alarms, value);
            } else if (strncmp(line, "Command", 7) == 0) {
                SET_STRING(command, value);
            } else if (strncmp(line, "MessageCmd", 10) == 0) {
                SET_STRING(msgcmd, value);
            } else if (strncmp(line, "Blink", 5) == 0) {
                switch_authorized = getbool(value);
            } else if (strncmp(line, "H12", 3) == 0) {
                h12 = getbool(value);
            } else if (strncmp(line, "Locale", 6) == 0) {
                use_locale = getbool(value);
            } else if (strncmp(line, "StyleDir", 8) == 0) {
                SET_STRING(style_dir, value);
            } else if (strncmp(line, "Style", 5) == 0) {
                SET_STRING(style_name, value);
            } else if (strncmp(line, "TimeMode", 5) == 0) {
                time_mode = atoi(value);
            } else if (strncmp(line, "ShowCal", 7) == 0) {
                showcal = getbool(value);
            } else if (strncmp(line, "CalAlrms", 8) == 0) {
                calalrms = getbool(value);
            } else {
                printf("Error in %s at line %d :\n[%s].\n", config_file, i, line);
            }
        }
        if (calalrms) load_calalrms();
        fclose(file);
    } else if (calalrms && cal_alrms_chg()) {
        reload_alarms();
    }
    return ok;
}