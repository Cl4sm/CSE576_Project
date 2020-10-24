void parse_arg(char *arg1, char *arg2)
{
	if(*arg1=='-') {
		if (!strcasecmp(arg1, "-f"))
			strcpy(ewin.name, arg2);
		else
		if (!strcasecmp(arg1, "-t"))
			tabsize = atoi(arg2);
		else
		if (!strcasecmp(arg1, "-j"))
			ewin.jump = atoi(arg2);
		else
		if (!strcasecmp(arg1, "-w")) {
			screen_width = atoi(arg2);
			if (screen_width<20) screen_width = 20;
			if (screen_width>180) screen_width = 180;
		}
		else
		if (!strcasecmp(arg1, "-h")) {
			screen_height = atoi(arg2);
			if (screen_height<5) screen_height = 5;
			if (screen_height>80) screen_height = 80;
		}
		else
		if (!strcasecmp(arg1, "-fn"))
			FontName = strdup(arg2);
		else
		if (!strcasecmp(arg1, "-rc"))
			RcFile = strdup(arg2);
		else
		if (!strcasecmp(arg1, "-fg"))
	        	FgColor = strdup(arg2);
		else
		if (!strcasecmp(arg1, "-bg"))
			BgColor = strdup(arg2);
		else
		if (!strcasecmp(arg1, "-hifg"))
			HiFgColor = strdup(arg2);
		else
		if (!strcasecmp(arg1, "-hibg"))
			HiBgColor = strdup(arg2);
		else
		if (!strcasecmp(arg1, "-cr"))
			CrColor = strdup(arg2);
		else
		if (!strcasecmp(arg1, "-edit"))
		        flag[EDT] = atoi(arg2);
	}
#ifndef MINIMAL
	if(*arg1=='@') {
		char *ptr;
		int i;
		i = tolower(arg1[1])-'a';
		if (i>=0 && i<26) {
			ptr = arg2;
			while(isspace(*ptr)) ++ptr;
			if (binding[i]) free(binding[i]);
			binding[i] = strdup(ptr);
		}
	}
#endif /* MINIMAL */
}
