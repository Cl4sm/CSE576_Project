static int dag_init_output(libtrace_out_t *libtrace) {
	char *dag_dev_name = NULL;
	char *scan = NULL;
	struct dag_dev_t *dag_device = NULL;
	int stream = 1;
	
	/* XXX I don't know if this is important or not, but this function
	 * isn't present in all of the driver releases that this code is
	 * supposed to support! */
	/*
	unsigned long wake_time;
	dagutil_sleep_get_wake_time(&wake_time,0);
	*/

	dag_init_format_out_data(libtrace);
	/* Grab the mutex while we're likely to be messing with the device 
	 * list */
	pthread_mutex_lock(&open_dag_mutex);
	
	/* Specific streams are signified using a comma in the libtrace URI,
	 * e.g. dag:/dev/dag0,1 refers to stream 1 on the dag0 device.
	 *
	 * If no stream is specified, we will write using stream 1 */
	if ((scan = strchr(libtrace->uridata,',')) == NULL) {
		dag_dev_name = strdup(libtrace->uridata);
	} else {
		dag_dev_name = (char *)strndup(libtrace->uridata,
				(size_t)(scan - libtrace->uridata));
		stream = atoi(++scan);
	}
	FORMAT_DATA_OUT->dagstream = stream;

	/* See if our DAG device is already open */
	dag_device = dag_find_open_device(dag_dev_name);

	if (dag_device == NULL) {
		/* Device not yet opened - open it ourselves */
		dag_device = dag_open_output_device(libtrace, dag_dev_name);
	} else {
		/* Otherwise, just use the existing one */
		free(dag_dev_name);
		dag_dev_name = NULL;
	}

	/* Make sure we have successfully opened a DAG device */
	if (dag_device == NULL) {
		if (dag_dev_name) {
			free(dag_dev_name);
		}
		pthread_mutex_unlock(&open_dag_mutex);
		return -1;
	}

	FORMAT_DATA_OUT->device = dag_device;
	pthread_mutex_unlock(&open_dag_mutex);
	return 0;
}