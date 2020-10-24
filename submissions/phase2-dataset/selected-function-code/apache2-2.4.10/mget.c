static int mget(request_rec *r, union VALUETYPE *p, unsigned char *s,
                struct magic *m, apr_size_t nbytes)
{
    long offset = m->offset;

    if (offset + sizeof(union VALUETYPE) > nbytes)
                  return 0;

    memcpy(p, s + offset, sizeof(union VALUETYPE));

    if (!mconvert(r, p, m))
        return 0;

    if (m->flag & INDIR) {

        switch (m->in.type) {
        case BYTE:
            offset = p->b + m->in.offset;
            break;
        case SHORT:
            offset = p->h + m->in.offset;
            break;
        case LONG:
            offset = p->l + m->in.offset;
            break;
        }

        if (offset + sizeof(union VALUETYPE) > nbytes)
                      return 0;

        memcpy(p, s + offset, sizeof(union VALUETYPE));

        if (!mconvert(r, p, m))
            return 0;
    }
    return 1;
}