int argvify(int *cmd_argc, char **cmd_argv[], const char *cmdline){
	int i=0, num_args=1, cmd_len=strlen(cmdline);
	char **argv=NULL, *next_arg=NULL, *cmd_copy=strdup(cmdline);

	if(strtok(cmd_copy, " \t\n")) num_args++;
	else { 
		fprintf(stderr, "argvify: unable to parse string!\n");
		return -1;
	}
	while(strtok(NULL, " \t\n")) num_args++;

	argv=(char **)malloc(sizeof(char*)*(num_args+1));
	if(!argv) { perror("argvify"); return -1; }
	else memset(argv, 0, sizeof(char*)*(num_args+1));

	argv[0]=strdup(g_config.progname);
	next_arg=cmd_copy;
	for(i=1; i<num_args; i++){
		while(next_arg[0]=='\0') next_arg+=sizeof(char);
		argv[i] = strdup(next_arg);
		next_arg=memchr(next_arg, '\0',cmd_len-(next_arg-cmd_copy));
	}
	argv[num_args]=NULL;
	*(cmd_argv)=argv;
	*(cmd_argc)=num_args-1;
	free(cmd_copy);
	return 0;
}
