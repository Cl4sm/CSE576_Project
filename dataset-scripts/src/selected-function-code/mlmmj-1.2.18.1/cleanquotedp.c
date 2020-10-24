char *cleanquotedp(const char *qpstr)
{
	char *retstr;
	char qc[3];
	const char *c = qpstr;
	long qcval;
	int i = 0;
	size_t len;

	/* XXX: We only use this function for checking whether the subject
	 * prefix is only present, so the recoding is neither guaranteed
	 * complete nor correct */

	len = strlen(qpstr);
	retstr = mymalloc(len + 1);
	retstr[len] = '\0';
	qc[2] = '\0';
	while(c < qpstr+len) {
		switch(*c) {
			case '=':
				c++;
				if (!isxdigit(*c))
					break;
				qc[0] = *(c++);
				if (!isxdigit(*c))
					break;
				qc[1] = *(c++);
				qcval = strtol(qc, NULL, 16);
				if(qcval)
					retstr[i++] = (char)qcval;
				break;
			case '_':
				retstr[i++] = ' ';
				c++;
				break;
			default:
				retstr[i++] = *(c++);
				break;
		}
	}
	
	retstr[i] = '\0';

	return retstr;
}