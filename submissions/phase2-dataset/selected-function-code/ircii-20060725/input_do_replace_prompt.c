input_do_replace_prompt(u_char *newprompt)
{
	ScreenInputBufferData* bufdata = &current_screen->inputdata.buffer;
	u_char* buf  = bufdata->buf;
	unsigned oldlen = bufdata->minpos;
	unsigned newlen = my_strlen(newprompt);
	unsigned max = sizeof(bufdata->buf);
	unsigned saved_len = max - (oldlen > newlen ? oldlen : newlen);
/*
	fprintf(stderr, "oldlen=%u, newlen=%u, max=%u, saved_len=%u\n",
		oldlen,newlen, max, saved_len);
*/
	memmove(buf+newlen,
	        buf+oldlen,
	        saved_len);
	memcpy(buf,
	       newprompt,
	       newlen);
	buf[max-1] = '\0'; /* prevent dragons */
	
	bufdata->minpos = newlen;
	bufdata->pos	= bufdata->pos - oldlen + newlen;
	
	if (bufdata->pos < newlen)
		bufdata->pos = newlen;	
}
