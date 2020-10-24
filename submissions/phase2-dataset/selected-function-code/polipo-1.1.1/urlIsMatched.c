int
urlIsMatched(char *url, int length, DomainPtr *domains, regex_t *regex)
{
    /* This requires url to be NUL-terminated. */
    assert(url[length] == '\0');

    if(length < 8)
        return 0;

    if(lwrcmp(url, "http://", 7) != 0)
        return 0;

    if(domains) {
        int i;
        DomainPtr *domain;
        for(i = 8; i < length; i++) {
            if(url[i] == '/')
                break;
        }
        domain = domains;
        while(*domain) {
            if((*domain)->length <= (i - 7) &&
               (url[i - (*domain)->length - 1] == '.' ||
                url[i - (*domain)->length - 1] == '/') &&
               memcmp(url + i - (*domain)->length,
                      (*domain)->domain,
                      (*domain)->length) == 0)
                return 1;
            domain++;
        }
    }

    if(regex)
        return !regexec(regex, url, 0, NULL, 0);

    return 0;
}