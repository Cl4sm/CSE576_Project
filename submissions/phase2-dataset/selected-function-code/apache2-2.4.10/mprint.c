static void mprint(request_rec *r, union VALUETYPE *p, struct magic *m)
{
    char *pp;
    unsigned long v;
    char time_str[APR_CTIME_LEN];

    switch (m->type) {
    case BYTE:
        v = p->b;
        break;

    case SHORT:
    case BESHORT:
    case LESHORT:
        v = p->h;
        break;

    case LONG:
    case BELONG:
    case LELONG:
        v = p->l;
        break;

    case STRING:
        if (m->reln == '=') {
            (void) magic_rsl_printf(r, m->desc, m->value.s);
        }
        else {
            (void) magic_rsl_printf(r, m->desc, p->s);
        }
        return;

    case DATE:
    case BEDATE:
    case LEDATE:
        apr_ctime(time_str, apr_time_from_sec(*(time_t *)&p->l));
        pp = time_str;
        (void) magic_rsl_printf(r, m->desc, pp);
        return;
    default:
        ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, r, APLOGNO(01537)
                    MODNAME ": invalid m->type (%d) in mprint().",
                    m->type);
        return;
    }

    v = signextend(r->server, m, v) & m->mask;
    (void) magic_rsl_printf(r, m->desc, (unsigned long) v);
}