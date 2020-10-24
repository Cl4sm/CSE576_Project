{
	int i;
	char *ptr;

	/* first count them */
	for( i=0; eo[i].longo; i++);
	length = i;
	
	my_longopts = (struct option *)calloc(length+2, sizeof(struct option));
	if ( my_longopts == 0 ){
		fprintf(stderr,"out of memory\n");
		exit(1);
	}
	for(i=0; i<length; i++){
		my_longopts[i].val = eo[i].shorto;
		my_longopts[i].name = eo[i].longo;
		my_longopts[i].has_arg = eo[i].has_arg;
		my_longopts[i].flag = 0;		
	}
	my_longopts[length].val = 'h';
	my_longopts[length].name = "help";
	my_longopts[length].has_arg = 0;
	my_longopts[length].flag = 0;
	my_longopts[length+1].name = 0;

	my_xopts = (char **) calloc(length, sizeof(char *));
	if ( !my_xopts){
		fprintf(stderr,"out of memory\n");
		exit(1);
	}
	for (i=0; i<length; i++)
 		my_xopts[i]= 0;

	ptr = my_shortopts = (char *)malloc( 2 * length + 2 );
	if ( !ptr ){
		fprintf(stderr,"out of memory\n");
		exit(1);
	}
	for ( i=0; i<length; i++){
		if ( !eo[i].shorto )
			continue;		
		*ptr++ = eo[i].shorto;
		if ( eo[i].has_arg )
			*ptr++ = ':' ;
	}
	*ptr++='h';
	*ptr='\0';
}
