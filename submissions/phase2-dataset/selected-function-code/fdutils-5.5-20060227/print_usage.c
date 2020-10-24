{
	int i;
	
	fprintf(stderr,"Usage: %s ", progname);
	for (i=0; i<length; i++){
		fprintf(stderr,"[-");
		if (eo[i].shorto )
			fprintf(stderr,"%c",eo[i].shorto);
		else
			fprintf(stderr,"-%s",eo[i].longo);
		if ( eo[i].has_arg ){
			if( eo[i].type & EO_TYPE_LIST )
				fprintf(stderr," list_of_");
			else
				fprintf(stderr," ");
			switch(eo[i].type){
			case EO_TYPE_CHAR:
				fprintf(stderr,"char");
				break;
			case EO_TYPE_STRING:
				fprintf(stderr,"string");
				break;
			case EO_TYPE_FLOAT:
				fprintf(stderr,"float");
				break;
			case EO_TYPE_DOUBLE:
				fprintf(stderr,"double");
				break;
			case EO_TYPE_BYTE:
				fprintf(stderr,"byte");
				break;
			case EO_TYPE_SHORT:
				fprintf(stderr,"short");
				break;
			case EO_TYPE_LONG:
				fprintf(stderr,"long");
				break;
			case EO_TYPE_FILE:
			case EO_TYPE_FILE_OR_FD:
				fprintf(stderr,"file");
				break;
			}
		}
		fprintf(stderr,"] ");
	}
	fprintf(stderr,"%s\n", userparams);
}
