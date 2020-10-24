DLLEXPORT libtrace_out_t *trace_create_output(const char *uri) {
	libtrace_out_t *libtrace = 
			(libtrace_out_t*)malloc(sizeof(libtrace_out_t));
	
	char *scan = 0;
        const char *uridata = 0;
	struct libtrace_format_t *tmp;

	trace_init();

	libtrace->err.err_num = TRACE_ERR_NOERROR;
	strcpy(libtrace->err.problem,"Error message set\n");
        libtrace->format = NULL;
	libtrace->uridata = NULL;
	
        /* Parse the URI to determine what capture format we want to write */

	if ((uridata = trace_parse_uri(uri, &scan)) == 0) {
		trace_set_err_out(libtrace,TRACE_ERR_BAD_FORMAT,
				"Bad uri format (%s)",uri);
		return libtrace;
	}
	
	/* Attempt to find the format in the list of supported formats */
	for(tmp=formats_list;tmp;tmp=tmp->next) {
                if (strlen(scan) == strlen(tmp->name) &&
                                !strncasecmp(scan,
                                        tmp->name,
                                        strlen(scan))) {
                                libtrace->format=tmp;
                                break;
                                }
        }
	free(scan);

        if (libtrace->format == NULL) {
		trace_set_err_out(libtrace,TRACE_ERR_BAD_FORMAT,
				"Unknown output format (%s)",scan);
                return libtrace;
        }
        libtrace->uridata = strdup(uridata);

        /* libtrace->format now contains the type of uri
         * libtrace->uridata contains the appropriate data for this
	 */

        if (libtrace->format->init_output) {
		/* 0 on success, -1 on failure */
                switch(libtrace->format->init_output(libtrace)) {
			case -1: /* failure */
				return libtrace;
			case 0: /* success */
				break;
			default:
				assert(!"Internal error: init_output() should return -1 for failure, or 0 for success");
		}
	} else {
		trace_set_err_out(libtrace,TRACE_ERR_UNSUPPORTED,
				"Format does not support writing (%s)",scan);
                return libtrace;
        }


	libtrace->started=false;
	return libtrace;
}