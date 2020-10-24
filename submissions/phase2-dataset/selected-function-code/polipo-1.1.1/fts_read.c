FTSENT *
fts_read(FTS *fts)
{
    struct dirent *dirent;
    int rc;
    char *name;
    char buf[1024];

    if(fts->ftsent.fts_path) {
        free(fts->ftsent.fts_path);
        fts->ftsent.fts_path = NULL;
    }

    if(fts->dname) {
        free(fts->dname);
        fts->dname = NULL;
    }

 again:
    dirent = readdir(fts->dir[fts->depth]);
    if(dirent == NULL) {
        char *newcwd = NULL;
        closedir(fts->dir[fts->depth]);
        fts->dir[fts->depth] = NULL;
        fts->depth--;
        if(fts->depth >= 0) {
            fts->dname = basename_a(fts->cwd);
            if(fts->dname == NULL)
                goto error;
            newcwd = dirname_a(fts->cwd);
            if(newcwd == NULL)
                goto error;
        }
        if(fts->cwd) free(fts->cwd);
        fts->cwd = NULL;
        if(fts->depth < 0)
            return NULL;
        rc = change_to_dir(fts->dir[fts->depth]);
        if(rc < 0) {
            free(newcwd);
            goto error;
        }
        fts->cwd = newcwd;
        name = fts->dname;
        fts->ftsent.fts_info = FTS_DP;
        goto done;
    }

    name = dirent->d_name;

 again2:
    rc = stat(name, &fts->ftstat);
    if(rc < 0) {
        fts->ftsent.fts_info = FTS_NS;
        goto error2;
    }

    if(S_ISDIR(fts->ftstat.st_mode)) {
        char *newcwd;
        DIR *dir;

        if(strcmp(name, ".") == 0 || strcmp(name, "..") == 0)
            goto again;

        if(fts->depth >= FTS_MAX_DEPTH) {
            errno = ENFILE;
            goto error;
        }
        dir = opendir(dirent->d_name);
        if(dir == NULL) {
            if(errno == EACCES) {
                fts->ftsent.fts_info = FTS_DNR;
                goto error2;
            } else
                goto error;
        }
        newcwd = mkfilename(fts->cwd, dirent->d_name);
        rc = change_to_dir(dir);
        if(rc < 0) {
            free(newcwd);
            goto error;
        }
        free(fts->cwd);
        fts->cwd = newcwd;
        fts->ftsent.fts_info = FTS_D;
        fts->depth++;
        fts->dir[fts->depth] = dir;
        goto done;
    } else if(S_ISREG(fts->ftstat.st_mode)) {
        fts->ftsent.fts_info = FTS_F;
        goto done;
#ifdef S_ISLNK
    } else if(S_ISLNK(fts->ftstat.st_mode)) {
        int rc;
        rc = readlink(name, buf, 1024);
        if(rc < 0)
            goto error;
        if(rc >= 1023) {
            errno = ENAMETOOLONG;
            goto error;
        }
        buf[rc] = '\0';
        name = buf;
        if(access(buf, F_OK) >= 0)
            goto again2;
        fts->ftsent.fts_info = FTS_SLNONE;
        goto done;
#endif
    } else {
        fts->ftsent.fts_info = FTS_DEFAULT;
        goto done;
    }
    
 done:
    if(fts->cwd == NULL)
        fts->cwd = getcwd_a();
    if(fts->cwd == NULL) goto error;
    fts->ftsent.fts_path = mkfilename(fts->cwd, name);
    if(fts->ftsent.fts_path == NULL) goto error;
    fts->ftsent.fts_accpath = name;
    fts->ftsent.fts_statp = &fts->ftstat;
    return &fts->ftsent;

 error:
    fts->ftsent.fts_info = FTS_ERR;
 error2:
    fts->ftsent.fts_errno = errno;
    return &fts->ftsent;
}