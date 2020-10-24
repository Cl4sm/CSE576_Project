static char *getstr(server_rec *serv, register char *s, register char *p,
                    int plen, int *slen)
{
    char *origs = s, *origp = p;
    char *pmax = p + plen - 1;
    register int c;
    register int val;

    while ((c = *s++) != '\0') {
        if (apr_isspace(c))
            break;
        if (p >= pmax) {
            ap_log_error(APLOG_MARK, APLOG_ERR, 0, serv, APLOGNO(01526)
                        MODNAME ": string too long: %s", origs);
            break;
        }
        if (c == '\\') {
            switch (c = *s++) {

            case '\0':
                goto out;

            default:
                *p++ = (char) c;
                break;

            case 'n':
                *p++ = '\n';
                break;

            case 'r':
                *p++ = '\r';
                break;

            case 'b':
                *p++ = '\b';
                break;

            case 't':
                *p++ = '\t';
                break;

            case 'f':
                *p++ = '\f';
                break;

            case 'v':
                *p++ = '\v';
                break;

                /* \ and up to 3 octal digits */
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
                val = c - '0';
                c = *s++;  /* try for 2 */
                if (c >= '0' && c <= '7') {
                    val = (val << 3) | (c - '0');
                    c = *s++;  /* try for 3 */
                    if (c >= '0' && c <= '7')
                        val = (val << 3) | (c - '0');
                    else
                        --s;
                }
                else
                    --s;
                *p++ = (char) val;
                break;

                /* \x and up to 3 hex digits */
            case 'x':
                val = 'x';            /* Default if no digits */
                c = hextoint(*s++);   /* Get next char */
                if (c >= 0) {
                    val = c;
                    c = hextoint(*s++);
                    if (c >= 0) {
                        val = (val << 4) + c;
                        c = hextoint(*s++);
                        if (c >= 0) {
                            val = (val << 4) + c;
                        }
                        else
                            --s;
                    }
                    else
                        --s;
                }
                else
                    --s;
                *p++ = (char) val;
                break;
            }
        }
        else
            *p++ = (char) c;
    }
  out:
    *p = '\0';
    *slen = p - origp;
    return s;
}