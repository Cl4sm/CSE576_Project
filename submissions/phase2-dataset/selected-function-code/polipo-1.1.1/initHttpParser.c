void
initHttpParser()
{
#define A(name, value) name = internAtom(value); if(!name) goto fail;
    /* These must be in lower-case */
    A(atomConnection, "connection");
    A(atomProxyConnection, "proxy-connection");
    A(atomContentLength, "content-length");
    A(atomHost, "host");
    A(atomAcceptRange, "accept-range");
    A(atomTE, "te");
    A(atomReferer, "referer");
    A(atomProxyAuthenticate, "proxy-authenticate");
    A(atomProxyAuthorization, "proxy-authorization");
    A(atomKeepAlive, "keep-alive");
    A(atomTrailer, "trailer");
    A(atomUpgrade, "upgrade");
    A(atomDate, "date");
    A(atomExpires, "expires");
    A(atomIfModifiedSince, "if-modified-since");
    A(atomIfUnmodifiedSince, "if-unmodified-since");
    A(atomIfRange, "if-range");
    A(atomLastModified, "last-modified");
    A(atomIfMatch, "if-match");
    A(atomIfNoneMatch, "if-none-match");
    A(atomAge, "age");
    A(atomTransferEncoding, "transfer-encoding");
    A(atomETag, "etag");
    A(atomCacheControl, "cache-control");
    A(atomPragma, "pragma");
    A(atomContentRange, "content-range");
    A(atomRange, "range");
    A(atomVia, "via");
    A(atomContentType, "content-type");
    A(atomContentEncoding, "content-encoding");
    A(atomVary, "vary");
    A(atomExpect, "expect");
    A(atomAuthorization, "authorization");
    A(atomSetCookie, "set-cookie");
    A(atomCookie, "cookie");
    A(atomCookie2, "cookie2");
    A(atomXPolipoDate, "x-polipo-date");
    A(atomXPolipoAccess, "x-polipo-access");
    A(atomXPolipoLocation, "x-polipo-location");
    A(atomXPolipoBodyOffset, "x-polipo-body-offset");
#undef A
    return;

 fail:
    do_log(L_ERROR, "Couldn't allocate atom.\n");
    exit(1);
}