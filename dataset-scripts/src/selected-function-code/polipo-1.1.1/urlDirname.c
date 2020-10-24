static int
urlDirname(char *buf, int n, const char *url, int len)
{
    int i, j;
    if(len < 8)
        return -1;
    if(lwrcmp(url, "http://", 7) != 0)
        return -1;

    if(checkRoot(localDocumentRoot) <= 0)
        return -1;

    if(n <= diskCacheRoot->length)
        return -1;

    memcpy(buf, diskCacheRoot->string, diskCacheRoot->length);
    j = diskCacheRoot->length;

    if(buf[j - 1] != '/')
        buf[j++] = '/';

    for(i = 7; i < len; i++) {
        if(i >= len || url[i] == '/')
            break;
        if(url[i] == '.' && i != len - 1 && url[i + 1] == '.')
            return -1;
        if(url[i] == '%' || !fssafe(url[i])) {
            if(j + 3 >= n) return -1;
            buf[j++] = '%';
            buf[j++] = i2h((url[i] & 0xF0) >> 4);
            buf[j++] = i2h(url[i] & 0x0F);
        } else {
            buf[j++] = url[i]; if(j >= n) return -1;
        }
    }
    buf[j++] = '/'; if(j >= n) return -1;
    buf[j] = '\0';
    return j;
}