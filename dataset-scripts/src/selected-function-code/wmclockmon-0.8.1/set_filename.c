static char *set_filename(const char *file_name, const char *ext) {
    int  length = style_dir ? strlen(style_dir) + 1 : 0;
    char *filename = NULL;
    if (! file_name) return NULL;
    if ((file_name[0] == '.') || (file_name[0] == '/') ||
        (ext && (strstr(file_name, ext) != NULL))) { /* local file */
        char *e;
        filename = xstrdup(file_name);
        e = strrchr(filename, '/');
        if ((!style_dir) && (e != NULL)) {
            char *p = filename;
            int  i = 0, style_len = strlen(filename) - strlen(e);
            style_dir = xmalloc(style_len + 2);
            while (p != e) {
                style_dir[i++] = p[0];
                p++;
            }
            style_dir[i] = 0;
        }
    } else {
        length += strlen(file_name);
        length += ext ? strlen(ext) + 1 : 1;
        filename = xmalloc(length);
        sprintf(filename, "%s%s%s%s",
                style_dir ? style_dir : "",
                style_dir ? "/" : "",
                file_name,
                ext ? ext : "");
    }
    return filename;
}