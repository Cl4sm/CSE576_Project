static int process_dir(char *path, apr_pool_t *pool, apr_off_t *nodes)
{
    apr_dir_t *dir;
    apr_pool_t *p;
    apr_hash_t *h;
    apr_hash_index_t *i;
    apr_file_t *fd;
    apr_status_t status;
    apr_finfo_t info;
    apr_size_t len;
    apr_time_t current, deviation;
    char *nextpath, *base, *ext;
    APR_RING_ENTRY(_direntry) anchor;
    DIRENTRY *d, *t, *n;
    ENTRY *e;
    int skip, retries;
    disk_cache_info_t disk_info;

    APR_RING_INIT(&anchor, _direntry, link);
    apr_pool_create(&p, pool);
    h = apr_hash_make(p);
    fd = NULL;
    deviation = MAXDEVIATION * APR_USEC_PER_SEC;

    if (apr_dir_open(&dir, path, p) != APR_SUCCESS) {
        return 1;
    }

    while (apr_dir_read(&info, 0, dir) == APR_SUCCESS && !interrupted) {
        if (!strcmp(info.name, ".") || !strcmp(info.name, "..")) {
            continue;
        }
        d = apr_pcalloc(p, sizeof(DIRENTRY));
        d->basename = apr_pstrcat(p, path, "/", info.name, NULL);
        APR_RING_INSERT_TAIL(&anchor, d, _direntry, link);
        (*nodes)++;
    }

    apr_dir_close(dir);

    if (interrupted) {
        return 1;
    }

    skip = baselen + 1;

    for (d = APR_RING_FIRST(&anchor);
         !interrupted && d != APR_RING_SENTINEL(&anchor, _direntry, link);
         d=n) {
        n = APR_RING_NEXT(d, link);
        base = strrchr(d->basename, '/');
        if (!base++) {
            base = d->basename;
        }
        ext = strchr(base, '.');

        /* there may be temporary files which may be gone before
         * processing, always skip these if not in realclean mode
         */
        if (!ext && !realclean) {
            if (!strncasecmp(base, AP_TEMPFILE_BASE, AP_TEMPFILE_BASELEN)
                && strlen(base) == AP_TEMPFILE_NAMELEN) {
                continue;
            }
        }

        /* this may look strange but apr_stat() may return an error which
         * is system dependent and there may be transient failures,
         * so just blindly retry for a short while
         */
        retries = STAT_ATTEMPTS;
        status = APR_SUCCESS;
        do {
            if (status != APR_SUCCESS) {
                apr_sleep(STAT_DELAY);
            }
            status = apr_stat(&info, d->basename, DIRINFO, p);
        } while (status != APR_SUCCESS && !interrupted && --retries);

        /* what may happen here is that apache did create a file which
         * we did detect but then does delete the file before we can
         * get file information, so if we don't get any file information
         * we will ignore the file in this case
         */
        if (status != APR_SUCCESS) {
            if (!realclean && !interrupted) {
                continue;
            }
            return 1;
        }

        if (info.filetype == APR_DIR) {
            if (process_dir(d->basename, pool, nodes)) {
                return 1;
            }
            continue;
        }

        if (info.filetype != APR_REG) {
            continue;
        }

        if (!ext) {
            if (!strncasecmp(base, AP_TEMPFILE_BASE, AP_TEMPFILE_BASELEN)
                && strlen(base) == AP_TEMPFILE_NAMELEN) {
                d->basename += skip;
                d->type = TEMP;
                d->dsize = info.size;
                apr_hash_set(h, d->basename, APR_HASH_KEY_STRING, d);
            }
            continue;
        }

        if (!strcasecmp(ext, CACHE_HEADER_SUFFIX)) {
            *ext = '\0';
            d->basename += skip;
            /* if a user manually creates a '.header' file */
            if (d->basename[0] == '\0') {
                continue;
            }
            t = apr_hash_get(h, d->basename, APR_HASH_KEY_STRING);
            if (t) {
                d = t;
            }
            d->type |= HEADER;
            d->htime = info.mtime;
            d->hsize = info.size;
            apr_hash_set(h, d->basename, APR_HASH_KEY_STRING, d);
            continue;
        }

        if (!strcasecmp(ext, CACHE_DATA_SUFFIX)) {
            *ext = '\0';
            d->basename += skip;
            /* if a user manually creates a '.data' file */
            if (d->basename[0] == '\0') {
                continue;
            }
            t = apr_hash_get(h, d->basename, APR_HASH_KEY_STRING);
            if (t) {
                d = t;
            }
            d->type |= DATA;
            d->dtime = info.mtime;
            d->dsize = info.size;
            apr_hash_set(h, d->basename, APR_HASH_KEY_STRING, d);
        }
    }

    if (interrupted) {
        return 1;
    }

    path[baselen] = '\0';

    for (i = apr_hash_first(p, h); i && !interrupted; i = apr_hash_next(i)) {
        void *hvalue;
        apr_uint32_t format;

        apr_hash_this(i, NULL, NULL, &hvalue);
        d = hvalue;

        switch(d->type) {
        case HEADERDATA:
            nextpath = apr_pstrcat(p, path, "/", d->basename,
                                   CACHE_HEADER_SUFFIX, NULL);
            if (apr_file_open(&fd, nextpath, APR_FOPEN_READ | APR_FOPEN_BINARY,
                              APR_OS_DEFAULT, p) == APR_SUCCESS) {
                len = sizeof(format);
                if (apr_file_read_full(fd, &format, len,
                                       &len) == APR_SUCCESS) {
                    if (format == DISK_FORMAT_VERSION) {
                        apr_off_t offset = 0;

                        apr_file_seek(fd, APR_SET, &offset);

                        len = sizeof(disk_cache_info_t);

                        if (apr_file_read_full(fd, &disk_info, len,
                                               &len) == APR_SUCCESS) {
                            apr_file_close(fd);
                            e = apr_palloc(pool, sizeof(ENTRY));
                            APR_RING_INSERT_TAIL(&root, e, _entry, link);
                            e->expire = disk_info.expire;
                            e->response_time = disk_info.response_time;
                            e->htime = d->htime;
                            e->dtime = d->dtime;
                            e->hsize = d->hsize;
                            e->dsize = d->dsize;
                            e->basename = apr_pstrdup(pool, d->basename);
                            if (!disk_info.has_body) {
                                delete_file(path, apr_pstrcat(p, path, "/",
                                        d->basename, CACHE_DATA_SUFFIX, NULL),
                                        nodes, p);
                            }
                            break;
                        }
                        else {
                            apr_file_close(fd);
                        }
                    }
                    else if (format == VARY_FORMAT_VERSION) {
                        apr_finfo_t finfo;

                        /* This must be a URL that added Vary headers later,
                         * so kill the orphaned .data file
                         */
                        apr_file_close(fd);

                        if (apr_stat(&finfo, apr_pstrcat(p, nextpath,
                                CACHE_VDIR_SUFFIX, NULL), APR_FINFO_TYPE, p)
                                || finfo.filetype != APR_DIR) {
                            delete_entry(path, d->basename, nodes, p);
                        }
                        else {
                            delete_file(path, apr_pstrcat(p, path, "/",
                                    d->basename, CACHE_DATA_SUFFIX, NULL),
                                    nodes, p);
                        }
                        break;
                    }
                    else {
                        /* We didn't recognise the format, kill the files */
                        apr_file_close(fd);
                        delete_entry(path, d->basename, nodes, p);
                        break;
                    }
                }
                else {
                    apr_file_close(fd);
                }

            }
            /* we have a somehow unreadable headers file which is associated
             * with a data file. this may be caused by apache currently
             * rewriting the headers file. thus we may delete the file set
             * either in realclean mode or if the headers file modification
             * timestamp is not within a specified positive or negative offset
             * to the current time.
             */
            current = apr_time_now();
            if (realclean || d->htime < current - deviation
                || d->htime > current + deviation) {
                delete_entry(path, d->basename, nodes, p);
                unsolicited += d->hsize;
                unsolicited += d->dsize;
            }
            break;

        /* single data and header files may be deleted either in realclean
         * mode or if their modification timestamp is not within a
         * specified positive or negative offset to the current time.
         * this handling is necessary due to possible race conditions
         * between apache and this process
         */
        case HEADER:
            current = apr_time_now();
            nextpath = apr_pstrcat(p, path, "/", d->basename,
                                   CACHE_HEADER_SUFFIX, NULL);
            if (apr_file_open(&fd, nextpath, APR_FOPEN_READ | APR_FOPEN_BINARY,
                              APR_OS_DEFAULT, p) == APR_SUCCESS) {
                len = sizeof(format);
                if (apr_file_read_full(fd, &format, len,
                                       &len) == APR_SUCCESS) {
                    if (format == VARY_FORMAT_VERSION) {
                        apr_time_t expires;

                        len = sizeof(expires);

                        if (apr_file_read_full(fd, &expires, len,
                                               &len) == APR_SUCCESS) {
                            apr_finfo_t finfo;

                            apr_file_close(fd);

                            if (apr_stat(&finfo, apr_pstrcat(p, nextpath,
                                    CACHE_VDIR_SUFFIX, NULL), APR_FINFO_TYPE, p)
                                    || finfo.filetype != APR_DIR) {
                                delete_entry(path, d->basename, nodes, p);
                            }
                            else if (expires < current) {
                                delete_entry(path, d->basename, nodes, p);
                            }

                            break;
                        }
                    }
                    else if (format == DISK_FORMAT_VERSION) {
                        apr_off_t offset = 0;

                        apr_file_seek(fd, APR_SET, &offset);

                        len = sizeof(disk_cache_info_t);

                        if (apr_file_read_full(fd, &disk_info, len,
                                               &len) == APR_SUCCESS) {
                            apr_file_close(fd);
                            e = apr_palloc(pool, sizeof(ENTRY));
                            APR_RING_INSERT_TAIL(&root, e, _entry, link);
                            e->expire = disk_info.expire;
                            e->response_time = disk_info.response_time;
                            e->htime = d->htime;
                            e->dtime = d->dtime;
                            e->hsize = d->hsize;
                            e->dsize = d->dsize;
                            e->basename = apr_pstrdup(pool, d->basename);
                            break;
                        }
                        else {
                            apr_file_close(fd);
                        }
                    }
                    else {
                        apr_file_close(fd);
                        delete_entry(path, d->basename, nodes, p);
                        break;
                    }
                }
                else {
                    apr_file_close(fd);
                }
            }

            if (realclean || d->htime < current - deviation
                || d->htime > current + deviation) {
                delete_entry(path, d->basename, nodes, p);
                unsolicited += d->hsize;
            }
            break;

        case DATA:
            current = apr_time_now();
            if (realclean || d->dtime < current - deviation
                || d->dtime > current + deviation) {
                delete_entry(path, d->basename, nodes, p);
                unsolicited += d->dsize;
            }
            break;

        /* temp files may only be deleted in realclean mode which
         * is asserted above if a tempfile is in the hash array
         */
        case TEMP:
            delete_file(path, d->basename, nodes, p);
            unsolicited += d->dsize;
            break;
        }
    }

    if (interrupted) {
        return 1;
    }

    apr_pool_destroy(p);

    if (benice) {
        apr_sleep(NICE_DELAY);
    }

    if (interrupted) {
        return 1;
    }

    return 0;
}