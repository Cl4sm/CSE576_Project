struct email_container *find_email_adr(const char *str,
		struct email_container *retstruct)
{
	char *c1 = NULL, *c2 = NULL;
	char *p;
	char *s;

	s = (char *)mymalloc(strlen(str) + 1);
	strcpy(s, str);

	p = s;
	while(p) {
		char *adr;
		char *cur;

		cur = p;
oncemore:
		p = strchr(p, ',');
		if (p) {
			/* If there's a comma, replace it with a NUL, so
			 * cur will only have one address in it. Except
			 * it's not in ""s */
			c1 = strchr(cur, '"');
			if(c1) {
				c2 = strchr(c1+1, '"');
			}
			if(c2) {
				if(*(c2-1) == '\\') {
					*c2 = ' ';
					c2 = NULL;
					goto oncemore;
				}
			}
			if((c1 == NULL) || (c1 > p) || (c2 && c2 < p)) {
				*p = '\0';
				p += 1;
			} else {
				*p = ' ';
				goto oncemore;
			}
		}

		while(cur && ((' ' == *cur) ||
			    ('\t' == *cur) ||
			    ('\r' == *cur) ||
			    ('\n' == *cur))) {
			cur += 1;
		}
		if ('\0' == *cur) {
			continue;
		}

		adr = skin(cur);
		if (adr) {
			retstruct->emailcount++;
			retstruct->emaillist = (char **)myrealloc(retstruct->emaillist,
					  sizeof(char *) * retstruct->emailcount);
			retstruct->emaillist[retstruct->emailcount-1] = adr;
		}
	}

	myfree(s);

	return retstruct;
}