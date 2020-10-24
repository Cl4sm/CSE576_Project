FILE *OpenOptionsFile( char *mode )
{
    char filename[200];
    FILE *file;
    
    if ((char *)getenv("HOME") != NULL)
		snprintf( filename, sizeof(filename), "%s/.penguin-command", getenv("HOME"));
    else
		sprintf( filename, "penguin-command.dat");

    file = fopen(filename, mode);
    
    if (file==NULL) {
	fprintf(stderr, "\nWarning: I could not open the options file ");
	if (strcmp(mode, "r") == 0)
	    fprintf(stderr, "for read:");
	else if (strcmp(mode, "w") == 0)
	    fprintf(stderr, "for write:");

	fprintf(stderr, "\n%s\n"
	    "The error that occured was:\n"
	    "%s\n\n", filename, strerror(errno));
    }
    
    return file;
}