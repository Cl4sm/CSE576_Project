static char *
dirnameUrl(char *url, int n, char *name, int len)
{
    int i, j, k, c1, c2;
    k = diskCacheRoot->length;
    if(len < k)
        return NULL;
    if(memcmp(name, diskCacheRoot->string, k) != 0)
        return NULL;
    if(n < 8)
        return NULL;
    memcpy(url, "http://", 7);
    if(name[len - 1] == '/')
        len --;
    j = 7;
    for(i = k; i < len; i++) {
        if(name[i] == '%') {
            if(i >= len - 2)
                return NULL;
            c1 = h2i(name[i + 1]);
            c2 = h2i(name[i + 2]);
            if(c1 < 0 || c2 < 0)
                return NULL;
            url[j++] = c1 * 16 + c2; if(j >= n) goto fail;
            i += 2;             /* skip extra digits */
        } else if(i < len - 1 && 
                  name[i] == '.' && name[i + 1] == '/') {
                return NULL;
        } else if(i == len - 1 && name[i] == '.') {
            return NULL;
        } else {
            url[j++] = name[i]; if(j >= n) goto fail;
        }
    }
    url[j++] = '/'; if(j >= n) goto fail;
    url[j] = '\0';
    return url;

 fail:
    return NULL;
}