static void show_cal_file(int type) {
    FILE *file;
    char *data = NULL;
    char *tmp  = NULL;
    char *calend_file = get_calend_file(type);

    if ((file = fopen(calend_file, "r")) != NULL) {
        while (! feof(file)) {
            char line[MAXSTRLEN + 1];
            bzero(line, MAXSTRLEN + 1);
            fgets(line, MAXSTRLEN, file);
            if (line[0] != 0) {
                int len = data ? strlen(data) : 0;
                tmp = xmalloc(len + strlen(line) + 1);
                sprintf(tmp, "%s%s", data ? data : "", line);
                FREE(data);
                data = tmp;
                tmp = NULL;
            }
        }
        fclose(file);
        my_system(msgcmd, data);
        FREE(data);
    }
    free(calend_file);
}