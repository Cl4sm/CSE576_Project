{
	int i, iterations;
	long number;
	
	if ( eo->type & EO_TYPE_LIST )
		iterations = eo->arg;
	else
		iterations = 1;

	if ( eo->has_arg ){
		printf("%s=", eo->longo);
		for(i=strlen(eo->longo); i<wordlength;  i++)
			putchar(' ');
	}

	for(i=0; i < iterations; i++){
		if ( i )
			printf(",");
		switch( eo->type & EO_DTYPE ){
		case EO_TYPE_BITMASK_BYTE:
			if ( eo->arg >= 0 &&
			    ((char *) eo->address)[i] & eo->arg)
				printf("%s\n", eo->longo);
			return;
		case EO_TYPE_BITMASK_SHORT:
			if ( eo->arg >= 0 &&
			    ((short *) eo->address)[i] & eo->arg)
				printf("%s\n", eo->longo);
			return;
		case EO_TYPE_BITMASK_LONG:
			if ( eo->arg >= 0 &&
			    ((long *) eo->address)[i] & eo->arg)
				printf("%s\n", eo->longo);
			return;
		}

		switch( eo->type & EO_DTYPE ){
		case EO_TYPE_FLOAT:
			printf("%f", ((float *) eo->address)[i] );
			continue;
		case EO_TYPE_DOUBLE:
			printf("%f", ((double *) eo->address)[i] );
			continue;
		case EO_TYPE_STRING:
			printf("%s", ((char **) eo->address)[i] );
			continue;
		case EO_TYPE_FILE:
		case EO_TYPE_FILE_OR_FD:
			number = ((int *) eo->address)[i];
			if ( number < 0 )
				printf("<closed>");
			else
				printf("%%%ld", number);
			continue;
		}

		switch( eo->type & EO_DTYPE ){
		case EO_TYPE_BYTE:
			number = ((char *) eo->address)[i] ;
			break;
		case EO_TYPE_SHORT:
			number = ((short *) eo->address)[i] ;
			break;
		case EO_TYPE_LONG:
			number = ((long *) eo->address)[i] ;
			break;
		case EO_TYPE_CHAR:
			number = ((char *) eo->address)[i] ;
			break;
		default:
			printf("??\n");
			return;
		}
		if ( eo->has_arg ){
			if ( (eo->type & EO_DTYPE ) ==
			    EO_TYPE_CHAR )
				printf("%c", (char) number);
			else
				printf("%ld", number);
		} else {
			if ( eo->arg == number )
				printf("%s", eo->longo );
			else return;
		}
	}
	printf("\n");
}
