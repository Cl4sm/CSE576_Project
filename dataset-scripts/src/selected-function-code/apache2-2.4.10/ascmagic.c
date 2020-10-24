static int ascmagic(request_rec *r, unsigned char *buf, apr_size_t nbytes)
{
    int has_escapes = 0;
    unsigned char *s;
    char nbuf[SMALL_HOWMANY + 1];  /* one extra for terminating '\0' */
    char *token;
    const struct names *p;
    int small_nbytes;
    char *strtok_state;

    /* these are easy, do them first */

    /*
     * for troff, look for . + letter + letter or .\"; this must be done to
     * disambiguate tar archives' ./file and other trash from real troff
     * input.
     */
    if (*buf == '.') {
        unsigned char *tp = buf + 1;

        while (apr_isspace(*tp))
            ++tp;  /* skip leading whitespace */
        if ((apr_isalnum(*tp) || *tp == '\\') &&
             (apr_isalnum(*(tp + 1)) || *tp == '"')) {
            magic_rsl_puts(r, "application/x-troff");
            return 1;
        }
    }
    if ((*buf == 'c' || *buf == 'C') && apr_isspace(*(buf + 1))) {
        /* Fortran */
        magic_rsl_puts(r, "text/plain");
        return 1;
    }

    /* look for tokens from names.h - this is expensive!, so we'll limit
     * ourselves to only SMALL_HOWMANY bytes */
    small_nbytes = (nbytes > SMALL_HOWMANY) ? SMALL_HOWMANY : nbytes;
    /* make a copy of the buffer here because apr_strtok() will destroy it */
    s = (unsigned char *) memcpy(nbuf, buf, small_nbytes);
    s[small_nbytes] = '\0';
    has_escapes = (memchr(s, '\033', small_nbytes) != NULL);
    while ((token = apr_strtok((char *) s, " \t\n\r\f", &strtok_state)) != NULL) {
        s = NULL;  /* make apr_strtok() keep on tokin' */
        for (p = names; p < names + NNAMES; p++) {
            if (STREQ(p->name, token)) {
                magic_rsl_puts(r, types[p->type]);
                if (has_escapes)
                    magic_rsl_puts(r, " (with escape sequences)");
                return 1;
            }
        }
    }

    switch (is_tar(buf, nbytes)) {
    case 1:
        /* V7 tar archive */
        magic_rsl_puts(r, "application/x-tar");
        return 1;
    case 2:
        /* POSIX tar archive */
        magic_rsl_puts(r, "application/x-tar");
        return 1;
    }

    /* all else fails, but it is ascii... */
    return 0;
}