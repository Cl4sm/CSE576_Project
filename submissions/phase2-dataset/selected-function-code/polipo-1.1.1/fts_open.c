FTS*
fts_open(char * const *path_argv, int options,
         int (*compar)(const FTSENT **, const FTSENT **))
{
    FTS *fts;
    DIR *dir;
    char *cwd;
    int rc;

    if(options != FTS_LOGICAL || compar != NULL || path_argv[1] != NULL) {
        errno = ENOSYS;
        return NULL;
    }

    dir = opendir(path_argv[0]);
    if(dir == NULL)
        return NULL;

    fts = calloc(sizeof(FTS), 1);
    if(fts == NULL) {
        int save = errno;
        closedir(dir);
        errno = save;
        return NULL;
    }

    cwd = getcwd_a();
    if(cwd == NULL) {
        int save = errno;
        free(fts);
        closedir(dir);
        errno = save;
        return NULL;
    }

    rc = change_to_dir(dir);
    if(rc < 0) {
        int save = errno;
        free(cwd);
        free(fts);
        closedir(dir);
        errno = save;
        return NULL;
    }

    fts->depth = 0;
    fts->dir[0] = dir;
    fts->cwd0 = cwd;
    fts->cwd = strdup(path_argv[0]);
    return fts;
}