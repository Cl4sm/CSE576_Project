int
buildClientAuthHeaders(AtomPtr url, char *word,
                       AtomPtr *message_return, AtomPtr *headers_return)
{
    int code;
    char *h;
    AtomPtr message, headers;
    if(urlIsLocal(url->string, url->length)) {
        code = 401;
        message = internAtomF("Server authentication %s", word);
        h = "WWW-Authenticate";
    } else {
        code = 407;
        message = internAtomF("Proxy authentication %s", word);
        h = "Proxy-Authenticate";
    }
    headers = internAtomF("\r\n%s: Basic realm=\"%s\"",
                          h, authRealm->string);
    if(message_return)
        *message_return = message;
    else
        releaseAtom(message);
    *headers_return = headers;
    return code;
}