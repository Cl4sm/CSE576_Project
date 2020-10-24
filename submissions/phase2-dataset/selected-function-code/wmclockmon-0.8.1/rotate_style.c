static void rotate_style() {
    DIR           *dir;
    struct dirent *dir_ent;
    char          *next = NULL;
    Bool           is_next = False;

    if (! style_dir) return;
    if ((dir = opendir(style_dir)) == NULL) return;
    while ((dir_ent = readdir(dir)) != NULL) {
        if (strstr(dir_ent->d_name, ".mwcs") != NULL) {
            if (is_next || (! style_name)) {
                next = xstrdup(dir_ent->d_name);
                is_next = False;
                break;
            }
            if (style_name && (strstr(style_name, dir_ent->d_name) != NULL))
                is_next = True;
        }
    }
    closedir(dir);
    if (next) {
        FREE(style_name);
        style_name = xmalloc(strlen(next)+strlen(style_dir)+2);
        sprintf(style_name, "%s/%s", style_dir, next);
    } else {
        FREE(style_name);
        FREE(style.parts_s);
        FREE(style.letters_s);
        FREE(style.itime_s);
        FREE(style.btime_s);
        FREE(style.parts);
        FREE(style.letters);
        FREE(style.backl);
        FREE(style.backu);
        FREE(style.ibackl);
        FREE(style.ibacku);
        FREE(style.bbackl);
        FREE(style.bbacku);
        style = default_style;
    }
    FREE(next);
}