static int fsmagic(request_rec *r, const char *fn)
{
    switch (r->finfo.filetype) {
    case APR_DIR:
        magic_rsl_puts(r, DIR_MAGIC_TYPE);
        return DONE;
    case APR_CHR:
        /*
         * (void) magic_rsl_printf(r,"character special (%d/%d)",
         * major(sb->st_rdev), minor(sb->st_rdev));
         */
        (void) magic_rsl_puts(r, MIME_BINARY_UNKNOWN);
        return DONE;
    case APR_BLK:
        /*
         * (void) magic_rsl_printf(r,"block special (%d/%d)",
         * major(sb->st_rdev), minor(sb->st_rdev));
         */
        (void) magic_rsl_puts(r, MIME_BINARY_UNKNOWN);
        return DONE;
        /* TODO add code to handle V7 MUX and Blit MUX files */
    case APR_PIPE:
        /*
         * magic_rsl_puts(r,"fifo (named pipe)");
         */
        (void) magic_rsl_puts(r, MIME_BINARY_UNKNOWN);
        return DONE;
    case APR_LNK:
        /* We used stat(), the only possible reason for this is that the
         * symlink is broken.
         */
        ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, r, APLOGNO(01527)
                    MODNAME ": broken symlink (%s)", fn);
        return HTTP_INTERNAL_SERVER_ERROR;
    case APR_SOCK:
        magic_rsl_puts(r, MIME_BINARY_UNKNOWN);
        return DONE;
    case APR_REG:
        break;
    default:
        ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, r, APLOGNO(01528)
                      MODNAME ": invalid file type %d.", r->finfo.filetype);
        return HTTP_INTERNAL_SERVER_ERROR;
    }

    /*
     * regular file, check next possibility
     */
    if (r->finfo.size == 0) {
        magic_rsl_puts(r, MIME_TEXT_UNKNOWN);
        return DONE;
    }
    return OK;
}