buddy_t* bud_create_buddy(const char *line){
	char *line_buf=strdup(line);
	buddy_t *next_buddy=MSTRUCT(buddy_t);
	short colon=0, len=0;
	int ac, i;
	char **av=NULL;
	wuzzah_config_t *bconf=NULL;

	// now check for a per-user config 
	len=strlen(line_buf);
	colon=strcspn(line_buf, ":");
	if(colon<len-1) {
		line_buf[colon]='\0';
		bconf=MSTRUCT(wuzzah_config_t);
		ac=string_to_argv(&line_buf[colon+1], &av);
		if(ac == -1) bail("error parsing config file\n", 1);
		else if(ac){
			process_args(ac, av, bconf);
			for(i=0;i<ac;i++){
				free(av[i]);
			}
			free(av);
		}
	}

	// now fill in all the data and insert into the ht
	if(line_buf[len-1]=='\n') line_buf[len-1]='\0';
	next_buddy->name=strdup(line_buf);
	next_buddy->conf=bconf;
	next_buddy->logins=ht_init(BUDDY_HT_SIZE, 
			bud_hash_login, bud_comp_login);

	free(line_buf);
	return next_buddy;
}
