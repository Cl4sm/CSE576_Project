int
parseConfigFile(AtomPtr filename)
{
    char buf[512];
    int lineno;
    FILE *f;

    if(!filename || filename->length == 0)
        return 0;
    f = fopen(filename->string, "r");
    if(f == NULL) {
        do_log_error(L_ERROR, errno, "Couldn't open config file %s",
                     filename->string);
        return -1;
    }

    lineno = 1;
    while(1) {
        char *s;
        s = fgets(buf, 512, f);
        if(s == NULL) {
            fclose(f);
            return 1;
        }
        parseConfigLine(buf, filename->string, lineno, 0);
        lineno++;
    }
}