next_token(char *word, char **next)
{
    char *ptr;
    char *ret, *t;
    int state, ctype;

    t = ret = malloc(strlen(word)+1);
    ptr = word;
    
    state = 0;
    *t = 0;
    while (1) {
	if (*ptr==0) 
	    ctype = PRC_EOS;
	else if (*ptr=='\\')
	    ctype = PRC_ESCAPE;
	else if (*ptr=='"')
	    ctype = PRC_DQUOTE;
	else if (*ptr=='\'')
	    ctype = PRC_SQUOTE;
	else if (*ptr==' ' || *ptr=='\t')
	    ctype = PRC_BLANK;
	else
	    ctype = PRC_ALPHA;

	if (mtable[state][ctype].output) {
	    *t = *ptr; t++;
	    *t = 0;
	}
	state = mtable[state][ctype].nstate;
	ptr++;
	if (mtable[state][0].output<0) {
	    break;
	}
    }

    if (*ret==0)
	t = NULL;
    else
	t = strdup(ret);

    free(ret);
    
    if (ctype==PRC_EOS)
	*next = NULL;
    else
	*next = ptr;
    
    return t;
}
