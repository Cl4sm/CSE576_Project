static void
kanjiconvert(const char *operation)
{
    char buffer[100], *p;
    char *(*codeconv)(const char*);

    strncpy(buffer, operation,sizeof(buffer)-1);
    if (strchr(buffer, '-') == NULL) {
        strcat(buffer, "-" KANJICODE);
    }
    if (strncasecmp(buffer, "jis-euc", 7) == 0)
        codeconv = jis_to_euc;
    else if (strncasecmp(buffer, "jis-sjis", 8) == 0)
        codeconv = jis_to_sjis;
    else if (strncasecmp(buffer, "euc-jis", 7) == 0)
        codeconv = euc_to_jis;
    else if (strncasecmp(buffer, "euc-sjis", 8) == 0)
        codeconv = euc_to_sjis;
    else if (strncasecmp(buffer, "sjis-jis", 8) == 0)
        codeconv = sjis_to_jis;
    else if (strncasecmp(buffer, "sjis-euc", 8) == 0)
        codeconv = sjis_to_euc;
    else {
        // fprintf(stderr, "Don't know kanji conversion \"%s\"\n", buffer);
        // exit(1);
        codeconv = NULL;
    }
    while(fgets(buffer, 100, stdin) != NULL) {
        // If the string "XSHISEN_VERSION" is found, it should be replaced
        // with the XSHISEN_VERSION value.
        if ((p = strstr(buffer, "XSHISEN_VERSION")) != NULL) {
            int len1 = strlen(XSHISEN_VERSION);
            char *p2 = p + strlen("XSHISEN_VERSION");
            strncpy(p, XSHISEN_VERSION, len1);
            p += len1;
            memmove(p, p2, 100 - (p2 - buffer));
        }
        if (codeconv == NULL) {
            fputs(buffer, stdout);
        } else {
            fputs(codeconv(buffer), stdout);
        }
    }
}
