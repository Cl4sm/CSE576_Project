static void guess_format(libtrace_t *libtrace, const char *filename)
{
	struct libtrace_format_t *tmp;
	
	/* Try and guess based on filename */
	for(tmp = formats_list; tmp; tmp=tmp->next) {
		if (tmp->probe_filename && tmp->probe_filename(filename)) {
			libtrace->format = tmp;
			libtrace->uridata = strdup(filename);
			return;
		}
	}

	libtrace->io = wandio_create(filename);
	if (!libtrace->io)
		return;

	/* Try and guess based on file magic */
	for(tmp = formats_list; tmp; tmp=tmp->next) {
		if (tmp->probe_magic && tmp->probe_magic(libtrace->io)) {
			libtrace->format = tmp;
			libtrace->uridata = strdup(filename);
			return;
		}
	}
	
	/* Oh well */
	return;
}