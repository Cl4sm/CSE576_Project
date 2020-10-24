static int parse(server_rec *serv, apr_pool_t *p, char *l, int lineno)
{
    struct magic *m;
    char *t, *s;
    magic_server_config_rec *conf = (magic_server_config_rec *)
                    ap_get_module_config(serv->module_config, &mime_magic_module);

    /* allocate magic structure entry */
    m = (struct magic *) apr_pcalloc(p, sizeof(struct magic));

    /* append to linked list */
    m->next = NULL;
    if (!conf->magic || !conf->last) {
        conf->magic = conf->last = m;
    }
    else {
        conf->last->next = m;
        conf->last = m;
    }

    /* set values in magic structure */
    m->flag = 0;
    m->cont_level = 0;
    m->lineno = lineno;

    while (*l == '>') {
        ++l;  /* step over */
        m->cont_level++;
    }

    if (m->cont_level != 0 && *l == '(') {
        ++l;  /* step over */
        m->flag |= INDIR;
    }

    /* get offset, then skip over it */
    m->offset = (int) strtol(l, &t, 0);
    if (l == t) {
        ap_log_error(APLOG_MARK, APLOG_ERR, 0, serv, APLOGNO(01521)
                    MODNAME ": offset %s invalid", l);
    }
    l = t;

    if (m->flag & INDIR) {
        m->in.type = LONG;
        m->in.offset = 0;
        /*
         * read [.lbs][+-]nnnnn)
         */
        if (*l == '.') {
            switch (*++l) {
            case 'l':
                m->in.type = LONG;
                break;
            case 's':
                m->in.type = SHORT;
                break;
            case 'b':
                m->in.type = BYTE;
                break;
            default:
                ap_log_error(APLOG_MARK, APLOG_ERR, 0, serv, APLOGNO(01522)
                        MODNAME ": indirect offset type %c invalid", *l);
                break;
            }
            l++;
        }
        s = l;
        if (*l == '+' || *l == '-')
            l++;
        if (apr_isdigit((unsigned char) *l)) {
            m->in.offset = strtol(l, &t, 0);
            if (*s == '-')
                m->in.offset = -m->in.offset;
        }
        else
            t = l;
        if (*t++ != ')') {
            ap_log_error(APLOG_MARK, APLOG_ERR, 0, serv, APLOGNO(01523)
                        MODNAME ": missing ')' in indirect offset");
        }
        l = t;
    }


    while (apr_isdigit((unsigned char) *l))
        ++l;
    EATAB;

#define NBYTE           4
#define NSHORT          5
#define NLONG           4
#define NSTRING         6
#define NDATE           4
#define NBESHORT        7
#define NBELONG         6
#define NBEDATE         6
#define NLESHORT        7
#define NLELONG         6
#define NLEDATE         6

    if (*l == 'u') {
        ++l;
        m->flag |= UNSIGNED;
    }

    /* get type, skip it */
    if (strncmp(l, "byte", NBYTE) == 0) {
        m->type = BYTE;
        l += NBYTE;
    }
    else if (strncmp(l, "short", NSHORT) == 0) {
        m->type = SHORT;
        l += NSHORT;
    }
    else if (strncmp(l, "long", NLONG) == 0) {
        m->type = LONG;
        l += NLONG;
    }
    else if (strncmp(l, "string", NSTRING) == 0) {
        m->type = STRING;
        l += NSTRING;
    }
    else if (strncmp(l, "date", NDATE) == 0) {
        m->type = DATE;
        l += NDATE;
    }
    else if (strncmp(l, "beshort", NBESHORT) == 0) {
        m->type = BESHORT;
        l += NBESHORT;
    }
    else if (strncmp(l, "belong", NBELONG) == 0) {
        m->type = BELONG;
        l += NBELONG;
    }
    else if (strncmp(l, "bedate", NBEDATE) == 0) {
        m->type = BEDATE;
        l += NBEDATE;
    }
    else if (strncmp(l, "leshort", NLESHORT) == 0) {
        m->type = LESHORT;
        l += NLESHORT;
    }
    else if (strncmp(l, "lelong", NLELONG) == 0) {
        m->type = LELONG;
        l += NLELONG;
    }
    else if (strncmp(l, "ledate", NLEDATE) == 0) {
        m->type = LEDATE;
        l += NLEDATE;
    }
    else {
        ap_log_error(APLOG_MARK, APLOG_ERR, 0, serv, APLOGNO(01524)
                    MODNAME ": type %s invalid", l);
        return -1;
    }
    /* New-style anding: "0 byte&0x80 =0x80 dynamically linked" */
    if (*l == '&') {
        ++l;
        m->mask = signextend(serv, m, strtol(l, &l, 0));
    }
    else
        m->mask = ~0L;
    EATAB;

    switch (*l) {
    case '>':
    case '<':
        /* Old-style anding: "0 byte &0x80 dynamically linked" */
    case '&':
    case '^':
    case '=':
        m->reln = *l;
        ++l;
        break;
    case '!':
        if (m->type != STRING) {
            m->reln = *l;
            ++l;
            break;
        }
        /* FALL THROUGH */
    default:
        if (*l == 'x' && apr_isspace(l[1])) {
            m->reln = *l;
            ++l;
            goto GetDesc;  /* Bill The Cat */
        }
        m->reln = '=';
        break;
    }
    EATAB;

    if (getvalue(serv, m, &l))
        return -1;
    /*
     * now get last part - the description
     */
  GetDesc:
    EATAB;
    if (l[0] == '\b') {
        ++l;
        m->nospflag = 1;
    }
    else if ((l[0] == '\\') && (l[1] == 'b')) {
        ++l;
        ++l;
        m->nospflag = 1;
    }
    else
        m->nospflag = 0;
    apr_cpystrn(m->desc, l, sizeof(m->desc));

#if MIME_MAGIC_DEBUG
    ap_log_error(APLOG_MARK, APLOG_DEBUG, 0, serv, APLOGNO(01525)
                MODNAME ": parse line=%d m=%x next=%x cont=%d desc=%s",
                lineno, m, m->next, m->cont_level, m->desc);
#endif /* MIME_MAGIC_DEBUG */

    return 0;
}