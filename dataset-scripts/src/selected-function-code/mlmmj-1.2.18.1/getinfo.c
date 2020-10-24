void getinfo(const char *line, struct mailhdr *readhdrs)
{
	int i = 0;
	size_t tokenlen, linelen, valuelen;

	while(readhdrs[i].token) {
		tokenlen = strlen(readhdrs[i].token);
		linelen = strlen(line);
		if(strncasecmp(line, readhdrs[i].token, tokenlen) == 0) {
			readhdrs[i].valuecount++;
			valuelen = linelen - tokenlen + 1;
			readhdrs[i].values =
				(char **)myrealloc(readhdrs[i].values,
				  readhdrs[i].valuecount * sizeof(char *));
			readhdrs[i].values[readhdrs[i].valuecount - 1] =
					(char *)mymalloc(valuelen + 1);
			strncpy(readhdrs[i].values[readhdrs[i].valuecount - 1],
						line+tokenlen, valuelen);
			chomp(readhdrs[i].values[readhdrs[i].valuecount - 1]);
		}
		i++;
	}
}