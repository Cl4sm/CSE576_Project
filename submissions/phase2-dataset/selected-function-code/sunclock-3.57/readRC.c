readRC(fname, verbosity)
char *fname;        /* Path to .sunclockrc file */
int verbosity;
{
    /*
     * Local Variables
     */

    FILE *rc;           /* File pointer for rc file */
    char buf[1028];     /* Buffer to hold input lines */
    int  j;

    /* Open the RC file */

    if ((rc = fopen(fname, "r")) == NULL) {
        if (verbosity)
           fprintf(stderr, 
               "Unable to find the config file  %s \n", fname);
        return(1);
    }

    /* Read and parse lines from the file */

    while (fgets(buf, 1024, rc)) {

        /* Look for blank lines or comments */

        j=0;
        while (j<1024 && isspace(buf[j]) && buf[j] != '0') ++j; 
        if ((buf[j] == '#') || (buf[j] == '\0')) continue;

        if (parseCmdLine(buf))
	   fprintf(stderr,"Recheck syntax of config file %s !!\n\n", fname);
           continue;
        }

    if (rc) fclose(rc);
    return(0);
}
