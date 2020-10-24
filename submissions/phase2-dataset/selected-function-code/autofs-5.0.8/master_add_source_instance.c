struct map_source *
master_add_source_instance(struct map_source *source, const char *type, const char *format, time_t age, int argc, const char **argv)
{
	struct map_source *instance;
	struct map_source *new;
	char *ntype, *nformat;
	const char **tmpargv;
	int status;

	instance = master_find_source_instance(source, type, format, argc, argv);
	if (instance)
		return instance;

	new = malloc(sizeof(struct map_source));
	if (!new)
		return NULL;
	memset(new, 0, sizeof(struct map_source));

	if (type) {
		ntype = strdup(type);
		if (!ntype) {
			master_free_map_source(new, 0);
			return NULL;
		}
		new->type = ntype;
	}

	if (format) {
		nformat = strdup(format);
		if (!nformat) {
			master_free_map_source(new, 0);
			return NULL;
		}
		new->format = nformat;
	}

	new->age = age;
	new->master_line = 0;
	new->mc = source->mc;
	new->exp_timeout = source->exp_timeout;
	new->stale = 1;

	tmpargv = copy_argv(argc, argv);
	if (!tmpargv) {
		master_free_map_source(new, 0);
		return NULL;
	}
	new->argc = argc;
	new->argv = tmpargv;

	status = pthread_mutex_lock(&instance_mutex);
	if (status)
		fatal(status);

	if (!source->instance)
		source->instance = new;
	else {
		/*
		 * We know there's no other instance of this
		 * type so just add to head of list
		 */
		new->next = source->instance;
		source->instance = new;
	}

	status = pthread_mutex_unlock(&instance_mutex);
	if (status)
		fatal(status);

	return new;
}