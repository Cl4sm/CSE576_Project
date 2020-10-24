static int magic_init(apr_pool_t *p, apr_pool_t *plog, apr_pool_t *ptemp, server_rec *main_server)
{
    int result;
    magic_server_config_rec *conf;
    magic_server_config_rec *main_conf;
    server_rec *s;
#if MIME_MAGIC_DEBUG
    struct magic *m, *prevm;
#endif /* MIME_MAGIC_DEBUG */

    main_conf = ap_get_module_config(main_server->module_config, &mime_magic_module);
    for (s = main_server; s; s = s->next) {
        conf = ap_get_module_config(s->module_config, &mime_magic_module);
        if (conf->magicfile == NULL && s != main_server) {
            /* inherits from the parent */
            *conf = *main_conf;
        }
        else if (conf->magicfile) {
            result = apprentice(s, p);
            if (result == -1)
                return OK;
#if MIME_MAGIC_DEBUG
            prevm = 0;
            ap_log_error(APLOG_MARK, APLOG_DEBUG, 0, s, APLOGNO(01558)
                        MODNAME ": magic_init 1 test");
            for (m = conf->magic; m; m = m->next) {
                if (apr_isprint((((unsigned long) m) >> 24) & 255) &&
                    apr_isprint((((unsigned long) m) >> 16) & 255) &&
                    apr_isprint((((unsigned long) m) >> 8) & 255) &&
                    apr_isprint(((unsigned long) m) & 255)) {
                    ap_log_error(APLOG_MARK, APLOG_DEBUG, 0, s, APLOGNO(01559)
                                MODNAME ": magic_init 1: POINTER CLOBBERED! "
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
        }
    }
    return OK;
}