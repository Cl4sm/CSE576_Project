__dead void
usage(void)
{
	extern char version[];
	char usage[] =
		"[-dN] "
		/**/
		/**/
		"[-f datafile] "
		/**/
		/**/
		"[-i interface] "
		/**/
		/**/
		"[-n net[/width]] "
		/**/
		/**/
		"[-r file] "
		/**/
		/**/
		"[-s sendmail_path] "
		/**/
		/**/
		"[-p] "
		/**/
		/**/
		"[-a] "
		/**/
		/**/
		"[-m addr] "
		/**/
		/**/
		"[-u username] "
		"[-R seconds ] "
		/**/
		/**/
		"[-Q] "
		/**/
		/**/
		"[-z ignorenet/ignoremask] "
		/**/
		/**/
		"\n"
	;

	(void)fprintf(stderr, "Version %s\n", version);
	(void)fprintf(stderr, "usage: %s %s", prog, usage);
	exit(1);
}