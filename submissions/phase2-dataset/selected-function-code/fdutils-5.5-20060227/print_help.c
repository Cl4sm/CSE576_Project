{
	int i, wordlength, htlength, col;
	char *ptr1, *ptr2;
	char format[20];

	wordlength = get_wordlength(eo);
	snprintf(format, 19, "--%%-%ds ", wordlength);
	htlength=80 - wordlength - 6;
	for(i=0; i<length; i++){
		if ( eo[i].shorto )
			printf("-%c ", eo[i].shorto );
		else
			printf("   ");
		printf(format, eo[i].longo);
		ptr1=eo[i].helptext;
		if (!ptr1){
			putchar('\n');
			continue;
		}
		col=wordlength+6;
		while(1){
			/* first print the first word */
			while (*ptr1 && *ptr1 != ' ' && col < 80){
				col++;
				putchar(*ptr1++);
			}
			ptr2=ptr1;
			while (1){
				if ( !*ptr2 || *ptr2 == ' ' )
					while( ptr1 < ptr2 )
						putchar(*ptr1++);
				if ( col == 80 || !*ptr2 ){
					putchar('\n');
					ptr2 = ptr1;
					break;
				}
				if ( *ptr1 == ' ' )
					putchar(*ptr1++);
				col++;
				ptr2++;
			}
			if ( !*ptr1 )
				break;
				
			if (col == 80 ){
				for(col=0; col<wordlength+6; col++)
					putchar(' ');
				while(*ptr1 == ' ' )
					ptr1++;
				ptr2=ptr1+80;
			}
		}
	}
}
