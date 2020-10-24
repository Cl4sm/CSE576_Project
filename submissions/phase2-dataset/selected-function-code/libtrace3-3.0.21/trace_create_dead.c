DLLEXPORT libtrace_t * trace_create_dead (const char *uri) {
	libtrace_t *libtrace = (libtrace_t *) malloc(sizeof(libtrace_t));
	char *scan = (char *)calloc(sizeof(char),URI_PROTO_LINE);
	char *uridata;
	struct libtrace_format_t *tmp;

	trace_init();
	
	libtrace->err.err_num = TRACE_ERR_NOERROR;

	if((uridata = strchr(uri,':')) == NULL) {
		xstrncpy(scan, uri, strlen(uri));
	} else {
		xstrncpy(scan,uri, (size_t)(uridata - uri));
	}
	
	libtrace->err.err_num = TRACE_ERR_NOERROR;
	libtrace->format=NULL;
        
	libtrace->event.tdelta = 0.0;
	libtrace->event.packet = NULL;
	libtrace->event.psize = 0;
	libtrace->event.trace_last_ts = 0.0;
	libtrace->filter = NULL;
	libtrace->snaplen = 0;
	libtrace->started=false;
	libtrace->uridata = NULL;
	libtrace->io = NULL;
	libtrace->filtered_packets = 0;
	
	for(tmp=formats_list;tmp;tmp=tmp->next) {
                if (strlen(scan) == strlen(tmp->name) &&
                                !strncasecmp(scan,
                                        tmp->name,
                                        strlen(scan))) {
                                libtrace->format=tmp;
                                break;
                                }
        }
        if (libtrace->format == 0) {
		trace_set_err(libtrace,TRACE_ERR_BAD_FORMAT,
				"Unknown format (%s)",scan);
        }

	libtrace->format_data = NULL;
	free(scan);
	return libtrace;

}