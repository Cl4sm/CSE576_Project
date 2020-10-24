char *chomp(char *str)
{
	size_t i;

	if(str == NULL)
		return NULL;

	if(*str == '\0' || *str == '\n' || *str == '\r') {
		*str = '\0';
		return str;
	}

	i = strlen(str) - 1;

	while((str[i] == '\n') || (str[i] == '\r')) {
		str[i] = 0;
		i--;
	}

	return str;
}