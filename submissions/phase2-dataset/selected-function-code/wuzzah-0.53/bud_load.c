	char *line_buf=NULL;
	int line_buf_size=4, buf_offset=0, len=0;
	buddy_t *next_buddy=NULL;
	wuzzah_config_t *bconf=NULL;
	FILE *buds=NULL;

	// initialize the hashtable of buddies

	if (g_config.noloadfile) return 0;
	else if(strcmp(fn, "-")==0) buds=stdin;
	else buds=fopen(fn, "r");

	if(!buds){
		fprintf(stderr, "unable to open %s! (%s)\n", fn,
				strerror(errno));
		next_buddy=bud_create_buddy(g_config.whoami);
		fprintf(stderr, "defaulting to just watching yourself...\n");
		ht_insert(next_buddy, ht);
		return 0;
	}

	line_buf=(char*)malloc(sizeof(char)*line_buf_size);
	while(!feof(buds)){
		// first make sure we get a whole line
		if(!fgets(line_buf+buf_offset, line_buf_size-len, buds))break;
		len=strlen(line_buf);
		if(strcspn(line_buf, "\n") == len && !feof(buds)){
			buf_offset=len;
			line_buf_size *= 2;
			line_buf=(char*)realloc(line_buf, line_buf_size);
			continue;
		}
	
		next_buddy=bud_create_buddy(line_buf);
		if(ht_find(next_buddy, ht)){
			fprintf(stderr, 
					"%s: warning: duplicate buddy %s\n",
					g_config.progname, next_buddy->name);
		} else ht_insert(next_buddy, ht);

		// reset some variables
		len=buf_offset=0;
		bconf=NULL;
	}

	return 0;
}
