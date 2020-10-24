static int
parseResolvConf(char *filename)
{
    FILE *f;
    char buf[512];
    char *p, *q;
    int n;
    AtomPtr nameserver = NULL;

    f = fopen(filename, "r");
    if(f == NULL) {
        do_log_error(L_ERROR, errno, "DNS: couldn't open %s", filename);
        return 0;
    }

    while(1) {
        p = fgets(buf, 512, f);
        if(p == NULL)
            break;

        n = strlen(buf);
        if(buf[n - 1] != '\n') {
            int c;
            do_log(L_WARN, "DNS: overly long line in %s -- skipping.\n",
                   filename);
            do {
                c = fgetc(f);
                if(c == EOF)
                    break;
            } while(c != '\n');
            if(c == EOF)
                break;
        }
        
        while(*p == ' ' || *p == '\t')
            p++;
        if(strcasecmp_n("nameserver", p, 10) != 0)
            continue;
        p += 10;
        while(*p == ' ' || *p == '\t')
            p++;
        q = p;
        while(*q == '.' || *q == ':' || digit(*q) || letter(*q))
            q++;
        if(*q != ' ' && *q != '\t' && *q != '\r' && *q != '\n') {
            do_log(L_WARN, "DNS: couldn't parse line in %s -- skipping.\n",
                   filename);
            continue;
        }
        nameserver = internAtomLowerN(p, q - p);
        break;
    }

    fclose(f);
    if(nameserver) {
        dnsNameServer = nameserver;
        return 1;
    } else {
        return 0;
    }
}