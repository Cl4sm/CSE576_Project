void read_rc()
{
	FILE *fr = NULL;
	char name[NLEN], *ptr;

	if ((ptr=getenv("HOME"))) {
		if (RcFile) {
			if (ptr && *RcFile == '~' && RcFile[1] == '/')
				sprintf(name, "%s/%s", ptr, RcFile+2);
			else
				strcpy(name, RcFile);
		}
		else
			sprintf(name, "%s/.%s", ptr, DEFAULT_RC);
		fr = fopen(name, "r");
	}

	if (!fr) {
		sprintf(name, "%s/%s", SHAREDIR, DEFAULT_RC);
		fr = fopen(name, "r");
	}

	if (fr) {
		parse_rc(fr);
		fclose(fr);
	}
}
