char *concatstr(int count, ...)
{
	va_list arg;
        const char *str;
        int i;
        size_t len = 0;
        char *retstr;

        va_start(arg, count);

        for(i = 0; i < count; i++) {
                str = va_arg(arg, const char *);
                if(str)
                        len += strlen(str);
        }

        retstr = mymalloc(len + 1);
        retstr[0] = retstr[len] = 0;

        va_start(arg, count);

        for(i = 0; i < count; i++) {
                str = va_arg(arg, const char *);
                if(str)
                        strcat(retstr, str);
        }

        va_end(arg);

        return retstr;
}