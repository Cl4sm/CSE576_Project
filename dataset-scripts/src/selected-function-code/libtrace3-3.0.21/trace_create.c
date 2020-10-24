DLLEXPORT libtrace_t *trace_create(const char *uri) {
        libtrace_t *libtrace = 
			(libtrace_t *)malloc(sizeof(libtrace_t));
        char *scan = 0;
        const char *uridata = 0;                  

	trace_init();

	assert(uri && "Passing NULL to trace_create makes me a very sad program");

	if (!libtrace) {
		/* Out of memory */
		return NULL;
	}
	
	libtrace->err.err_num = TRACE_ERR_NOERROR;
	libtrace->format=NULL;
        
	libtrace->event.tdelta = 0.0;
	libtrace->event.packet = NULL;
	libtrace->event.psize = 0;
	libtrace->event.trace_last_ts = 0.0;
	libtrace->event.waiting = false;
	libtrace->filter = NULL;
	libtrace->snaplen = 0;
	libtrace->started=false;
	libtrace->uridata = NULL;
	libtrace->io = NULL;
	libtrace->filtered_packets = 0;
	libtrace->accepted_packets = 0;

        /* Parse the URI to determine what sort of trace we are dealing with */
	if ((uridata = trace_parse_uri(uri, &scan)) == 0) {
		/* Could not parse the URI nicely */
		guess_format(libtrace,uri);
		if (!libtrace->format) {
			trace_set_err(libtrace,TRACE_ERR_BAD_FORMAT,"Unable to guess format (%s)",uri);
			return libtrace;
		}
	}
	else {
		struct libtrace_format_t *tmp;

		/* Find a format that matches the first part of the URI */
		for (tmp=formats_list;tmp;tmp=tmp->next) {
			if (strlen(scan) == strlen(tmp->name) &&
					strncasecmp(scan, tmp->name, strlen(scan)) == 0
					) {
				libtrace->format=tmp;
				break;
			}
		}

		if (libtrace->format == 0) {
			trace_set_err(libtrace, TRACE_ERR_BAD_FORMAT,
					"Unknown format (%s)",scan);
			return libtrace;
		}

		libtrace->uridata = strdup(uridata);
	}
        /* libtrace->format now contains the type of uri
         * libtrace->uridata contains the appropriate data for this
	 */
       
       	/* Call the init_input function for the matching capture format */ 
	if (libtrace->format->init_input) {
		int err=libtrace->format->init_input(libtrace);
		assert (err==-1 || err==0);
		if (err==-1) {
			/* init_input should call trace_set_err to set 
			 * the error message
			 */
			return libtrace;
		}
	} else {
		trace_set_err(libtrace,TRACE_ERR_UNSUPPORTED,
				"Format does not support input (%s)",scan);
		return libtrace;
	}
	
	if (scan)
		free(scan);
	libtrace->err.err_num=TRACE_ERR_NOERROR;
	libtrace->err.problem[0]='\0';
        return libtrace;
}