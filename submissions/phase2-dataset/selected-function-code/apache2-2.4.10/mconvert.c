static int mconvert(request_rec *r, union VALUETYPE *p, struct magic *m)
{
    char *rt;

    switch (m->type) {
    case BYTE:
    case SHORT:
    case LONG:
    case DATE:
        return 1;
    case STRING:
        /* Null terminate and eat the return */
        p->s[sizeof(p->s) - 1] = '\0';
        if ((rt = strchr(p->s, '\n')) != NULL)
            *rt = '\0';
        return 1;
    case BESHORT:
        p->h = (short) ((p->hs[0] << 8) | (p->hs[1]));
        return 1;
    case BELONG:
    case BEDATE:
        p->l = (long)
            ((p->hl[0] << 24) | (p->hl[1] << 16) | (p->hl[2] << 8) | (p->hl[3]));
        return 1;
    case LESHORT:
        p->h = (short) ((p->hs[1] << 8) | (p->hs[0]));
        return 1;
    case LELONG:
    case LEDATE:
        p->l = (long)
            ((p->hl[3] << 24) | (p->hl[2] << 16) | (p->hl[1] << 8) | (p->hl[0]));
        return 1;
    default:
        ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, r, APLOGNO(01538)
                    MODNAME ": invalid type %d in mconvert().", m->type);
        return 0;
    }
}