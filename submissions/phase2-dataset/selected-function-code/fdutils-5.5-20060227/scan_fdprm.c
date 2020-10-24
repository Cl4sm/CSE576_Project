int scan_fdprm(FILE *file, char *name, struct floppy_struct *ft, 
	       void (*callback)(char *name, char *comment,
				struct floppy_struct *ft))
{
	char line[MAXLINE+2],this[MAXLINE+2],param[9][MAXLINE+2];
	char *params[9],*start;
	char *comment;
	int i, lineno;
	
	lineno = 0;
	while (fgets(line,MAXLINE,file)) {
		lineno++;
		for (start = line; *start == ' ' || *start == '\t'; start++);
		if (!*start || *start == '\n' || *start == '#') {
			/* comment line, print as is */
			if(!name)
				printf("%s", line);
			continue;
		}
		
		comment = strchr(start, '#');
		if(comment) {
			*comment='\0';
			comment++;
		}
		if (sscanf(start,"%s %s %s %s %s %s %s %s %s %s",this,param[0],
			   param[1],param[2],param[3],param[4],param[5],param[6],
			   param[7],param[8]) != 10) {
			fprintf(stderr,"Syntax error in line %d: '%s'\n",
				lineno, line);
			exit(1);
		}
		for(i=0;i<9;i++)
			params[i]=param[i];
		if(!name || !strcmp(this,name) ) {			
			i=set_params(params, ft);
			if(name)
				return 0;
			else
				callback(this, comment, ft);
		}
	}
	return 1;
}
