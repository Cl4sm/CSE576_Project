void
objectPrintf(ObjectPtr object, int offset, const char *format, ...)
{
    char *buf;
    int rc;

    va_list args;
    va_start(args, format);
    buf = vsprintf_a(format, args);
    va_end(args);

    if(buf == NULL) {
        abortObject(object, 500, internAtom("Couldn't allocate string"));
        return;
    }

    rc = objectAddData(object, buf, offset, strlen(buf));
    free(buf);
    if(rc < 0)
        abortObject(object, 500, internAtom("Couldn't add data to object"));
}