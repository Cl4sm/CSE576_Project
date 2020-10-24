static void load_stylepart(const char *filename, StyleDef *opts) {
    FILE *file;
    int   i = 0;

    if ((!filename) || (filename[0] == '-')) {
        for (i = 0 ; opts[i].key ; i++)
            set_style_default(opts[i]);
        return;
    }
    if ((file = fopen(filename, "r")) == NULL) return;
    while (!feof(file)) {
        char line[MAXSTRLEN + 1], *value;
        int  j, set = False;
        bzero(line, MAXSTRLEN + 1);
        fgets(line, MAXSTRLEN, file);
 
        i++;
        if (line[strlen(line) - 1] == '\n') line[strlen(line) - 1] = 0;
        if ((line[0] == '#') || (line[0] == 0)) continue;
        value = strchr(line, '=') + 1;
        while ((value[0] == ' ') && (value[0] != 0)) value++;
        if (value[0] == 0) continue;

        for (j = 0 ; opts[j].key ; j++) {
            int length = strlen(opts[j].key);
            if (strlen(line) <= length) continue;
            if (strncmp(line, opts[j].key, length) == 0) {
                set_style_value(opts[j], value);
                set = True;
                break;
            }
        }
        if (!set)
            fprintf(stderr, "Error in %s at line %d :\n[%s].\n",
                    filename, i, line);
    }
}