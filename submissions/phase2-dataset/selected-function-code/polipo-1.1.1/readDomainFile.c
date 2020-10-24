int
readDomainFile(char *filename)
{
    FILE *in;
    char buf[512];
    char *rs;
    int i, j, is_regex, start;

    in = fopen(filename, "r");
    if(in == NULL) {
        if(errno != ENOENT)
            do_log_error(L_ERROR, errno, "Couldn't open file %s", filename);
        return -1;
    }

    while(1) {
        rs = fgets(buf, 512, in);
        if(rs == NULL)
            break;
        for(i = 0; i < 512; i++) {
            if(buf[i] != ' ' && buf[i] != '\t')
                break;
        }
        start = i;
        for(i = start; i < 512; i++) {
            if(buf[i] == '#' || buf[i] == '\r' || buf[i] == '\n')
                break;
        }
        while(i > start) {
            if(buf[i - 1] != ' ' && buf[i - 1] != '\t')
                break;
            i--;
        }

        if(i <= start)
            continue;

        /* The significant part of the line is now between start and i */

        is_regex = 0;
        for(j = start; j < i; j++) {
            if(buf[j] == '\\' || buf[j] == '*' || buf[j] == '/') {
                is_regex = 1;
                break;
            }
        }

        if(is_regex) {
            while(rlen + i - start + 8 >= rsize) {
                char *new_regexbuf;
                new_regexbuf = realloc(regexbuf, rsize * 2 + 1);
                if(new_regexbuf == NULL) {
                    do_log(L_ERROR, "Couldn't reallocate regex.\n");
                    fclose(in);
                    return -1;
                }
                regexbuf = new_regexbuf;
                rsize = rsize * 2 + 1;
            }
            if(rlen != 0)
                rlen = snnprintf(regexbuf, rlen, rsize, "|");
            rlen = snnprintf(regexbuf, rlen, rsize, "(");
            rlen = snnprint_n(regexbuf, rlen, rsize, buf + start, i - start);
            rlen = snnprintf(regexbuf, rlen, rsize, ")");
        } else {
            DomainPtr new_domain;
            if(dlen >= dsize - 1) {
                DomainPtr *new_domains;
                new_domains = realloc(domains, (dsize * 2 + 1) *
                                      sizeof(DomainPtr));
                if(new_domains == NULL) {
                    do_log(L_ERROR,
                           "Couldn't reallocate domain list.\n");
                    fclose(in);
                    return -1;
                }
                domains = new_domains;
                dsize = dsize * 2 + 1;
            }
            new_domain = malloc(sizeof(DomainRec) - 1 + i - start);
            if(new_domain == NULL) {
                do_log(L_ERROR, "Couldn't allocate domain.\n");
                fclose(in);
                return -1;
            }
            new_domain->length = i - start;
            memcpy(new_domain->domain, buf + start, i - start);
            domains[dlen++] = new_domain;
        }
    }
    fclose(in);
    return 1;
}