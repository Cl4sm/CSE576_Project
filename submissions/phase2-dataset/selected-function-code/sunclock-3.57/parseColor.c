int
parseColor(s)
char * s;
{
int i, l, done = 0;
char *ptr = NULL;

    l = strlen(s);
    for (i=0; i<l; i++) {
        if (s[i]=='|') {
	   s[i] = '\0';
	   ptr = s+i+1;
	   break;
	}
    }
    l = i;
	
    if (ptr) {
       for (i=0; i<NUMPIXELS; i++)
          if (!strcasecmp(s, colorfield[i])) {
             StringReAlloc(&Color[i], ptr);
             done = 1;
	     break;
          }
       s[l] = '|';
    }

    if (!ptr || !done) {
       fprintf(stderr, "Incorrect -setcolor specification\n");
       return 1;
    }
    return 0;
}
