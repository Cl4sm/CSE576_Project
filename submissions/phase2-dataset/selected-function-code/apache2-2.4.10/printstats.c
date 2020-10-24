static void printstats(char *path, struct stats *s)
{
    char ttype, stype, mtype, utype;
    apr_off_t tfrag, sfrag, ufrag;

    if (!verbose) {
        return;
    }

    ttype = 'K';
    tfrag = ((s->total * 10) / KBYTE) % 10;
    s->total /= KBYTE;
    if (s->total >= KBYTE) {
        ttype = 'M';
        tfrag = ((s->total * 10) / KBYTE) % 10;
        s->total /= KBYTE;
    }

    stype = 'K';
    sfrag = ((s->sum * 10) / KBYTE) % 10;
    s->sum /= KBYTE;
    if (s->sum >= KBYTE) {
        stype = 'M';
        sfrag = ((s->sum * 10) / KBYTE) % 10;
        s->sum /= KBYTE;
    }

    mtype = 'K';
    s->max /= KBYTE;
    if (s->max >= KBYTE) {
        mtype = 'M';
        s->max /= KBYTE;
    }

    apr_file_printf(errfile, "Cleaned %s. Statistics:" APR_EOL_STR, path);
    if (unsolicited) {
        utype = 'K';
        ufrag = ((unsolicited * 10) / KBYTE) % 10;
        unsolicited /= KBYTE;
        if (unsolicited >= KBYTE) {
            utype = 'M';
            ufrag = ((unsolicited * 10) / KBYTE) % 10;
            unsolicited /= KBYTE;
        }
        if (!unsolicited && !ufrag) {
            ufrag = 1;
        }
        apr_file_printf(errfile, "unsolicited size %d.%d%c" APR_EOL_STR,
                        (int)(unsolicited), (int)(ufrag), utype);
    }
    apr_file_printf(errfile, "size limit %" APR_OFF_T_FMT ".0%c" APR_EOL_STR,
            s->max, mtype);
    apr_file_printf(errfile, "inodes limit %" APR_OFF_T_FMT APR_EOL_STR,
            s->inodes);
    apr_file_printf(
            errfile,
            "total size was %" APR_OFF_T_FMT ".%" APR_OFF_T_FMT "%c, total size now "
            "%" APR_OFF_T_FMT ".%" APR_OFF_T_FMT "%c" APR_EOL_STR, s->total,
            tfrag, ttype, s->sum, sfrag, stype);
    apr_file_printf(errfile, "total inodes was %" APR_OFF_T_FMT
            ", total %sinodes now "
            "%" APR_OFF_T_FMT APR_EOL_STR, s->ntotal, dryrun && deldirs ? "estimated "
            : "", s->nodes);
    apr_file_printf(
            errfile,
            "total entries was %" APR_OFF_T_FMT ", total entries now %" APR_OFF_T_FMT
            APR_EOL_STR, s->etotal, s->entries);
    apr_file_printf(
            errfile,
            "%" APR_OFF_T_FMT " entries deleted (%" APR_OFF_T_FMT " from future, %"
            APR_OFF_T_FMT " expired, %" APR_OFF_T_FMT " fresh)" APR_EOL_STR,
            (s->etotal - s->entries), s->dfuture, s->dexpired, s->dfresh);
}