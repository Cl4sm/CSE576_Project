	int i;

	struct option *opts=NULL;
	int longindex=0;
	char rbuff[31];

	bool usage=FALSE, verbosity=FALSE;

	char *data=NULL;
	int datafile=-1;
	int datalen=0;
	bool randomflag=FALSE;

	sendip_module *mod;
	int optc;

	int num_modules=0;

	sendip_data packet;
	
	num_opts = 0;	
	first=last=NULL;

	progname=argv[0];

	/* magic random seed that gives 4 really random octets */
	srandom(time(NULL) ^ (getpid()+(42<<15)));

	/* First, get all the builtin options, and load the modules */
	gnuopterr=0; gnuoptind=0;
	while(gnuoptind<argc && (EOF != (optc=gnugetopt(argc,argv,"-p:vd:hf:")))) {
		switch(optc) {
		case 'p':
			if(load_module(gnuoptarg))
				num_modules++;
			break;
		case 'v':
			verbosity=TRUE;
			break;
		case 'd':
			if(data == NULL) {
				data=gnuoptarg;
				if(*data=='r') {
					/* random data, format is r<n> when n is number of bytes */
					datalen = atoi(data+1);
					if(datalen < 1) {
						fprintf(stderr,"Random data with length %d invalid\nNo data will be included\n",datalen);
						data=NULL;
						datalen=0;
					}
					data=(char *)malloc(datalen);
					for(i=0;i<datalen;i++)
						data[i]=(char)random();
					randomflag=TRUE;
				} else {
					/* "normal" data */
					datalen = compact_string(data);
				}
			} else {
				fprintf(stderr,"Only one -d or -f option can be given\n");
				usage = TRUE;
			}
			break;
		case 'h':
			usage=TRUE;
			break;
		case 'f':
			if(data == NULL) {
				datafile=open(gnuoptarg,O_RDONLY);
				if(datafile == -1) {
					perror("Couldn't open data file");
					fprintf(stderr,"No data will be included\n");
				} else {
					datalen = lseek(datafile,0,SEEK_END);
					if(datalen == -1) {
						perror("Error reading data file: lseek()");
						fprintf(stderr,"No data will be included\n");
						datalen=0;
					} else if(datalen == 0) {
						fprintf(stderr,"Data file is empty\nNo data will be included\n");
					} else {
						data = mmap(NULL,datalen,PROT_READ,MAP_SHARED,datafile,0);
						if(data == MAP_FAILED) {
							perror("Couldn't read data file: mmap()");
							fprintf(stderr,"No data will be included\n");
							data = NULL;
							datalen=0;
						}
					}
				}
			} else {
				fprintf(stderr,"Only one -d or -f option can be given\n");
				usage = TRUE;
			}
			break;
		case '?':
		case ':':
			/* skip any further characters in this option
				this is so that -tonop doesn't cause a -p option
			*/
			nextchar = NULL; gnuoptind++;
			break;
		}
	}

	/* Build the getopt listings */
	opts = malloc((1+num_opts)*sizeof(struct option));
	if(opts==NULL) {
		perror("OUT OF MEMORY!\n");
		return 1;
	}
	memset(opts,'\0',(1+num_opts)*sizeof(struct option));
	i=0;
	for(mod=first;mod!=NULL;mod=mod->next) {
		int j;
		char *s;   // nasty kludge because option.name is const
		for(j=0;j<mod->num_opts;j++) {
			/* +2 on next line is one for the char, one for the trailing null */
			opts[i].name = s = malloc(strlen(mod->opts[j].optname)+2);
			sprintf(s,"%c%s",mod->optchar,mod->opts[j].optname);
			opts[i].has_arg = mod->opts[j].arg;
			opts[i].flag = NULL;
			opts[i].val = mod->optchar;
			i++;
		}
	}
	if(verbosity) printf("Added %d options\n",num_opts);

	/* Initialize all */
	for(mod=first;mod!=NULL;mod=mod->next) {
		if(verbosity) printf("Initializing module %s\n",mod->name);
		mod->pack=mod->initialize();
	}

	/* Do the get opt */
	gnuopterr=1;
	gnuoptind=0;
	while(EOF != (optc=getopt_long_only(argc,argv,"p:vd:hf:",opts,&longindex))) {
		
		switch(optc) {
		case 'p':
		case 'v':
		case 'd':
		case 'f':
		case 'h':
			/* Processed above */
			break;
		case ':':
			usage=TRUE;
			fprintf(stderr,"Option %s requires an argument\n",
					  opts[longindex].name);
			break;
		case '?':
			usage=TRUE;
			fprintf(stderr,"Option starting %c not recognized\n",gnuoptopt);
			break;
		default:
			for(mod=first;mod!=NULL;mod=mod->next) {
				if(mod->optchar==optc) {
					
					/* Random option arguments */
					if(gnuoptarg != NULL && !strcmp(gnuoptarg,"r")) {
						/* need a 32 bit number, but random() is signed and
							nonnegative so only 31bits - we simply repeat one */
						unsigned long r = (unsigned long)random()<<1;
						r+=(r&0x00000040)>>6;
						sprintf(rbuff,"%lu",r);
						gnuoptarg = rbuff;
					}

					if(!mod->do_opt(opts[longindex].name,gnuoptarg,mod->pack)) {
						usage=TRUE;
					}
				}
			}
		}
	}

	/* gnuoptind is the first thing that is not an option - should have exactly
		one hostname...
	*/
	if(argc != gnuoptind+1) {
 		usage=TRUE;
		if(argc-gnuoptind < 1) fprintf(stderr,"No hostname specified\n");
		else fprintf(stderr,"More than one hostname specified\n");
	} else {
		if(first && first->set_addr) {
			first->set_addr(argv[gnuoptind],first->pack);
		}
	}

	/* free opts now we have finished with it */
	for(i=0;i<(1+num_opts);i++) {
		if(opts[i].name != NULL) free((void *)opts[i].name);
	}
	free(opts); /* don't need them any more */

	if(usage) {
		print_usage();
		unload_modules(TRUE,verbosity);
		if(datafile != -1) {
			munmap(data,datalen);
			close(datafile);
			datafile=-1;
		}
		if(randomflag) free(data);
		return 0;
	}


	/* EVIL EVIL EVIL! */
	/* Stick all the bits together.  This means that finalize better not
		change the size or location of any packet's data... */
	packet.data = NULL;
	packet.alloc_len = 0;
	packet.modified = 0;
	for(mod=first;mod!=NULL;mod=mod->next) {
		packet.alloc_len+=mod->pack->alloc_len;
	}
	if(data != NULL) packet.alloc_len+=datalen;
	packet.data = malloc(packet.alloc_len);
	for(i=0, mod=first;mod!=NULL;mod=mod->next) {
		memcpy((char *)packet.data+i,mod->pack->data,mod->pack->alloc_len);
		free(mod->pack->data);
		mod->pack->data = (char *)packet.data+i;
		i+=mod->pack->alloc_len;
	}

	/* Add any data */
	if(data != NULL) memcpy((char *)packet.data+i,data,datalen);
	if(datafile != -1) {
		munmap(data,datalen);
		close(datafile);
		datafile=-1;
	}
	if(randomflag) free(data);

	/* Finalize from inside out */
	{
		char hdrs[num_modules];
		sendip_data *headers[num_modules];
		sendip_data d;

		d.alloc_len = datalen;
		d.data = (char *)packet.data+packet.alloc_len-datalen;

		for(i=0,mod=first;mod!=NULL;mod=mod->next,i++) {
			hdrs[i]=mod->optchar;
			headers[i]=mod->pack;
		}

		for(i=num_modules-1,mod=last;mod!=NULL;mod=mod->prev,i--) {

			if(verbosity) printf("Finalizing module %s\n",mod->name);

			/* Remove this header from enclosing list */
			hdrs[i]='\0';
			headers[i] = NULL;

			mod->finalize(hdrs, headers, &d, mod->pack);

			/* Get everything ready for the next call */
			d.data=(char *)d.data-mod->pack->alloc_len;
			d.alloc_len+=mod->pack->alloc_len;
		}
	}

	/* And send the packet */
	{
		int af_type;
		if(first==NULL) {
			if(data == NULL) {
				fprintf(stderr,"Nothing specified to send!\n");
				print_usage();
				free(packet.data);
				unload_modules(FALSE,verbosity);
				return 1;
			} else {
				af_type = AF_INET;
			}
		}
		else if(first->optchar=='i') af_type = AF_INET;
		else if(first->optchar=='6') af_type = AF_INET6;
		else {
			fprintf(stderr,"Either IPv4 or IPv6 must be the outermost packet\n");
			unload_modules(FALSE,verbosity);
			free(packet.data);
			return 1;
		}
		i = sendpacket(&packet,argv[gnuoptind],af_type,verbosity);
		free(packet.data);
	}
	unload_modules(FALSE,verbosity);

	return 0;
}
