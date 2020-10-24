static unsigned long signextend(server_rec *s, struct magic *m, unsigned long v)
{
    if (!(m->flag & UNSIGNED))
        switch (m->type) {
            /*
             * Do not remove the casts below.  They are vital. When later
             * compared with the data, the sign extension must have happened.
             */
        case BYTE:
            v = (char) v;
            break;
        case SHORT:
        case BESHORT:
        case LESHORT:
            v = (short) v;
            break;
        case DATE:
        case BEDATE:
        case LEDATE:
        case LONG:
        case BELONG:
        case LELONG:
            v = (long) v;
            break;
        case STRING:
            break;
        default:
            ap_log_error(APLOG_MARK, APLOG_ERR, 0, s, APLOGNO(01520)
                        MODNAME ": can't happen: m->type=%d", m->type);
            return -1;
        }
    return v;
}