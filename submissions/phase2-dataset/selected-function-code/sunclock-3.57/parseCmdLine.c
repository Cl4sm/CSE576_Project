int
parseCmdLine(buf)
char * buf;
{
    int i, j, l;
    char *dup, *str;
    char ** argv;
    int argc;

    l = strlen(buf);
    dup = (char *) salloc((l+2)*sizeof(char));
    if (dup) {
       if (*buf == '-')
          strcpy(dup, buf);
       else {
	  strcpy(dup+1, buf);
	  *dup = '-';
       }
    } else return 1;

    j = 0;
    for (i=0 ; dup[i] ; ++i)
       if (isspace(dup[i])) ++j;

    argv = (char **) salloc((j+2)*sizeof(char *));

    i = argc = 0;

 rescan:
    while(dup[i] && isspace(dup[i])) {
      dup[i] = '\0';
      ++i;
    }

    if (dup[i]) {
       str = argv[argc] = dup+i;
       while(dup[i] && !isspace(dup[i])) ++i;
       if (str[0]==':' && (!str[1] || isspace(str[1]))) {
	  str[0] = '\0';
	  goto rescan;
       }
       ++argc;
       goto rescan;
    } else
       argv[argc] = NULL;

    for (i=0; i<l+1; i++)
       if (dup[i] && dup[i]<' ') dup[i] = ' ';

    for (i=0; i<argc; i++) {
       str = argv[i];
       l = strlen(str)-1;
       if (*str == '-' && str[l] == ':') str[l] = '\0';
    }

    l = parseArgs(argc+1, argv-1);

    free(dup);
    free(argv);
    return l;
}
