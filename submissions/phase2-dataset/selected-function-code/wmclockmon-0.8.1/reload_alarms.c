static void reload_alarms() {
    FILE *file;
    int  i = 0;
    char line[MAXSTRLEN + 1];
    char *value;

    if (alarms) free_alrm(&alarms);
    if ((file = fopen(config_file, "r")) == NULL) {
        if (strstr(config_file, "/"DEFAULT_CFGFILE) == NULL)
            printf("Unable to open configuration file \"%s\".\n", config_file);
    } else {
        while (! feof(file)) {
            bzero(line, MAXSTRLEN + 1);
            fgets(line, MAXSTRLEN, file);
            i++;
            if (line[strlen(line) - 1] == '\n') line[strlen(line) - 1] = 0;
            if ((line[0] == '#') || (line[0] == 0)) continue;
            value = strchr (line, '=') + 1;
            while ((value[0] == ' ') && (value[0] != 0)) value++;
            if (value[0] == 0) continue;

            if (strncmp(line, "Alarm", 5) == 0) alrm_add(&alarms, value);
        }
    }
    if (calalrms) load_calalrms();
}