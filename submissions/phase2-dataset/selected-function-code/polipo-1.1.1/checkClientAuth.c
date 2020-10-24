int
checkClientAuth(AtomPtr auth, AtomPtr url,
                AtomPtr *message_return, AtomPtr *headers_return)
{
    int code = 0;
    AtomPtr message = NULL, headers = NULL;

    if(authRealm == NULL || authCredentials == NULL)
        return 0;

    if(auth == NULL)
        code = buildClientAuthHeaders(url, "required", &message, &headers);
    else if(auth->length >= 6 || lwrcmp(auth->string, "basic ", 6) == 0) {
        if(b64cmp(auth->string + 6, auth->length - 6,
                  authCredentials->string, authCredentials->length) == 0)
            return 0;
        code = buildClientAuthHeaders(url, "incorrect", &message, &headers);
    } else {
        code = buildClientAuthHeaders(url, NULL, NULL, &headers);
        message = internAtom("Unexpected authentication scheme");
    }

    *message_return = message;
    *headers_return = headers;
    return code;
}