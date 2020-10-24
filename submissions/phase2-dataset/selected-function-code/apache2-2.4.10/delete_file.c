static void delete_file(char *path, char *basename, apr_off_t *nodes,
        apr_pool_t *pool)
{
    char *nextpath;
    apr_pool_t *p;

    /* temp pool, otherwise lots of memory could be allocated */
    apr_pool_create(&p, pool);
    nextpath = apr_pstrcat(p, path, "/", basename, NULL);

    if (dryrun) {
        apr_finfo_t finfo;
        if (!apr_stat(&finfo, nextpath, APR_FINFO_NLINK, p)) {
            (*nodes)--;
        }
    }
    else if (!apr_file_remove(nextpath, p)) {
        (*nodes)--;
    }

    apr_pool_destroy(p);

    if (benice) {
        if (++delcount >= DELETE_NICE) {
            apr_sleep(NICE_DELAY);
            delcount = 0;
        }
    }

    delete_parent(path, basename, nodes, pool);

}