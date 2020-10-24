int
httpWriteErrorHeaders(char *buf, int size, int offset, int do_body,
                      int code, AtomPtr message, int close, AtomPtr headers,
                      char *url, int url_len, char *etag)
{
    int n, m, i;
    char *body;
    char htmlMessage[100];
    char timeStr[100];

    assert(code != 0);

    i = htmlString(htmlMessage, 0, 100, message->string, message->length);
    if(i < 0)
        strcpy(htmlMessage, "(Couldn't format message)");
    else
        htmlMessage[MIN(i, 99)] = '\0';

    if(code != 304) {
        body = get_chunk();
        if(!body) {
            do_log(L_ERROR, "Couldn't allocate body buffer.\n");
            return -1;
        }
        m = snnprintf(body, 0, CHUNK_SIZE,
                      "<!DOCTYPE HTML PUBLIC "
                      "\"-//W3C//DTD HTML 4.01 Transitional//EN\" "
                      "\"http://www.w3.org/TR/html4/loose.dtd\">"
                      "\n<html><head>"
                      "\n<title>Proxy %s: %3d %s.</title>"
                      "\n</head><body>"
                      "\n<h1>%3d %s</h1>"
                      "\n<p>The following %s",
                      code >= 400 ? "error" : "result",
                      code, htmlMessage,
                      code, htmlMessage,
                      code >= 400 ? 
                      "error occurred" :
                      "status was returned");
        if(url_len > 0) {
            m = snnprintf(body, m, CHUNK_SIZE,
                          " while trying to access <strong>");
            m = htmlString(body, m, CHUNK_SIZE, url, url_len);
            m = snnprintf(body, m, CHUNK_SIZE, "</strong>");
        }

        {
            /* On BSD systems, tv_sec is a long. */
            const time_t ct = current_time.tv_sec;
                                             /*Mon, 24 Sep 2004 17:46:35 GMT*/
            strftime(timeStr, sizeof(timeStr), "%a, %d %b %Y %H:%M:%S %Z",
                     localtime(&ct));
        }
        
        m = snnprintf(body, m, CHUNK_SIZE,
                      ":<br><br>"
                      "\n<strong>%3d %s</strong></p>"
                      "\n<hr>Generated %s by %s on <em>%s:%d</em>."
                      "\n</body></html>\r\n",
                      code, htmlMessage,
                      timeStr, displayName->string, proxyName->string, proxyPort);
        if(m <= 0 || m >= CHUNK_SIZE) {
            do_log(L_ERROR, "Couldn't write error body.\n");
            dispose_chunk(body);
            return -1;
        }
    } else {
        body = NULL;
        m = 0;
    }

    n = snnprintf(buf, 0, size,
                 "HTTP/1.1 %3d %s"
                 "\r\nConnection: %s"
                 "\r\nDate: ",
                  code, atomString(message),
                  close ? "close" : "keep-alive");
    n = format_time(buf, n, size, current_time.tv_sec);
    if(code != 304) {
        n = snnprintf(buf, n, size,
                      "\r\nContent-Type: text/html"
                      "\r\nContent-Length: %d", m);
    } else {
        if(etag)
            n = snnprintf(buf, n, size, "\r\nETag: \"%s\"", etag);
    }

    if(code != 304 && code != 412) {
        n = snnprintf(buf, n, size,
                      "\r\nExpires: 0"
                      "\r\nCache-Control: no-cache"
                      "\r\nPragma: no-cache");
    }

    if(headers)
        n = snnprint_n(buf, n, size,
                      headers->string, headers->length);

    n = snnprintf(buf, n, size, "\r\n\r\n");

    if(n < 0 || n >= size) {
        do_log(L_ERROR, "Couldn't write error.\n");
        dispose_chunk(body);
        return -1;
    }

    if(code != 304 && do_body) {
        if(m > 0) memcpy(buf + n, body, m);
        n += m;
    }

    if(body)
        dispose_chunk(body);

    return n;
}