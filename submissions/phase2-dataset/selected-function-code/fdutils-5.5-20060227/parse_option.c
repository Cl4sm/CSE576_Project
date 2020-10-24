			 char *name)
{
	char *end;
	char *nextname;
	int number;
	int i, iterations;
	int flags;
	double d;
	float f;
	
	nextname = name - 1;

	if ( eo->type & EO_TYPE_LIST )
		iterations = eo->arg;
	else
		iterations = 1;

	for( i=0; i < iterations; i++ ){
		if ( !nextname )
			break;
		name = nextname+1;
		if ( eo->type & EO_TYPE_LIST )
			nextname = strchr(name, ',' );
		else
			nextname = 0;

		switch(eo->type & EO_DTYPE){
		case EO_TYPE_NONE:
			return 0;
		case EO_TYPE_STRING:
			if ( nextname )
				*nextname='\0';
			*((char **) eo->address) = name;
			if ( nextname)
				*nextname=',';
			continue;			
		}

		if (eo->has_arg){
			if(!*name ) /* last field is empty */
				break;
			if ( name == nextname ) /* field to be skipped */
				continue;
		}

		switch(eo->type & EO_DTYPE){
		case EO_TYPE_FLOAT:
			if( sscanf(name, "%f", &f ) != 1 ){
				fprintf(stderr,"%s is not a float\n", name);
				return -1;
			}
			((float *) eo->address)[i] = f;
			continue;
		case EO_TYPE_DOUBLE:
			if ( sscanf(name, "%lf", &d) != 1 ){
				fprintf(stderr,"%s is not a double\n", name);
				return -1;
			}
			((double *) eo->address)[i]=d;
			continue;
		case EO_TYPE_CHAR:
			*((char *) eo->address ) = name[0];
			continue;
		case EO_TYPE_BITMASK_BYTE:
			if ( eo->arg >= 0 )
				((char *) eo->address)[i] |= eo->arg & 0xff;
			else
				((char *) eo->address)[i] &= eo->arg;
			return 0;
		case EO_TYPE_BITMASK_SHORT:
			if ( eo->arg >= 0 )
				((short *) eo->address)[i] |= eo->arg & 0xff;
			else
				((short *) eo->address)[i] &= eo->arg;
			return 0;
		case EO_TYPE_BITMASK_LONG:
		if ( eo->arg >= 0 )
			((long *) eo->address)[i] |= eo->arg & 0x7fff;
		else
			((long *) eo->address)[i] &= eo->arg;
			return 0;
		}

		if ( ! eo->has_arg ) 
			number = eo->arg;
		else
			number = strtoul(name, &end, 0 );
		
		switch( eo->type & EO_DTYPE){
		case EO_TYPE_FILE_OR_FD:
			if ((!*end || end == nextname) && number >= 0){
				((int *) eo->address)[i] = number;
				continue;
			}
			/* fall through */
		case EO_TYPE_FILE:
			if (nextname)
				*nextname='\0';
			if ( eo->type & EO_TYPE_LIST )
				flags = O_RDONLY;
			else
				flags = eo->arg;
			number = open(name, flags, 0644 );
			if (nextname)
				*nextname=',';
			if ( number < 0 ){
				perror(name);
			}
			((int *) eo->address)[i] = number;
			continue;
		}

		if (eo->has_arg && end != nextname && *end){
			fprintf(stderr,"%s is not a number\n", name);
			return -1;
		}

		switch(eo->type & EO_DTYPE){
		case EO_TYPE_BYTE:
			((char *) eo->address)[i] = number;
			continue;
		case EO_TYPE_SHORT:
			((short *) eo->address)[i] = number;
			continue;
		case EO_TYPE_LONG:
			((long *) eo->address)[i] = number;
			continue;
		default:
			fprintf(stderr,
				"Bad option type in parse_option for %s\n",
				eo->longo);
			exit(1);
		}
	}
	if ( eo->type & EO_TYPE_LIST )
		eo->arg = i;
	return 0;
}
