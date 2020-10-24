DLLEXPORT const char * trace_parse_uri(const char *uri, char **format) {
	const char *uridata = 0;
	
	if((uridata = strchr(uri,':')) == NULL) {
                /* Badly formed URI - needs a : */
                return 0;
        }

        if ((unsigned)(uridata - uri) > URI_PROTO_LINE) {
                /* Badly formed URI - uri type is too long */
                return 0;
        }

	/* NOTE: this is allocated memory - it should be freed by the caller
	 * once they are done with it */
        *format=xstrndup(uri, (size_t)(uridata - uri));

	/* Push uridata past the delimiter */
        uridata++;
	
	return uridata;
}