static int
httpClientDelayed(TimeEventHandlerPtr event)
{
     HTTPConnectionPtr connection = *(HTTPConnectionPtr*)event->data;

     /* IO_NOTNOW is unfortunate, but needed to avoid starvation if a
        client is pipelining a lot of requests. */
     if(connection->reqlen > 0) {
         int bufsize;
         if((connection->flags & CONN_BIGREQBUF) &&
            connection->reqlen < CHUNK_SIZE)
             httpConnectionUnbigifyReqbuf(connection);
         /* Don't read new requests if buffer is big. */
         bufsize = (connection->flags & CONN_BIGREQBUF) ?
             connection->reqlen : CHUNK_SIZE;
         do_stream(IO_READ | IO_IMMEDIATE | IO_NOTNOW,
                   connection->fd, connection->reqlen,
                   connection->reqbuf, bufsize,
                   httpClientHandler, connection);
     } else {
         httpConnectionDestroyReqbuf(connection);
         do_stream_buf(IO_READ | IO_NOTNOW,
                       connection->fd, 0,
                       &connection->reqbuf, CHUNK_SIZE,
                       httpClientHandler, connection);
     }
     return 1;
}