SFopenFile(name, mode, prompt, failed)
    char *name;
    char *mode;
    char *prompt;
    char *failed;
{
    Arg args[1];
    FILE *fp;

    SFchdir(SFstartDir);
    if ((fp = fopen(name, mode)) == NULL) {
	char *buf;
        char *syserrstring;
#ifdef USE_STRERROR
        syserrstring = strerror(errno);
#else
        if (errno <= sys_nerr)
            syserrstring = sys_errlist[errno];
        else
            syserrstring = "";
#endif
	buf = XtMalloc(strlen(failed) + strlen(syserrstring) + 
	    	   strlen(prompt) + 2);
	strcpy(buf, failed);
	strcat(buf, syserrstring);
	strcat(buf, "\n");
	strcat(buf, prompt);
	XtSetArg(args[0], XtNlabel, buf);
	XtSetValues(selFilePrompt, args, 1);
	XtFree(buf);
	return NULL;
    }
    return fp;
}
