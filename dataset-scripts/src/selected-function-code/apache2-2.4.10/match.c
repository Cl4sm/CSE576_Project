static int match(request_rec *r, unsigned char *s, apr_size_t nbytes)
{
#if MIME_MAGIC_DEBUG
    int rule_counter = 0;
#endif
    int cont_level = 0;
    int need_separator = 0;
    union VALUETYPE p;
    magic_server_config_rec *conf = (magic_server_config_rec *)
                ap_get_module_config(r->server->module_config, &mime_magic_module);
    struct magic *m;

#if MIME_MAGIC_DEBUG
    ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, APLOGNO(01529)
                MODNAME ": match conf=%x file=%s m=%s m->next=%s last=%s",
                conf,
                conf->magicfile ? conf->magicfile : "NULL",
                conf->magic ? "set" : "NULL",
                (conf->magic && conf->magic->next) ? "set" : "NULL",
                conf->last ? "set" : "NULL");
#endif

#if MIME_MAGIC_DEBUG
    for (m = conf->magic; m; m = m->next) {
        if (apr_isprint((((unsigned long) m) >> 24) & 255) &&
            apr_isprint((((unsigned long) m) >> 16) & 255) &&
            apr_isprint((((unsigned long) m) >> 8) & 255) &&
            apr_isprint(((unsigned long) m) & 255)) {
            ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, APLOGNO(01530)
                        MODNAME ": match: POINTER CLOBBERED! "
                        "m=\"%c%c%c%c\"",
                        (((unsigned long) m) >> 24) & 255,
                        (((unsigned long) m) >> 16) & 255,
                        (((unsigned long) m) >> 8) & 255,
                        ((unsigned long) m) & 255);
            break;
        }
    }
#endif

    for (m = conf->magic; m; m = m->next) {
#if MIME_MAGIC_DEBUG
        rule_counter++;
        ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, APLOGNO(01531)
                    MODNAME ": line=%d desc=%s", m->lineno, m->desc);
#endif

        /* check if main entry matches */
        if (!mget(r, &p, s, m, nbytes) ||
            !mcheck(r, &p, m)) {
            struct magic *m_cont;

            /*
             * main entry didn't match, flush its continuations
             */
            if (!m->next || (m->next->cont_level == 0)) {
                continue;
            }

            m_cont = m->next;
            while (m_cont && (m_cont->cont_level != 0)) {
#if MIME_MAGIC_DEBUG
                rule_counter++;
                ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, APLOGNO(01532)
                        MODNAME ": line=%d mc=%x mc->next=%x cont=%d desc=%s",
                            m_cont->lineno, m_cont,
                            m_cont->next, m_cont->cont_level,
                            m_cont->desc);
#endif
                /*
                 * this trick allows us to keep *m in sync when the continue
                 * advances the pointer
                 */
                m = m_cont;
                m_cont = m_cont->next;
            }
            continue;
        }

        /* if we get here, the main entry rule was a match */
        /* this will be the last run through the loop */
#if MIME_MAGIC_DEBUG
        ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, APLOGNO(01533)
                    MODNAME ": rule matched, line=%d type=%d %s",
                    m->lineno, m->type,
                    (m->type == STRING) ? m->value.s : "");
#endif

        /* print the match */
        mprint(r, &p, m);

        /*
         * If we printed something, we'll need to print a blank before we
         * print something else.
         */
        if (m->desc[0])
            need_separator = 1;
        /* and any continuations that match */
        cont_level++;
        /*
         * while (m && m->next && m->next->cont_level != 0 && ( m = m->next
         * ))
         */
        m = m->next;
        while (m && (m->cont_level != 0)) {
#if MIME_MAGIC_DEBUG
            ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, APLOGNO(01534)
                        MODNAME ": match line=%d cont=%d type=%d %s",
                        m->lineno, m->cont_level, m->type,
                        (m->type == STRING) ? m->value.s : "");
#endif
            if (cont_level >= m->cont_level) {
                if (cont_level > m->cont_level) {
                    /*
                     * We're at the end of the level "cont_level"
                     * continuations.
                     */
                    cont_level = m->cont_level;
                }
                if (mget(r, &p, s, m, nbytes) &&
                    mcheck(r, &p, m)) {
                    /*
                     * This continuation matched. Print its message, with a
                     * blank before it if the previous item printed and this
                     * item isn't empty.
                     */
                    /* space if previous printed */
                    if (need_separator
                        && (m->nospflag == 0)
                        && (m->desc[0] != '\0')
                        ) {
                        (void) magic_rsl_putchar(r, ' ');
                        need_separator = 0;
                    }
                    mprint(r, &p, m);
                    if (m->desc[0])
                        need_separator = 1;

                    /*
                     * If we see any continuations at a higher level, process
                     * them.
                     */
                    cont_level++;
                }
            }

            /* move to next continuation record */
            m = m->next;
        }
#if MIME_MAGIC_DEBUG
        ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, APLOGNO(01535)
                    MODNAME ": matched after %d rules", rule_counter);
#endif
        return 1;  /* all through */
    }
#if MIME_MAGIC_DEBUG
    ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, APLOGNO(01536)
                MODNAME ": failed after %d rules", rule_counter);
#endif
    return 0;  /* no match at all */
}