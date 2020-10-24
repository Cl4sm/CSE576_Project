int
streamRequestDone(StreamRequestPtr request)
{
    int len123 = 
        request->len + request->len2 +
        ((request->operation & IO_BUF3) ? request->u.b.len3 : 0);

    if(request->offset < 0)
        return 0;
    else if(request->offset < len123)
        return 0;
    else if(request->operation & IO_CHUNKED) {
        if(request->operation & IO_END) {
            if(request->offset < len123 + (len123 ? 7 : 5))
                return 0;
        } else {
            if(request->offset < len123 + 2)
                return 0;
        }
    }

    return 1;
}