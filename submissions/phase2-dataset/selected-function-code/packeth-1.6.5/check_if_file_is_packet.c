int check_if_file_is_packet(FILE *file_p)
{
	int c, i=0;	
	gboolean first = 1;

	while ( (c = fgetc( file_p )) != EOF ) {
		if (first ==1) {
			if (isspace(c) != 0) 
				continue;
			if (c == 35) {
				while ( getc(file_p) != 10);
				continue;
			}
		}
		if (isxdigit(c) == 0) {
			//printf("Error: File does not contain a valid packet!\n");
			error("Error: File does not contain a valid packet");
			return -1;
		}

		if (first == 1)
			first = 0;
		else
			first = 1;
		i++;
	}
	
	/* 1514 or 1518, how to enable the vlan checking */
	if ( (i%2 != 0) || (i > 3536) )	{
		//printf("Error: File length is not ok\n");
		error("Error: File length is not ok");
		return -1;
	}

	return i;
}