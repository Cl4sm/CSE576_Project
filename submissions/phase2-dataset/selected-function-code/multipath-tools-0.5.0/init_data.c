init_data(char *conf_file, void (*init_keywords) (void))
{
	int r;

	stream = fopen(conf_file, "r");
	if (!stream) {
		syslog(LOG_WARNING, "Configuration file open problem");
		return 1;
	}

	/* Init Keywords structure */
	(*init_keywords) ();

/* Dump configuration *
  vector_dump(keywords);
  dump_keywords(keywords, 0);
*/

	/* Stream handling */
	line_nr = 0;
	r = process_stream(keywords);
	fclose(stream);
	//free_keywords(keywords);

	return r;
}
