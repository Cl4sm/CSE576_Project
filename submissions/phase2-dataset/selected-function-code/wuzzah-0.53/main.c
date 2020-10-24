int main(int argc, char *argv[]){
	htable_t *ht=NULL;
	process_args(argc, argv, &g_config);

	ht=bud_init_budtable();
	wuzzah_whoami();
	if(g_config.all_users){
		bud_load_every_user(ht);
	} else {
		if(g_config.extrabuddies)
			wuzzah_add_extrabuddies(g_config.extrabuddies, ht);
		if(!g_config.noloadfile) bud_load(g_config.infile, ht);
	}

	if(ht->size==0){
		fprintf(stderr, 
			"%s: no buddies specified\n", g_config.progname);
		fprintf(stderr, "well, if you don't have any friends for\n");
		fprintf(stderr, "me to watch, I'm going away!\n");
		exit(1);
	}

	// let's do it once first, and not spam everyone when
	// we start up (hence 0).
	if(!g_config.process_current) bud_chk_utmpx(ht, -1);
	else bud_chk_utmpx(ht, g_config.write_users);
	// if we were only supposed to go once
	if(g_config.run_once) return 0;
	// otherwise...
	while(!sleep(g_config.sleep_interval)){
		bud_chk_utmpx(ht, g_config.write_users);
	}

	return 0;
}
