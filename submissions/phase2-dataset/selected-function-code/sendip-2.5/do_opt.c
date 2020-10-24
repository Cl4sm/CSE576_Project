	ripng_header *rippack = (ripng_header *)pack->data;
	ripng_entry *ripopt;
	char *p, *q;
	switch(opt[1]) {
	case 'v': /* version */
		rippack->version = (u_int8_t)strtoul(arg, (char **)0, 0);
		pack->modified |= RIPNG_MOD_VERSION;
		break;
	case 'c': /* command */
		rippack->command = (u_int8_t)strtoul(arg, (char **)0, 0);
		pack->modified |= RIPNG_MOD_COMMAND;
		break;
	case 'r': /* reserved */
		rippack->res = htons((u_int16_t)strtoul(arg, (char **)0, 0));
		pack->modified |= RIPNG_MOD_RESERVED;
		break;
		/*
	case 'a': / * authenticate * /
		if(RIPNG_NUM_ENTRIES(pack) != 0) {
			usage_error("Warning: a real RIP-2 packet only has authentication on the first entry.\n");
		}
		pack->modified |= RIP_IS_AUTH;
		pack->data = realloc(pack->data,pack->alloc_len+strlen(arg));
		strcpy(pack->data+pack->alloc_len,arg);
		pack->alloc_len += strlen(arg);
		break;
		*/
	case 'e': /* rip entry */
		RIPNG_ADD_ENTRY(pack);
		ripopt = RIPNG_ENTRY(pack);
		p=q=arg;
		/* TODO: if arg is malformed, this could segfault */
		while(*(q++)!='/') /* do nothing */; *(--q)='\0';
		ripopt->prefix = (p==q)?in6addr_any:inet6_addr(p);

		p=++q; while(*(q++)!='/') /* do nothing */; *(--q)='\0';
		ripopt->tag=htons( (p==q)?0:(u_int16_t)strtoul(p, (char **)0,0));

		p=++q; while(*(q++)!='/') /* do nothing */; *(--q)='\0';
		ripopt->len=(p==q)?(u_int8_t)128:(u_int8_t)strtoul(p, (char **)0,0);

		p=++q; while(*(q++)!='\0') /* do nothing */; *(--q)='\0';
		ripopt->metric=(p==q)?(u_int8_t)16:(u_int8_t)strtoul(p,(char **)0, 0);
		break;
	case 'd': /* default request */
		if(RIPNG_NUM_ENTRIES(pack) != 0) {
			usage_error("Warning: a real RIPng packet does not have any other entries in a default request.\n");
		}
		rippack->command = (u_int8_t)1;
		rippack->version = (u_int8_t)1;
		rippack->res = (u_int16_t)0;
		pack->modified|=RIPNG_MOD_COMMAND|RIPNG_MOD_VERSION|RIPNG_MOD_RESERVED;
		RIPNG_ADD_ENTRY(pack);
		ripopt=RIPNG_ENTRY(pack);
		ripopt->prefix=in6addr_any;
		ripopt->tag=(u_int16_t)0;
		ripopt->len=(u_int8_t)0;
		ripopt->metric=htons((u_int16_t)16);
		break;
	}
	return TRUE;

}
