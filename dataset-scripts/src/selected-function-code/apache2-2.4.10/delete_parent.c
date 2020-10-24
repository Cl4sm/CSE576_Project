static void delete_parent(const char *path, const char *basename,
        apr_off_t *nodes, apr_pool_t *pool)
{
    char *nextpath, *name;
    apr_pool_t *p;

    /* temp pool, otherwise lots of memory could be allocated */
    apr_pool_create(&p, pool);
    name = apr_pstrdup(p, basename);

    /* If asked to delete dirs, do so now. We don't care if it fails.
     * If it fails, it likely means there was something else there.
     */
    if (deldirs && !dryrun) {
        const char *vary;
        char *end = strrchr(name, '/');
        while (end) {
            *end = 0;

            /* remove the directory */
            nextpath = apr_pstrcat(p, path, "/", name, NULL);
            if (!apr_dir_remove(nextpath, p)) {
                (*nodes)--;

                /* vary directory found? */
                vary = strstr(name, CACHE_VDIR_SUFFIX);
                if (vary && !vary[sizeof(CACHE_VDIR_SUFFIX) - 1]) {
                    nextpath = apr_pstrcat(p, path, "/", apr_pstrndup(p, name, vary
                            - name), NULL);
                    if (!apr_file_remove(nextpath, p)) {
                        (*nodes)--;
                    }
                }

            }
            else {
                break;
            }
            end = strrchr(name, '/');
        }
    }

    apr_pool_destroy(p);

    if (benice) {
        if (++delcount >= DELETE_NICE) {
            apr_sleep(NICE_DELAY);
            delcount = 0;
        }
    }

}