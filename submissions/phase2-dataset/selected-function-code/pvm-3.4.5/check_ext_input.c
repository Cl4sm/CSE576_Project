check_ext_input()
{
	char input[256];

	char *ptr, *ptr2;

	char c;

	int flags;
	int i;

	/* Set stdin Non-Blocking */

	flags = fcntl( 0, F_GETFL, 0 );

	fcntl( 0, F_SETFL, flags | O_NDELAY );

	/* Test for Input */

	i = 0;

	while ( (int)(input[i] = getc( stdin )) != (char) EOF
			&& input[i] != '\n' && i < 255 )
		i++;

	input[i] = '\0';

	/* Parse Extension Options Here */

	if ( i ) {
		ptr = input;
		while ( *ptr != '\0' )
		{
			/* Virtual Machine ID */
			if ( !strncmp( "PVM_VMID=", ptr, 9 ) ) {
				ptr2 = ptr + 9;
				while ( *ptr2 != ' ' && *ptr2 != '\t' && *ptr2 != '\0' )
					ptr2++;
				c = *ptr2;
				*ptr2 = '\0';
				pvmputenv(STRALLOC(ptr));
				*ptr2 = c;
				ptr = ptr2;
			}

			/* Move Past Unknown Options */
			else
				while ( *ptr != ' ' && *ptr != '\t' && *ptr != '\0' )
					ptr++;

			/* Advance to Next Option */
			while ( *ptr == ' ' || *ptr == '\t' )
				ptr++;
		}
	}

	/* Reset stdin to Blocking */

	fcntl( 0, F_SETFL, flags );
}
