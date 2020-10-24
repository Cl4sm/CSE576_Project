{
	int i;

	while (mk+blen >= edbuf+amax && realloc_buffer(0));
	if (mk+blen < edbuf+amax) 
		memcpy(mk, bb, blen);
	else {
		if(fb == 0) return;
		fseek(fb, 0L, 0);		/* 0L offset, 0=beginning of file */
		fread(mk, 1, blen, fb);
	}
	/* calculate ytot */
	for(i=0; i<blen; i++) {
		if((mk[i] == EOL) || (mk[i] == LF)) {
			ytot++;
			mk[i] = 0;
		}
	}
}
