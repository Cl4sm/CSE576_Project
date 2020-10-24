static int
writeHeaders(int fd, int *body_offset_return,
             ObjectPtr object, char *chunk, int chunk_len)
{
    int n, rc, error = -1;
    int body_offset = *body_offset_return;
    char *buf = NULL;
    int buf_is_chunk = 0;
    int bufsize = 0;

    if(object->flags & OBJECT_LOCAL)
        return -1;

    if(body_offset > CHUNK_SIZE)
        goto overflow;

    /* get_chunk might trigger object expiry */
    bufsize = CHUNK_SIZE;
    buf_is_chunk = 1;
    buf = maybe_get_chunk();
    if(!buf) {
        bufsize = 2048;
        buf_is_chunk = 0;
        buf = malloc(2048);
        if(buf == NULL) {
            do_log(L_ERROR, "Couldn't allocate buffer.\n");
            return -1;
        }
    }

 format_again:
    n = snnprintf(buf, 0, bufsize, "HTTP/1.1 %3d %s",
                  object->code, object->message->string);

    n = httpWriteObjectHeaders(buf, n, bufsize, object, 0, -1);
    if(n < 0)
        goto overflow;

    n = snnprintf(buf, n, bufsize, "\r\nX-Polipo-Location: ");
    n = snnprint_n(buf, n, bufsize, object->key, object->key_size);

    if(object->age >= 0 && object->age != object->date) {
        n = snnprintf(buf, n, bufsize, "\r\nX-Polipo-Date: ");
        n = format_time(buf, n, bufsize, object->age);
    }

    if(object->atime >= 0) {
        n = snnprintf(buf, n, bufsize, "\r\nX-Polipo-Access: ");
        n = format_time(buf, n, bufsize, object->atime);
    }

    if(n < 0)
        goto overflow;

    if(body_offset < 0)
        body_offset = chooseBodyOffset(n, object);

    if(body_offset > bufsize)
        goto overflow;

    if(body_offset > 0 && body_offset != n + 4)
        n = snnprintf(buf, n, bufsize, "\r\nX-Polipo-Body-Offset: %d",
                      body_offset);

    n = snnprintf(buf, n, bufsize, "\r\n\r\n");
    if(n < 0)
        goto overflow;

    if(body_offset < 0)
        body_offset = n;
    if(n > body_offset) {
        error = -2;
        goto fail;
    }

    if(n < body_offset)
        memset(buf + n, 0, body_offset - n);

 again:
#ifdef HAVE_READV_WRITEV
    if(chunk_len > 0) {
        struct iovec iov[2];
        iov[0].iov_base = buf;
        iov[0].iov_len = body_offset;
        iov[1].iov_base = chunk;
        iov[1].iov_len = chunk_len;
        rc = writev(fd, iov, 2);
    } else
#endif
        rc = write(fd, buf, body_offset);

    if(rc < 0 && errno == EINTR)
        goto again;

    if(rc < body_offset)
        goto fail;
    if(object->length >= 0 && 
       rc - body_offset >= object->length)
        object->flags |= OBJECT_DISK_ENTRY_COMPLETE;

    *body_offset_return = body_offset;
    if(buf_is_chunk)
        dispose_chunk(buf);
    else
        free(buf);
    return rc;

 overflow:
    if(bufsize < bigBufferSize) {
        char *oldbuf = buf;
        buf = malloc(bigBufferSize);
        if(!buf) {
            do_log(L_ERROR, "Couldn't allocate big buffer.\n");
            goto fail;
        }
        bufsize = bigBufferSize;
        if(oldbuf) {
            if(buf_is_chunk)
                dispose_chunk(oldbuf);
            else
                free(oldbuf);
        }
        buf_is_chunk = 0;
        goto format_again;
    }
    /* fall through */

 fail:
    if(buf_is_chunk)
        dispose_chunk(buf);
    else
        free(buf);
    return error;
}