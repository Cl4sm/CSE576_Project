static void parse_env(void)
{
	const char *str = getenv("LIBTRACEIO");
	char option[1024];
	const char *ip;
	char *op;

	if (!str)
		return;

	for(ip=str, op=option; *ip!='\0' && op < option+sizeof(option); ++ip) {
		if (*ip == ',') {
			*op='\0';
			do_option(option);
			op=option;
		}
		else
			*(op++) = *ip;
	}
	*op='\0';
	do_option(option);
}