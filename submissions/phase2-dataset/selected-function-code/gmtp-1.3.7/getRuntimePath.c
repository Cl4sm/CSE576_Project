gchar *getRuntimePath(int argc, char *argv[]) {

    gchar *fullpath;
    gchar *filepath;
    gchar *foundpath = NULL;
    const char delimit[] = ";:";
    gchar *token;

    if (g_ascii_strcasecmp(PACKAGE_NAME, argv[0]) == 0) {
        // list each directory individually.
        fullpath = g_strdup(getenv("PATH"));
        token = strtok(fullpath, delimit);
        while ((token != NULL) && (foundpath == NULL)) {
            // Now test to see if we have it here...
            filepath = g_strdup(token);
            filepath = g_strconcat(filepath, "/", PACKAGE_NAME, NULL);
            if (access(filepath, F_OK) != -1) {
                foundpath = g_strdup(token);
            }
            token = strtok(NULL, delimit);
            g_free(filepath);
        }
    } else {
        // We were started with full file path.
        foundpath = g_strdup(dirname(argv[0]));
    }
    if (argc == 3) {
        // We have some other options, lets check for --datapath
        if (g_ascii_strcasecmp("--datapath", argv[1]) == 0) {
            // our first argument is --datapath, so set the path to argv[2];
            foundpath = g_strdup(argv[2]);
        }
    }
    return foundpath;
} // end getRuntimePath
