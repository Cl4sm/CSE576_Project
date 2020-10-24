static const char *gen_unique_id(const request_rec *r)
{
    char *str;
    /*
     * Buffer padded with two final bytes, used to copy the unique_id_red
     * structure without the internal paddings that it could have.
     */
    unique_id_rec new_unique_id;
    struct {
        unique_id_rec foo;
        unsigned char pad[2];
    } paddedbuf;
    unsigned char *x,*y;
    unsigned short counter;
    int i,j,k;

    new_unique_id.in_addr = cur_unique_id.in_addr;
    new_unique_id.pid = cur_unique_id.pid;
    new_unique_id.counter = cur_unique_id.counter;

    new_unique_id.stamp = htonl((unsigned int)apr_time_sec(r->request_time));
    new_unique_id.thread_index = htonl((unsigned int)r->connection->id);

    /* we'll use a temporal buffer to avoid uuencoding the possible internal
     * paddings of the original structure */
    x = (unsigned char *) &paddedbuf;
    k = 0;
    for (i = 0; i < UNIQUE_ID_REC_MAX; i++) {
        y = ((unsigned char *) &new_unique_id) + unique_id_rec_offset[i];
        for (j = 0; j < unique_id_rec_size[i]; j++, k++) {
            x[k] = y[j];
        }
    }
    /*
     * We reset two more bytes just in case padding is needed for the uuencoding.
     */
    x[k++] = '\0';
    x[k++] = '\0';

    /* alloc str and do the uuencoding */
    str = (char *)apr_palloc(r->pool, unique_id_rec_size_uu + 1);
    k = 0;
    for (i = 0; i < unique_id_rec_total_size; i += 3) {
        y = x + i;
        str[k++] = uuencoder[y[0] >> 2];
        str[k++] = uuencoder[((y[0] & 0x03) << 4) | ((y[1] & 0xf0) >> 4)];
        if (k == unique_id_rec_size_uu) break;
        str[k++] = uuencoder[((y[1] & 0x0f) << 2) | ((y[2] & 0xc0) >> 6)];
        if (k == unique_id_rec_size_uu) break;
        str[k++] = uuencoder[y[2] & 0x3f];
    }
    str[k++] = '\0';

    /* and increment the identifier for the next call */

    counter = ntohs(new_unique_id.counter) + 1;
    cur_unique_id.counter = htons(counter);

    return str;
}