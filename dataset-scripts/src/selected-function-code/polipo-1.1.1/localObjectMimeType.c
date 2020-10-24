static char*
localObjectMimeType(ObjectPtr object, char **encoding_return)
{
    char *name = object->key;
    int nlen = object->key_size;
    int i;

    assert(nlen >= 1);

    if(name[nlen - 1] == '/') {
        *encoding_return = NULL;
        return "text/html";
    }

    if(nlen < 3) {
        *encoding_return = NULL;
        return "application/octet-stream";
    }

    if(memcmp(name + nlen - 3, ".gz", 3) == 0) {
        *encoding_return = "x-gzip";
        nlen -= 3;
    } else if(memcmp(name + nlen - 2, ".Z", 2) == 0) {
        *encoding_return = "x-compress";
        nlen -= 2;
    } else {
        *encoding_return = NULL;
    }

    for(i = 0; i < sizeof(mimeEntries) / sizeof(mimeEntries[0]); i++) {
        int len = strlen(mimeEntries[i].extension);
        if(nlen > len && 
           name[nlen - len - 1] == '.' &&
           memcmp(name + nlen - len, mimeEntries[i].extension, len) == 0)
            return mimeEntries[i].mime;
    }

    return "application/octet-stream";
}