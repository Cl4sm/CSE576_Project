int string_to_argv(const char *str, char **av[]){
	int c=0, i=0, j=0, num_args=1, len=strlen(str), procd_str_pipe[2];
	int arg_len=0, arg_max_len=0;
	short parsing_whitespace=1;
	char **argv=NULL, *tmp=strdup(str);
	FILE *p_write, *p_read;

	if(pipe(procd_str_pipe)) perror("opening pipe");
	p_read=fdopen(procd_str_pipe[0], "r");
	if(!p_read) perror("opening read end of pipe");
	p_write=fdopen(procd_str_pipe[1], "w");
	if(!p_write) perror("opening write end of pipe");

	for(i=0;i<len;i++){
		switch(str[i]){
		case '\\':
			if(parsing_whitespace) {
				num_args++;
				parsing_whitespace=0;
			}
			if(i==len-1) {
				fprintf(stderr, 
					"syntax err: unterminated \\\n");
				return -1;
			}
			fputc(str[i++], p_write);
			fputc(str[i], p_write);
			arg_len+=2;
			break;
		case ' ': case '\n': case '\t':
			if(!parsing_whitespace){
				if(arg_len>arg_max_len)arg_max_len=arg_len;
				arg_len=0;
				fputc('\0', p_write);
				parsing_whitespace=1;
			} 
			break;
		case '\'':
			if(parsing_whitespace) {
				num_args++;
				parsing_whitespace=0;
			}
			while(str[i+1] != '\'' || str[i] == '\\'){
				if(++i > len){
					fprintf(stderr, 
						"error: unterminated '\n");
					return -1;
				}
				if(str[i+1]=='\'' && str[i] == '\\') continue;
				else {
					arg_len++;
					fputc(str[i], p_write);
				}
			}
			i++;
			break;
		case '"':
			if(parsing_whitespace) {
				num_args++;
				parsing_whitespace=0;
			}
			while(str[i+1] != '"' || str[i] == '\\'){
				if(++i > len){
					fprintf(stderr, 
						"error: unterminated \"\n");
					return -1;
				}
				if(str[i+1]=='"' && str[i] == '\\') continue;
				else {
					arg_len++;
					fputc(str[i], p_write);
				}
			}
			i++;
			break;
		default: 
			if(parsing_whitespace) {
				num_args++;
				parsing_whitespace=0;
			}
			arg_len++;
			fputc(str[i], p_write);
			break;
		}
	}

	fclose(p_write);
	argv=(char **)malloc(sizeof(char*)*(num_args+1));
	memset(argv, 0, sizeof(char)*(num_args+1));

	argv[0]=strdup(g_config.progname);

	for(i=1; i<num_args; i++){
		argv[i]=(char*)malloc(sizeof(char)*(arg_max_len+1));
		memset(argv[i], 0, sizeof(char)*(arg_max_len+1));
		c=fgetc(p_read);
		for(j=0; c!='\0' && !feof(p_read) && !ferror(p_read); j++){
			argv[i][j] = c;	
			c=fgetc(p_read);
		}
		argv[i][j]='\0';
	}
	free(tmp);
	argv[num_args]=NULL;
	*(av)=argv;
	return num_args;
}
