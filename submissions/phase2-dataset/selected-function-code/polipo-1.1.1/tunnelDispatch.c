static void
tunnelDispatch(TunnelPtr tunnel)
{
    if(circularBufferEmpty(&tunnel->buf1)) {
        if(tunnel->buf1.buf && 
           !(tunnel->flags & (TUNNEL_READER1 | TUNNEL_WRITER2))) {
            dispose_chunk(tunnel->buf1.buf);
            tunnel->buf1.buf = NULL;
            tunnel->buf1.head = tunnel->buf1.tail = 0;
        }
    }

    if(circularBufferEmpty(&tunnel->buf2)) {
        if(tunnel->buf2.buf &&
           !(tunnel->flags & (TUNNEL_READER2 | TUNNEL_WRITER1))) {
            dispose_chunk(tunnel->buf2.buf);
            tunnel->buf2.buf = NULL;
            tunnel->buf2.head = tunnel->buf2.tail = 0;
        }
    }

    if(tunnel->fd1 >= 0) {
        if(!(tunnel->flags & (TUNNEL_READER1 | TUNNEL_EOF1)) && 
           !circularBufferFull(&tunnel->buf1)) {
            tunnel->flags |= TUNNEL_READER1;
            bufRead(tunnel->fd1, &tunnel->buf1, tunnelRead1Handler, tunnel);
        }
        if(!(tunnel->flags & (TUNNEL_WRITER1 | TUNNEL_EPIPE1)) &&
           !circularBufferEmpty(&tunnel->buf2)) {
            tunnel->flags |= TUNNEL_WRITER1;
            /* There's no IO_NOTNOW in bufWrite, so it might close the
               file descriptor straight away.  Wait until we're
               rescheduled. */
            bufWrite(tunnel->fd1, &tunnel->buf2, tunnelWrite1Handler, tunnel);
            return;
        }
        if(tunnel->fd2 < 0 || (tunnel->flags & TUNNEL_EOF2)) {
            if(!(tunnel->flags & TUNNEL_EPIPE1))
                shutdown(tunnel->fd1, 1);
            tunnel->flags |= TUNNEL_EPIPE1;
        } else if(tunnel->fd1 < 0 || (tunnel->flags & TUNNEL_EPIPE2)) {
            if(!(tunnel->flags & TUNNEL_EOF1))
                shutdown(tunnel->fd1, 0);
            tunnel->flags |= TUNNEL_EOF1;
        }
        if((tunnel->flags & TUNNEL_EOF1) && (tunnel->flags & TUNNEL_EPIPE1)) {
            if(!(tunnel->flags & (TUNNEL_READER1 | TUNNEL_WRITER1))) {
                CLOSE(tunnel->fd1);
                tunnel->fd1 = -1;
            }
        }
    }

    if(tunnel->fd2 >= 0) {
        if(!(tunnel->flags & (TUNNEL_READER2 | TUNNEL_EOF2)) && 
           !circularBufferFull(&tunnel->buf2)) {
            tunnel->flags |= TUNNEL_READER2;
            bufRead(tunnel->fd2, &tunnel->buf2, tunnelRead2Handler, tunnel);
        }
        if(!(tunnel->flags & (TUNNEL_WRITER2 | TUNNEL_EPIPE2)) &&
           !circularBufferEmpty(&tunnel->buf1)) {
            tunnel->flags |= TUNNEL_WRITER2;
            bufWrite(tunnel->fd2, &tunnel->buf1, tunnelWrite2Handler, tunnel);
            return;
        }
        if(tunnel->fd1 < 0 || (tunnel->flags & TUNNEL_EOF1)) {
            if(!(tunnel->flags & TUNNEL_EPIPE2))
                shutdown(tunnel->fd2, 1);
            tunnel->flags |= TUNNEL_EPIPE2;
        } else if(tunnel->fd1 < 0 || (tunnel->flags & TUNNEL_EPIPE1)) {
            if(!(tunnel->flags & TUNNEL_EOF2))
                shutdown(tunnel->fd2, 0);
            tunnel->flags |= TUNNEL_EOF2;
        }
        if((tunnel->flags & TUNNEL_EOF2) && (tunnel->flags & TUNNEL_EPIPE2)) {
            if(!(tunnel->flags & (TUNNEL_READER2 | TUNNEL_WRITER2))) {
                CLOSE(tunnel->fd2);
                tunnel->fd2 = -1;
            }
        }
    }

    if(tunnel->fd1 < 0 && tunnel->fd2 < 0)
        destroyTunnel(tunnel);
    else
        assert(tunnel->flags & (TUNNEL_READER1 | TUNNEL_WRITER1 |
                                TUNNEL_READER2 | TUNNEL_WRITER2));
}