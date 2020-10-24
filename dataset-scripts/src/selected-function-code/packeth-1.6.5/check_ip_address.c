int check_ip_address(gchar *ptr)
{
	int i, j;
	gchar tmp[4];
	
	for (i=0; i<4; i++) {
		for(j=0; j<4 && (*ptr != '\0'); j++) {
			if ( ((int)*ptr == '.') && (i<3) && (j>0) ) {
				ptr++;
				break;
			}
			if ( (*ptr <48) || (*ptr>57) )
				return -1;
			else {
				tmp[j] = *ptr;
				ptr++;
			}
		}
		tmp[j] = '\0';
		if ( (atoi(tmp) < 0) || (atoi(tmp) > 255) || (strlen(tmp)==0) || (strlen(tmp)>3) )
			return -1;
	}
	return 1;
}