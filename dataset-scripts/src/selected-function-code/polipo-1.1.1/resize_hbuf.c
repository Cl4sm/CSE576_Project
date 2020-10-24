static char *
resize_hbuf(char *hbuf, int *size, char *hbuf_small)
{
    int new_size = 2 * *size;
    char *new_hbuf;

    if(new_size <= *size)
        goto fail;

    if(hbuf == hbuf_small) {
        new_hbuf = malloc(new_size);
        if(new_hbuf == NULL) goto fail;
        memcpy(new_hbuf, hbuf, *size);
    } else {
        new_hbuf = realloc(hbuf, new_size);
        if(new_hbuf == NULL) goto fail;
    }
    *size = new_size;
    return new_hbuf;

 fail:
    if(hbuf != hbuf_small)
        free(hbuf);
    *size = 0;
    return NULL;
}