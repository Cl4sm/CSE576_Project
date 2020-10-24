static int apprentice(server_rec *s, apr_pool_t *p)
{
    apr_file_t *f = NULL;
    apr_status_t result;
    char line[BUFSIZ + 1];
    int errs = 0;
    int lineno;
#if MIME_MAGIC_DEBUG
    int rule = 0;
    struct magic *m, *prevm;
#endif
    magic_server_config_rec *conf = (magic_server_config_rec *)
                    ap_get_module_config(s->module_config, &mime_magic_module);
    const char *fname = ap_server_root_relative(p, conf->magicfile);

    if (!fname) {
        ap_log_error(APLOG_MARK, APLOG_ERR, APR_EBADPATH, s, APLOGNO(01514)
                     MODNAME ": Invalid magic file path %s", conf->magicfile);
        return -1;
    }
    if ((result = apr_file_open(&f, fname, APR_READ | APR_BUFFERED,
                                APR_OS_DEFAULT, p)) != APR_SUCCESS) {
        ap_log_error(APLOG_MARK, APLOG_ERR, result, s, APLOGNO(01515)
                     MODNAME ": can't read magic file %s", fname);
        return -1;
    }

    /* set up the magic list (empty) */
    conf->magic = conf->last = NULL;

    /* parse it */
    for (lineno = 1; apr_file_gets(line, BUFSIZ, f) == APR_SUCCESS; lineno++) {
        int ws_offset;
        char *last = line + strlen(line) - 1; /* guaranteed that len >= 1 since an
                                               * "empty" line contains a '\n'
                                               */

        /* delete newline and any other trailing whitespace */
        while (last >= line
               && apr_isspace(*last)) {
            *last = '\0';
            --last;
        }

        /* skip leading whitespace */
        ws_offset = 0;
        while (line[ws_offset] && apr_isspace(line[ws_offset])) {
            ws_offset++;
        }

        /* skip blank lines */
        if (line[ws_offset] == 0) {
            continue;
        }

        /* comment, do not parse */
        if (line[ws_offset] == '#')
            continue;

#if MIME_MAGIC_DEBUG
        /* if we get here, we're going to use it so count it */
        rule++;
#endif

        /* parse it */
        if (parse(s, p, line + ws_offset, lineno) != 0)
            ++errs;
    }

    (void) apr_file_close(f);

#if MIME_MAGIC_DEBUG
    ap_log_error(APLOG_MARK, APLOG_DEBUG, 0, s, APLOGNO(01516)
                MODNAME ": apprentice conf=%x file=%s m=%s m->next=%s last=%s",
                conf,
                conf->magicfile ? conf->magicfile : "NULL",
                conf->magic ? "set" : "NULL",
                (conf->magic && conf->magic->next) ? "set" : "NULL",
                conf->last ? "set" : "NULL");
    ap_log_error(APLOG_MARK, APLOG_DEBUG, 0, s, APLOGNO(01517)
                MODNAME ": apprentice read %d lines, %d rules, %d errors",
                lineno, rule, errs);
#endif

#if MIME_MAGIC_DEBUG
    prevm = 0;
    ap_log_error(APLOG_MARK, APLOG_DEBUG, 0, s, APLOGNO(01518)
                MODNAME ": apprentice test");
    for (m = conf->magic; m; m = m->next) {
        if (apr_isprint((((unsigned long) m) >> 24) & 255) &&
            apr_isprint((((unsigned long) m) >> 16) & 255) &&
            apr_isprint((((unsigned long) m) >> 8) & 255) &&
            apr_isprint(((unsigned long) m) & 255)) {
            ap_log_error(APLOG_MARK, APLOG_DEBUG, 0, s, APLOGNO(01519)
                        MODNAME ": apprentice: POINTER CLOBBERED! "
                        "m=\"%c%c%c%c\" line=%d",
                        (((unsigned long) m) >> 24) & 255,
                        (((unsigned long) m) >> 16) & 255,
                        (((unsigned long) m) >> 8) & 255,
                        ((unsigned long) m) & 255,
                        prevm ? prevm->lineno : -1);
            break;
        }
        prevm = m;
    }
#endif

    return (errs ? -1 : 0);
}