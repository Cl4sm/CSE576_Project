static int dag_init_input(libtrace_t *libtrace) {
	struct stat buf;
        char *dag_dev_name = NULL;
	char *scan = NULL;

	/* Since DAG 2.5 has been changed to support a slightly different URI
	 * format, it's probably a good idea to deal with URIs specified in
	 * such a fashion even if we just end up ignoring the stream number */
	if ((scan = strchr(libtrace->uridata,',')) == NULL) {
		dag_dev_name = strdup(libtrace->uridata);
	} else {
		dag_dev_name = (char *)strndup(libtrace->uridata,
				(size_t)(scan - libtrace->uridata));
	}


	/* Make sure a DAG device with the right name exists */	
        if (stat(dag_dev_name, &buf) == -1) {
                trace_set_err(libtrace,errno,"stat(%s)",dag_dev_name);
		free(dag_dev_name);
                return -1;
        }
	
	dag_init_format_data(libtrace);
	if (S_ISCHR(buf.st_mode)) {
                /* DEVICE */
                if((FORMAT_DATA->fd = dag_open(dag_dev_name)) < 0) {
                        trace_set_err(libtrace,errno,"Cannot open DAG %s",
                                        dag_dev_name);
			free(dag_dev_name);
                        return -1;
                }

		/* Memory-map ourselves a pointer to the DAG memory hole */
                if((FORMAT_DATA->buf = (void *)dag_mmap(FORMAT_DATA->fd)) == MAP_FAILED) {
                        trace_set_err(libtrace,errno,"Cannot mmap DAG %s",
                                        dag_dev_name);
			free(dag_dev_name);
                        return -1;
                }
        } else {
                trace_set_err(libtrace,errno,"Not a valid dag device: %s",
                                dag_dev_name);
		free(dag_dev_name);
                return -1;
        }

	free(dag_dev_name);

        return 0;
}