void
expireDiskObjects()
{
    int rc;
    char *fts_argv[2];
    FTS *fts;
    FTSENT *fe;
    int files = 0, considered = 0, unlinked = 0, truncated = 0;
    int dirs = 0, rmdirs = 0;
    long left = 0, total = 0;

    if(diskCacheRoot == NULL || 
       diskCacheRoot->length <= 0 || diskCacheRoot->string[0] != '/')
        return;

    fts_argv[0] = diskCacheRoot->string;
    fts_argv[1] = NULL;
    fts = fts_open(fts_argv, FTS_LOGICAL, NULL);
    if(fts == NULL) {
        do_log_error(L_ERROR, errno, "Couldn't fts_open disk cache");
    } else {
        while(1) {
            gettimeofday(&current_time, NULL);

            fe = fts_read(fts);
            if(!fe) break;

            if(fe->fts_info == FTS_D)
                continue;

            if(fe->fts_info == FTS_DP || fe->fts_info == FTS_DC ||
               fe->fts_info == FTS_DNR) {
                if(fe->fts_accpath[0] == '/' &&
                   strlen(fe->fts_accpath) <= diskCacheRoot->length)
                    continue;
                dirs++;
                rc = rmdir(fe->fts_accpath);
                if(rc >= 0)
                    rmdirs++;
                else if(errno != ENOTEMPTY && errno != EEXIST)
                    do_log_error(L_ERROR, errno,
                                 "Couldn't remove directory %s",
                                 scrub(fe->fts_accpath));
                continue;
            } else if(fe->fts_info == FTS_NS) {
                do_log_error(L_ERROR, fe->fts_errno, "Couldn't stat file %s",
                             scrub(fe->fts_accpath));
                continue;
            } else if(fe->fts_info == FTS_ERR) {
                do_log_error(L_ERROR, fe->fts_errno,
                             "Couldn't fts_read disk cache");
                break;
            }

            if(!S_ISREG(fe->fts_statp->st_mode)) {
                do_log(L_ERROR, "Unexpected file %s type 0%o.\n", 
                       fe->fts_accpath, (unsigned int)fe->fts_statp->st_mode);
                continue;
            }

            files++;
            left += expireFile(fe->fts_accpath, fe->fts_statp,
                               &considered, &unlinked, &truncated);
            total += fe->fts_statp->st_size;
        }
        fts_close(fts);
    }

    printf("Disk cache purged.\n");
    printf("%d files, %d considered, %d removed, %d truncated "
           "(%ldkB -> %ldkB).\n",
           files, considered, unlinked, truncated, total/1024, left/1024);
    printf("%d directories, %d removed.\n", dirs, rmdirs);
    return;
}