int
fts_close(FTS *fts)
{
    int save = 0;
    int rc;

    if(fts->ftsent.fts_path) {
        free(fts->ftsent.fts_path);
        fts->ftsent.fts_path = NULL;
    }

    if(fts->dname) {
        free(fts->dname);
        fts->dname = NULL;
    }

    rc = chdir(fts->cwd0);
    if(rc < 0)
        save = errno;

    while(fts->depth >= 0) {
        closedir(fts->dir[fts->depth]);
        fts->depth--;
    }

    free(fts->cwd0);
    if(fts->cwd) free(fts->cwd);
    free(fts);

    if(rc < 0) {
        errno = save;
        return -1;
    }
    return 0;
}