void wuzzah_add_extrabuddies(const char *line, htable_t *ht){
	int i=0, len=strlen(line);
	const char *head=line;
	char *buf=NULL;

	buf=(char*)malloc(sizeof(char)*(strlen(line)+1));
	while((int)(head-line)<len){
		i=strcspn(head, " \t\n,:");
		if(i>0) {
			strncpy(buf, head, i);
			buf[i]='\0';
		} else {
			strncpy(buf, head, strlen(head));
			buf[strlen(head)]='\0';
		}
		ht_insert(bud_create_buddy(buf), ht);
		head=&head[i+1];
	}
	free(buf);
}
