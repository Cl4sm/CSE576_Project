static int zmagic(request_rec *r, unsigned char *buf, apr_size_t nbytes)
{
    unsigned char *newbuf;
    int newsize;
    int i;

    for (i = 0; i < ncompr; i++) {
        if (nbytes < compr[i].maglen)
            continue;
        if (memcmp(buf, compr[i].magic, compr[i].maglen) == 0)
            break;
    }

    if (i == ncompr)
        return 0;

    if ((newsize = uncompress(r, i, &newbuf, HOWMANY)) > 0) {
        /* set encoding type in the request record */
        r->content_encoding = compr[i].encoding;

        newbuf[newsize-1] = '\0';  /* null-terminate uncompressed data */
        /* Try to detect the content type of the uncompressed data */
        if (tryit(r, newbuf, newsize, 0) != OK) {
            return 0;
        }
    }
    return 1;
}