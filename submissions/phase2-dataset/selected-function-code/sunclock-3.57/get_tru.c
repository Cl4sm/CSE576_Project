int get_tru(char* pos)
{
	char* tmpp;
	int i = 1;

	tmpp = line_start + strlen(line_start+1)+1;
		if(line_start > pos || tmpp < pos) {
#ifdef DEBUG
			printf("Pos=%x, ls=%x, le=%x.\n",
					(unsigned int)pos,
					(unsigned int)line_start,
					(unsigned int)tmpp);
#else
			printf("get_tru\n");
			bell();
#endif /* DEBUG */
			return 0;
		}
		for(tmpp = line_start+1; tmpp < pos; tmpp++){
			if(*tmpp != '\t') {i++;}
			else {
				i = i + ((tabsize) - ((i-1)%tabsize));
			}
		}
		return(i);
}
