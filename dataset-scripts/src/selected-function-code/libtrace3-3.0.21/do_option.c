static void do_option(const char *option)
{
	if (*option == '\0') 
		;
	else if (strcmp(option,"stats") == 0)
		keep_stats = 1;
	/*
	else if (strcmp(option,"directwrite") == 0)
		force_directio_write = 1;
	else if (strcmp(option,"directread") == 0)
		force_directio_read  = 1;
	*/
	else if (strcmp(option,"nothreads") == 0)
		use_threads = 0;
	else if (strcmp(option,"noautodetect") == 0)
		use_autodetect = 0;
	else if (strncmp(option,"threads=",8) == 0)
		use_threads = atoi(option+8);
	else if (strncmp(option,"buffers=",8) == 0)
		max_buffers = atoi(option+8);
	else {
		fprintf(stderr,"Unknown libtraceio debug option '%s'\n", option);
	}
}