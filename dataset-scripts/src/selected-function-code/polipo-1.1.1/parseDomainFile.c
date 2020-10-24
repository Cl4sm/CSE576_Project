void
parseDomainFile(AtomPtr file,
                DomainPtr **domains_return, regex_t **regex_return)
{
    struct stat ss;
    int rc;

    if(*domains_return) {
        DomainPtr *domain = *domains_return;
        while(*domain) {
            free(*domain);
            domain++;
        }
        free(*domains_return);
        *domains_return = NULL;
    }

    if(*regex_return) {
        regfree(*regex_return);
        *regex_return = NULL;
    }

    if(!file || file->length == 0)
        return;

    domains = malloc(64 * sizeof(DomainPtr));
    if(domains == NULL) {
        do_log(L_ERROR, "Couldn't allocate domain list.\n");
        return;
    }
    dlen = 0;
    dsize = 64;

    regexbuf = malloc(512);
    if(regexbuf == NULL) {
        do_log(L_ERROR, "Couldn't allocate regex.\n");
        free(domains);
        return;
    }
    rlen = 0;
    rsize = 512;

    rc = stat(file->string, &ss);
    if(rc < 0) {
        if(errno != ENOENT)
            do_log_error(L_WARN, errno, "Couldn't stat file %s", file->string);
    } else {
        if(!S_ISDIR(ss.st_mode))
            readDomainFile(file->string);
        else {
            char *fts_argv[2];
            FTS *fts;
            FTSENT *fe;
            fts_argv[0] = file->string;
            fts_argv[1] = NULL;
            fts = fts_open(fts_argv, FTS_LOGICAL, NULL);
            if(fts) {
                while(1) {
                    fe = fts_read(fts);
                    if(!fe) break;
                    if(fe->fts_info != FTS_D && fe->fts_info != FTS_DP &&
                       fe->fts_info != FTS_DC && fe->fts_info != FTS_DNR)
                        readDomainFile(fe->fts_accpath);
                }
                fts_close(fts);
            } else {
                do_log_error(L_ERROR, errno,
                             "Couldn't scan directory %s", file->string);
            }
        }
    }

    if(dlen > 0) {
        domains[dlen] = NULL;
    } else {
        free(domains);
        domains = NULL;
    }

    regex_t *regex;

    if(rlen > 0) {
        regex = malloc(sizeof(regex_t));
        rc = regcomp(regex, regexbuf, REG_EXTENDED | REG_NOSUB);
        if(rc != 0) {
            char errbuf[100];
            regerror(rc, regex, errbuf, 100);
            do_log(L_ERROR, "Couldn't compile regex: %s.\n", errbuf);
            free(regex);
            regex = NULL;
        }
    } else {
        regex = NULL;
    }
    free(regexbuf);

    *domains_return = domains;
    *regex_return = regex;

    return;
}