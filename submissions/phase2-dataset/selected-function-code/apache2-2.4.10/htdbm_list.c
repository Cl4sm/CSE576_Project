static apr_status_t htdbm_list(htdbm_t *htdbm)
{
    apr_status_t rv;
    apr_datum_t key, val;
    char *cmnt;
    int i = 0;

    rv = apr_dbm_firstkey(htdbm->dbm, &key);
    if (rv != APR_SUCCESS) {
        fprintf(stderr, "Empty database -- %s\n", htdbm->filename);
        return APR_ENOENT;
    }
    fprintf(stderr, "Dumping records from database -- %s\n", htdbm->filename);
    fprintf(stderr, "    %-32s Comment\n", "Username");
    while (key.dptr != NULL) {
        rv = apr_dbm_fetch(htdbm->dbm, key, &val);
        if (rv != APR_SUCCESS) {
            fprintf(stderr, "Failed getting data from %s\n", htdbm->filename);
            return APR_EGENERAL;
        }
        /* Note: we don't store \0-terminators on our dbm data */
        fprintf(stderr, "    %-32.*s", (int)key.dsize, key.dptr);
        cmnt = memchr(val.dptr, ':', val.dsize);
        if (cmnt)
            fprintf(stderr, " %.*s", (int)(val.dptr+val.dsize - (cmnt+1)), cmnt + 1);
        fprintf(stderr, "\n");
        rv = apr_dbm_nextkey(htdbm->dbm, &key);
        if (rv != APR_SUCCESS)
            fprintf(stderr, "Failed getting NextKey\n");
        ++i;
    }

    fprintf(stderr, "Total #records : %d\n", i);
    return APR_SUCCESS;
}