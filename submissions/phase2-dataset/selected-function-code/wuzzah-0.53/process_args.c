void process_args(int ac, char *av[], wuzzah_config_t *conf){
	int c, len;
	char *homedir;

	optind=0;

	memset(conf, 0, sizeof(wuzzah_config_t));
	conf->progname=av[0]=basename(av[0]);	

	while(1){
		c=getopt_long(ac, av, "ac:f:Fhi:m:nopqs:u:vw", 
				program_opts, NULL);
		switch(c){
		case 'a': conf->all_users=1; break;
		case 'c': conf->eventcmd=strdup(optarg); break;
		case 'f': conf->infile=strdup(optarg); break;
		case 'F': conf->noloadfile=1; break;
		case 'h': version(); usage(); exit(0); break;
		case 'i': conf->sleep_interval=atoi(optarg); break;
		case 'm': conf->buddy_msg=strdup(optarg); break;
		case 'n': conf->no_newline=1; break;
		case 'o': conf->run_once=1;
		case 'p': conf->process_current=1; break;
		case 'q': conf->write_users=0; break;
		case 's': conf->status_msg=strdup(optarg); break;
		case 'u': conf->extrabuddies=strdup(optarg); break;
		case 'v': version(); exit(0); break;
		case 'w': conf->write_users=1; break;
		} 
		if(c==-1)break;
	}
	if(!conf->buddy_msg)conf->buddy_msg=g_default_buddy_msg;
	if(!conf->status_msg)conf->status_msg=g_default_status_msg;

	//	if the userfile hasn't been specified, use ${HOME}/.wuzzah
	if(!conf->infile && !conf->noloadfile){
		homedir=getenv("HOME");
		if(homedir==NULL) { 
			conf->noloadfile=1; 
		} else {
			len=strlen(homedir)+1+strlen(g_default_infile); // 1 for '/'
			conf->infile=(char*)malloc(sizeof(char)*(len+1));
			strncpy(conf->infile, homedir, strlen(homedir)+1);
			strncat(conf->infile, "/", 1);
			strncat(conf->infile, g_default_infile,
				strlen(g_default_infile));
		}
	}
	if(!conf->sleep_interval)
		conf->sleep_interval=g_default_sleep_interval;
}
