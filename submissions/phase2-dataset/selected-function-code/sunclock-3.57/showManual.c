void 
showManual()
{
int i=0, j, k, l, c, cp;
char cmd[256];
char tmpname[80];
struct stat buf;
char *ptr, *text;
FILE *man;

     secure_tmpnam(tmpname);
     strcat(tmpname, "_sunclock.man");
     sprintf(cmd, "man sunclock > %s", tmpname);
     system(cmd);
     man = fopen(tmpname, "r");
     if (!man) {
        unlink(tmpname);
	return;
     }
     ptr = strdup(tmpname);
     buf.st_mode = 0;
     stat(ptr, &buf);
     free(ptr);
     text = malloc(buf.st_size+2);
     if (!text) {
        fclose(man);
        return;
     }
     while ((c=fgetc(man))!=EOF) {
       iter:
        if (c=='_') {
	   cp = fgetc(man);
	   if (cp==EOF) break;
	   if (cp==8) continue;
	   text[i++] = (char)c;
	   c = cp;
           goto iter;
	} else
	if (c==8) {
	   c = fgetc(man);
	   if (c==EOF) break;
	} else
	   text[i++] = (char)c;
     }
     text[i] = '\0';
     fclose(man);
     man = fopen(tmpname, "w");
     if (!man) {
        free(text);
        return;
     }
     for (j=0; j<i; j++) {
         if (text[j]=='\n' && !strncmp(text+j+1, "Sunclock-", 6)) {
	    k = j;
	    while (isspace(text[k])) k--;
	    while (text[k]!='\n') k++;
	    ptr = strstr(text+j+1, "sunclock(1x)");
	    if (ptr) ptr = strstr(ptr+2, "sunclock(1x)");
	    if (ptr) {
               j = ptr-text+1;
	       while (text[j]!='\n') j++;
	       while (isspace(text[j])) j++;
	       while (text[j]!='\n') j--;
	       for (l=k; l<j; l++) text[l] = '\0';
	    } else
	       for (l=k+3; l<j; l++) text[l] = '\0';
	 }
     }
     for (j=0; j<i; j++) {
         if (text[j]) fputc(text[j], man);
     }
     free(text);
     fflush(man);
     fclose(man);
     chmod(tmpname, 0555);
     sprintf(cmd, "(%s %s ; rm -f %s ) &", 
	     EditorCommand, tmpname, tmpname);
     system(cmd);
}
