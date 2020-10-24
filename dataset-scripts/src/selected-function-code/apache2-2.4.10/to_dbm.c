static apr_status_t to_dbm(apr_dbm_t *dbm, apr_file_t *fp, apr_pool_t *pool)
{
    apr_status_t rv = APR_SUCCESS;
    char line[REWRITE_MAX_TXT_MAP_LINE + 1]; /* +1 for \0 */
    apr_datum_t dbmkey;
    apr_datum_t dbmval;
    apr_pool_t* p;

    apr_pool_create(&p, pool);

    while (apr_file_gets(line, sizeof(line), fp) == APR_SUCCESS) {
        char *c, *value;

        if (*line == '#' || apr_isspace(*line)) {
            continue;
        }

        c = line;

        while (*c && !apr_isspace(*c)) {
            ++c;
        }

        if (!*c) {
            /* no value. solid line of data. */
            continue;
        }

        dbmkey.dptr = apr_pstrmemdup(p, line,  c - line);
        dbmkey.dsize = (c - line);

        while (apr_isspace(*c)) {
            ++c;
        }

        if (!*c) {
            apr_pool_clear(p);
            continue;
        }

        value = c;

        while (*c && !apr_isspace(*c)) {
            ++c;
        }

        dbmval.dptr = apr_pstrmemdup(p, value,  c - value);
        dbmval.dsize = (c - value);

        if (verbose) {
            apr_file_printf(errfile, "    '%s' -> '%s'" NL,
                            dbmkey.dptr, dbmval.dptr);
        }

        rv = apr_dbm_store(dbm, dbmkey, dbmval);

        apr_pool_clear(p);

        if (rv != APR_SUCCESS) {
            break;
        }
    }

    return rv;
}