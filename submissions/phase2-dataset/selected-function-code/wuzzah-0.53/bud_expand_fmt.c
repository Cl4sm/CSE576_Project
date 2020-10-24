char* bud_expand_fmt(const bud_utrec_t *bud, const char *fmt_string){
	int buf_fd[2], buf_size=0, amt=0, i, h_len=0;
	char *hname=NULL, *out_str=NULL;
	time_t tp;
	struct tm *tm_p;
	FILE *buf;
	struct hostent *hent;
	struct in_addr in;

	if(!bud || !fmt_string) return NULL;
	if(pipe(buf_fd)!=0){
		perror("unable to alloc pipe in bud_expand_fmt");
		return NULL;
	}
	buf=fdopen(buf_fd[1], "w");
	if(!buf){
		perror("unable to attach file ptr in bud_expand_fmt");
		return NULL;
	}

	tp=time(NULL);
	tm_p=localtime(&tp);

	for(i=0;i<strlen(fmt_string);i++){
		if(fmt_string[i]!='%') amt=fprintf(buf, "%c", fmt_string[i]);
		else if(i < strlen(fmt_string) - 1){
			i++;
			switch(fmt_string[i]){
			case 'a': amt=fprintf(buf, "\a"); break;
			case 'b': 
				amt=fprintf(buf, "%s", bud->u->ut_user); 
				break;
			case 'd':
				amt=fprintf(buf, "%02d:%02d:%02d", 
						tm_p->tm_hour, 
						tm_p->tm_min, tm_p->tm_sec);
				break;
			case 'h': 
				h_len=UTMPX_HOSTLEN(bud->u);
				if(h_len == 0){
					amt=fprintf(buf, "localhost");
				} else {
					hname=(char*)malloc(sizeof(char*)*(h_len+1));
					memcpy(hname, 
						UTMPX_HOSTNAME(bud->u), h_len);
					hname[h_len] = '\0';
					amt=fprintf(buf, "%s", hname);
					free(hname);
				}
				break;
			case 'H': 
				h_len=UTMPX_HOSTLEN(bud->u);
				if(h_len == 0){
					amt=fprintf(buf, "127.0.0.1");
				} else {
					hname=(char*)malloc(sizeof(char*)*(h_len+1));
					memcpy(hname, 
						UTMPX_HOSTNAME(bud->u), h_len);
					hname[h_len] = '\0';
					hent=gethostbyname(hname);
					if(!hent) amt=fprintf(buf, "n/a");
					else {
						memcpy(&in, 
							hent->h_addr_list[0],
							sizeof(struct in_addr));
						hname=inet_ntoa(in);
						amt=fprintf(buf, "%s", hname);
					}
					free(hname);
				}
				break;
			case 'l': 
				amt=fprintf(buf, "%-8s", bud->u->ut_line); 
				break;
			case 'm': 
				if(g_config.write_users) amt=fprintf(buf, "messaging");
				else amt=fprintf(buf, "not messaging");
				break;
			case 'n': amt=fprintf(buf, "\n"); break;
			case 'o': 
				if(bud->verified==B_LOGGED_IN)
					amt=fprintf(buf, "logged on ");
				else if(bud->verified==B_LOGGED_OUT)
					amt=fprintf(buf, "logged off");
				else amt=fprintf(buf, "bummin'");
				break;
			case 'u': amt=fprintf(buf, "%s", g_config.whoami); break;
			default: 
				amt=fprintf(buf, "%c", fmt_string[i]); 
				break;
			}
		}
		if(amt>0) buf_size+=amt;
		else if(amt==-1) perror("error writing to pipe");
	}
	if(!g_config.no_newline) {
		amt=fprintf(buf, "\n");
		if(amt>0) buf_size+=amt;
		else if(amt==-1) perror("error writing to pipe");
	}
	if(fclose(buf)!=0) perror("error closing pipe");

	out_str=(char*)malloc(sizeof(char)*(buf_size+2));
	amt=read(buf_fd[0], out_str, sizeof(char)*(buf_size));
	out_str[amt]='\0';
	if(close(buf_fd[0])!=0) perror("error closing pipe");
	return out_str;
}
