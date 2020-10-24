static apr_status_t find_directory(apr_pool_t *pool, const char *base,
        const char *rest)
{
    apr_status_t rv;
    apr_dir_t *dirp;
    apr_finfo_t dirent;
    int found = 0, files = 0;
    const char *header = apr_pstrcat(pool, rest, CACHE_HEADER_SUFFIX, NULL);
    const char *data = apr_pstrcat(pool, rest, CACHE_DATA_SUFFIX, NULL);
    const char *vdir = apr_pstrcat(pool, rest, CACHE_HEADER_SUFFIX,
            CACHE_VDIR_SUFFIX, NULL);
    const char *dirname = NULL;

    rv = apr_dir_open(&dirp, base, pool);
    if (rv != APR_SUCCESS) {
        apr_file_printf(errfile, "Could not open directory %s: %pm" APR_EOL_STR,
                base, &rv);
        return rv;
    }

    rv = APR_ENOENT;

    while (apr_dir_read(&dirent, APR_FINFO_DIRENT | APR_FINFO_TYPE, dirp)
            == APR_SUCCESS) {
        int len = strlen(dirent.name);
        int restlen = strlen(rest);
        if (dirent.filetype == APR_DIR && !strncmp(rest, dirent.name, len)) {
            dirname = apr_pstrcat(pool, base, "/", dirent.name, NULL);
            rv = find_directory(pool, dirname, rest + (len < restlen ? len
                    : restlen));
            if (APR_SUCCESS == rv) {
                found = 1;
            }
        }
        if (dirent.filetype == APR_DIR) {
            if (!strcmp(dirent.name, vdir)) {
                files = 1;
            }
        }
        if (dirent.filetype == APR_REG) {
            if (!strcmp(dirent.name, header) || !strcmp(dirent.name, data)) {
                files = 1;
            }
        }
    }

    apr_dir_close(dirp);

    if (files) {
        rv = APR_SUCCESS;
        if (!dryrun) {
            const char *remove;
            apr_status_t status;

            remove = apr_pstrcat(pool, base, "/", header, NULL);
            status = apr_file_remove(remove, pool);
            if (status != APR_SUCCESS && !APR_STATUS_IS_ENOENT(status)) {
                apr_file_printf(errfile, "Could not remove file %s: %pm" APR_EOL_STR,
                        remove, &status);
                rv = status;
            }

            remove = apr_pstrcat(pool, base, "/", data, NULL);
            status = apr_file_remove(remove, pool);
            if (status != APR_SUCCESS && !APR_STATUS_IS_ENOENT(status)) {
                apr_file_printf(errfile, "Could not remove file %s: %pm" APR_EOL_STR,
                        remove, &status);
                rv = status;
            }

            status = remove_directory(pool, apr_pstrcat(pool, base, "/", vdir, NULL));
            if (status != APR_SUCCESS && !APR_STATUS_IS_ENOENT(status)) {
                rv = status;
            }
        }
    }

    /* If asked to delete dirs, do so now. We don't care if it fails.
     * If it fails, it likely means there was something else there.
     */
    if (dirname && deldirs && !dryrun) {
        apr_dir_remove(dirname, pool);
    }

    if (found) {
        return APR_SUCCESS;
    }

    return rv;
}