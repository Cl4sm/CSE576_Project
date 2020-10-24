needMore(argc, argv)
int *                  argc;
char **                argv;
{
	-- *argc;
        if (*argc == 0) {
                if (runlevel == PARSECMDLINE) {
		   Usage();
		   exit(1);
		}
		   else 
                if (runlevel == RUNTIMEOPTION) {
                   fprintf(stderr, "Invalid option specification\n");
		   runlevel = FAILEDOPTION;
		} else
                   fprintf(stderr, "Error in config file : \n");

                fprintf(stderr, "%s: option `%s' (with no argument) incorrect\n",
                        ProgName, *argv);

		if (runlevel == PARSECMDLINE) exit(1);
	        return 1;
        }
	return 0;
}
