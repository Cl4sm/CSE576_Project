int
do_scheduled_stream(int status, FdEventHandlerPtr event)
{
    StreamRequestPtr request = (StreamRequestPtr)&event->data;
    int rc, done, i;
    struct iovec iov[6];
    int chunk_header_len;
    char chunk_header[10];
    int len12 = request->len + request->len2;
    int len123 = 
        request->len + request->len2 + 
        ((request->operation & IO_BUF3) ? request->u.b.len3 : 0);

    if(status) {
        done = request->handler(status, event, request);
        return done;
    }

    i = 0;

    if(request->offset < 0) {
        assert((request->operation & (IO_MASK | IO_BUF3 | IO_BUF_LOCATION)) == 
               IO_WRITE);
        if(request->operation & IO_CHUNKED) {
            chunk_header_len = chunkHeaderLen(len123);
        } else {
            chunk_header_len = 0;
        }

        if(request->offset < -chunk_header_len) {
            assert(request->offset >= -(request->u.h.hlen + chunk_header_len));
            iov[i].iov_base = request->u.h.header;
            iov[i].iov_len = -request->offset - chunk_header_len;
            i++;
        }

        if(chunk_header_len > 0) {
            chunkHeader(chunk_header, 10, len123);
            if(request->offset < -chunk_header_len) {
                iov[i].iov_base = chunk_header;
                iov[i].iov_len = chunk_header_len;
            } else {
                iov[i].iov_base = chunk_header + 
                    chunk_header_len + request->offset;
                iov[i].iov_len = -request->offset;
            }
            i++;
        }
    }

    if(request->len > 0) {
        if(request->buf == NULL && 
           (request->operation & IO_BUF_LOCATION)) {
            assert(*request->u.l.buf_location == NULL);
            request->buf = *request->u.l.buf_location = get_chunk();
            if(request->buf == NULL) {
                done = request->handler(-ENOMEM, event, request);
                return done;
            }
        }
        if(request->offset <= 0) {
            iov[i].iov_base = request->buf;
            iov[i].iov_len = request->len;
            i++;
        } else if(request->offset < request->len) {
            iov[i].iov_base = request->buf + request->offset;
            iov[i].iov_len = request->len - request->offset;
            i++;
        }
    }

    if(request->len2 > 0) {
        if(request->offset <= request->len) {
            iov[i].iov_base = request->buf2;
            iov[i].iov_len = request->len2;
            i++;
        } else if(request->offset < request->len + request->len2) {
            iov[i].iov_base = request->buf2 + request->offset - request->len;
            iov[i].iov_len = request->len2 - request->offset + request->len;
            i++;
        }
    }

    if((request->operation & IO_BUF3) && request->u.b.len3 > 0) {
        if(request->offset <= len12) {
            iov[i].iov_base = request->u.b.buf3;
            iov[i].iov_len = request->u.b.len3;
            i++;
        } else if(request->offset < len12 + request->u.b.len3) {
            iov[i].iov_base = request->u.b.buf3 + request->offset - len12;
            iov[i].iov_len = request->u.b.len3 - request->offset + len12;
            i++;
        }
    }

    if((request->operation & IO_CHUNKED)) {
        int l;
        const char *trailer;
        if(request->operation & IO_END) {
            if(len123 == 0) {
                trailer = endChunkTrailer + 2;
                l = 5;
            } else {
                trailer = endChunkTrailer;
                l = 7;
            }
        } else {
            trailer = endChunkTrailer;
            l = 2;
        }

        if(request->offset <= len123) {
            iov[i].iov_base = (char*)trailer;
            iov[i].iov_len = l;
            i++;
        } else if(request->offset < len123 + l) {
            iov[i].iov_base = 
                (char*)endChunkTrailer + request->offset - len123;
            iov[i].iov_len = l - request->offset + len123;
            i++;
        }
    }

    assert(i > 0);

    if((request->operation & IO_MASK) == IO_WRITE) {
        if(i > 1) 
            rc = WRITEV(request->fd, iov, i);
        else
            rc = WRITE(request->fd, iov[0].iov_base, iov[0].iov_len);
    } else {
        if(i > 1) 
            rc = READV(request->fd, iov, i);
        else
            rc = READ(request->fd, iov[0].iov_base, iov[0].iov_len);
    }

    if(rc > 0) {
        request->offset += rc;
        if(request->offset < 0) return 0;
        done = request->handler(0, event, request);
        return done;
    } else if(rc == 0 || errno == EPIPE) {
        done = request->handler(1, event, request);
    } else if(errno == EAGAIN || errno == EINTR) {
        return 0;
    } else if(errno == EFAULT || errno == EBADF) {
        abort();
    } else {
        done = request->handler(-errno, event, request);
    }
    assert(done);
    return done;
}