static char *get_calend_file(int type) {
    char *Home     = robust_home();
    char *filename = xmalloc(
            strlen(Home) +
            strlen(DEFAULT_CONFIGDIR) +
            18);

    switch (type) {
        case 1:
            sprintf(filename, "%s/%s/%04d-%02d-%02d",
                    Home,
                    DEFAULT_CONFIGDIR,
                    timeinfos->tm_year + 1900,
                    timeinfos->tm_mon + 1,
                    timeinfos->tm_mday);
            break;
        case 2:
            sprintf(filename, "%s/%s/XXXX-%02d-%02d",
                    Home,
                    DEFAULT_CONFIGDIR,
                    timeinfos->tm_mon + 1,
                    timeinfos->tm_mday);
            break;
        case 3:
            sprintf(filename, "%s/%s/XXXX-XX-%02d",
                    Home,
                    DEFAULT_CONFIGDIR,
                    timeinfos->tm_mday);
            break;
        default: exit(1);
    }
    return filename;
}