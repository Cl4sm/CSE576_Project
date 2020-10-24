static struct dag_dev_t *dag_open_device(libtrace_t *libtrace, char *dev_name) {
	struct stat buf;
	int fd;
	struct dag_dev_t *new_dev;

	/* Make sure the device exists */
        if (stat(dev_name, &buf) == -1) {
                trace_set_err(libtrace,errno,"stat(%s)",dev_name);
                return NULL;
        }

	/* Make sure it is the appropriate type of device */
	if (S_ISCHR(buf.st_mode)) {
		/* Try opening the DAG device */
		if((fd = dag_open(dev_name)) < 0) {
                        trace_set_err(libtrace,errno,"Cannot open DAG %s",
                                        dev_name);
                        return NULL;
                }
	} else {
		trace_set_err(libtrace,errno,"Not a valid dag device: %s",
                                dev_name);
                return NULL;
        }

	/* Add the device to our device list - it is just a doubly linked
	 * list with no inherent ordering; just tack the new one on the front
	 */
	new_dev = (struct dag_dev_t *)malloc(sizeof(struct dag_dev_t));
	new_dev->fd = fd;
	new_dev->dev_name = dev_name;
	new_dev->ref_count = 1;

	new_dev->prev = NULL;
	new_dev->next = open_dags;
	if (open_dags)
		open_dags->prev = new_dev;

	open_dags = new_dev;

	return new_dev;
}