void *
pvmcompileregex( pattern )
char *pattern;
{
	struct re_pattern_buffer *pattbuff;

	char *patterr;

	pattbuff = (struct re_pattern_buffer *) malloc(
		sizeof( struct re_pattern_buffer ) );

	pattbuff->translate = (char *) NULL;
	pattbuff->fastmap = (char *) NULL;
	pattbuff->buffer = (unsigned char *) NULL;
	pattbuff->allocated = (long) 0;

	patterr = (char *) re_compile_pattern( pattern,
			strlen( pattern ), pattbuff );

	if ( patterr ) {
		printf( "Error Compiling Pattern \"%s\":\n\t%s\n",
				pattern, patterr );

		return( (void *) NULL );
	}

	return( (void *) pattbuff );
}
