void
checkRCfile(argc, argv)
register int                    argc;
register char **                argv;
{
        int i;

        for (i=1; i<argc; i++) {
           if (i<argc-1 && !strcasecmp(argv[i], "-rcfile"))
              rc_file = argv[i+1];
           if (i<argc-1 && !strcasecmp(argv[i], "-language")) {
              strncpy(language, argv[i+1], 2);
	      language[2] = '\0';
	   }
           if (strcasecmp(argv[i], "-verbose") == 0)
	      verbose = 1;
	} 

        if (strcmp(language, oldlanguage)) readLanguage();
}
