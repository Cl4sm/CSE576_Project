int
localFilename(char *buf, int n, char *key, int len)
{
    int i, j;
    if(len <= 0 || key[0] != '/') return -1;

    if(urlIsSpecial(key, len)) return -1;

    if(checkRoot(localDocumentRoot) <= 0)
        return -1;

    if(n <= localDocumentRoot->length)
        return -1;

    i = 0;
    if(key[i] != '/')
        return -1;

    memcpy(buf, localDocumentRoot->string, localDocumentRoot->length);
    j = localDocumentRoot->length;
    if(buf[j - 1] == '/')
        j--;

    while(i < len) {
        if(j >= n - 1)
            return -1;
        if(key[i] == '/' && i < len - 2)
            if(key[i + 1] == '.' && 
               (key[i + 2] == '.' || key[i + 2] == '/'))
                return -1;
        buf[j++] = key[i++];
    }

    if(buf[j - 1] == '/') {
        if(j >= n - 11)
            return -1;
        memcpy(buf + j, "index.html", 10);
        j += 10;
    }

    buf[j] = '\0';
    return j;
}