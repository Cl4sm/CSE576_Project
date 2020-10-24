parseFont(s)
char * s;
{
int i, l1 = 0, l2 = 0, done;
char *ptr1 = NULL, *ptr2 = NULL;

    done = strlen(s);
    for (i=0; i<done; i++) {
        if (s[i]=='|') {
	   s[i] = '\0';
	   if (!ptr1) {
	      ptr1 = s+i+1;
	      l1 = i;
	   } else {
	      ptr2 = s+i+1;
	      l2 = i;
  	      break;
	   } 
	}
    }
   
    /* Do not take font setting into account if selected language is
       not specified in list of languages */
    if (ptr2 && !strstr(ptr2, language)) return 0;
	
    if (!strcasecmp(s, "menu") && ptr1 && strcmp(ptr1, SunFont[4]))
       option_changes |= 1;
	
    done = 0;
    if (ptr1) {
       for (i=0; i<NUMFONTS; i++)
          if (!strcasecmp(s, fontfield[i])) {
             StringReAlloc(&SunFont[i], ptr1);
             done = 1;
	     break;
          }
       s[l1] = '|';
       if (l2) s[l2] = '|';
    }

    if (!ptr1 || !done) {
       fprintf(stderr, "Incorrect -setfont specification\n");
       return 1;
    }

    return 0;
}
