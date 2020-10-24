static int mcheck(request_rec *r, union VALUETYPE *p, struct magic *m)
{
    register unsigned long l = m->value.l;
    register unsigned long v;
    int matched;

    if ((m->value.s[0] == 'x') && (m->value.s[1] == '\0')) {
        ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, r, APLOGNO(01539)
                    MODNAME ": BOINK");
        return 1;
    }

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
    case DATE:
    case BEDATE:
    case LEDATE:
        v = p->l;
        break;

    case STRING:
        l = 0;
        /*
         * What we want here is: v = strncmp(m->value.s, p->s, m->vallen);
         * but ignoring any nulls.  bcmp doesn't give -/+/0 and isn't
         * universally available anyway.
         */
        v = 0;
        {
            register unsigned char *a = (unsigned char *) m->value.s;
            register unsigned char *b = (unsigned char *) p->s;
            register int len = m->vallen;

            while (--len >= 0)
                if ((v = *b++ - *a++) != 0)
                    break;
        }
        break;
    default:
        /*  bogosity, pretend that it just wasn't a match */
        ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, r, APLOGNO(01540)
                    MODNAME ": invalid type %d in mcheck().", m->type);
        return 0;
    }

    v = signextend(r->server, m, v) & m->mask;

    switch (m->reln) {
    case 'x':
#if MIME_MAGIC_DEBUG
        ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, APLOGNO(01541)
                    "%lu == *any* = 1", v);
#endif
        matched = 1;
        break;

    case '!':
        matched = v != l;
#if MIME_MAGIC_DEBUG
        ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, APLOGNO(01542)
                    "%lu != %lu = %d", v, l, matched);
#endif
        break;

    case '=':
        matched = v == l;
#if MIME_MAGIC_DEBUG
        ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, APLOGNO(01543)
                    "%lu == %lu = %d", v, l, matched);
#endif
        break;

    case '>':
        if (m->flag & UNSIGNED) {
            matched = v > l;
#if MIME_MAGIC_DEBUG
            ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, APLOGNO(01544)
                        "%lu > %lu = %d", v, l, matched);
#endif
        }
        else {
            matched = (long) v > (long) l;
#if MIME_MAGIC_DEBUG
            ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, APLOGNO(01545)
                        "%ld > %ld = %d", v, l, matched);
#endif
        }
        break;

    case '<':
        if (m->flag & UNSIGNED) {
            matched = v < l;
#if MIME_MAGIC_DEBUG
            ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, APLOGNO(01546)
                        "%lu < %lu = %d", v, l, matched);
#endif
        }
        else {
            matched = (long) v < (long) l;
#if MIME_MAGIC_DEBUG
            ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, APLOGNO(01547)
                        "%ld < %ld = %d", v, l, matched);
#endif
        }
        break;

    case '&':
        matched = (v & l) == l;
#if MIME_MAGIC_DEBUG
        ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, APLOGNO(01548)
                    "((%lx & %lx) == %lx) = %d", v, l, l, matched);
#endif
        break;

    case '^':
        matched = (v & l) != l;
#if MIME_MAGIC_DEBUG
        ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, APLOGNO(01549)
                    "((%lx & %lx) != %lx) = %d", v, l, l, matched);
#endif
        break;

    default:
        /* bogosity, pretend it didn't match */
        matched = 0;
        ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, r, APLOGNO(01550)
                    MODNAME ": mcheck: can't happen: invalid relation %d.",
                    m->reln);
        break;
    }

    return matched;
}