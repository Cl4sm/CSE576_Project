static struct map_source *
prepare_plus_include(struct autofs_point *ap,
		     time_t age, char *key, unsigned int inc,
		     struct lookup_context *ctxt)
{
	struct map_source *current;
	struct map_source *source;
	struct map_type_info *info;
	const char *argv[2];
	char **tmp_argv, **tmp_opts;
	int argc;
	char *buf;

	current = ap->entry->current;
	ap->entry->current = NULL;
	master_source_current_signal(ap->entry);

	/*
	 * TODO:
	 * Initially just consider the passed in key to be a simple map
	 * name (and possible source) and use the global map options in
	 * the given autofs_point. ie. global options override.
	 *
	 * Later we might want to parse this and fill in the autofs_point
	 * options fields.
	 */
	/* skip plus */
	buf = strdup(key + 1);
	if (!buf) {
		error(ap->logopt, MODPREFIX "failed to strdup key");
		return NULL;
	}

	if (!(info = parse_map_type_info(buf))) {
		error(ap->logopt, MODPREFIX "failed to parse map info");
		free(buf);
		return NULL;
	}

	argc = 1;
	argv[0] = info->map;
	argv[1] = NULL;

	tmp_argv = (char **) copy_argv(argc, argv);
	if (!tmp_argv) {
		error(ap->logopt, MODPREFIX "failed to allocate args vector");
		free_map_type_info(info);
		free(buf);
		return NULL;
	}

	tmp_opts = (char **) copy_argv(ctxt->opts_argc, ctxt->opts_argv);
	if (!tmp_opts) {
		error(ap->logopt, MODPREFIX "failed to allocate options args vector");
		free_argv(argc, (const char **) tmp_argv);
		free_map_type_info(info);
		free(buf);
		return NULL;
	}

	tmp_argv = append_argv(argc, tmp_argv, ctxt->opts_argc, tmp_opts);
	if (!tmp_argv) {
		error(ap->logopt, MODPREFIX "failed to append options vector");
		free_map_type_info(info);
		free(buf);
		return NULL;
	}
	argc += ctxt->opts_argc;

	source = master_find_source_instance(current,
					     info->type, info->format,
					     argc, (const char **) tmp_argv);
	if (source) {
		/*
		 * Make sure included map age is in sync with its owner
		 * or we could incorrectly wipe out its entries.
		 */
		source->age = age;
		source->stale = 1;
	} else {
		source = master_add_source_instance(current,
						    info->type, info->format, age,
						    argc, (const char **) tmp_argv);
		if (!source) {
			free_argv(argc, (const char **) tmp_argv);
			free_map_type_info(info);
			free(buf);
			error(ap->logopt, "failed to add included map instance");
			return NULL;
		}
	}
	free_argv(argc, (const char **) tmp_argv);

	source->depth = current->depth + 1;
	if (inc)
		source->recurse = 1;

	free_map_type_info(info);
	free(buf);

	return source;
}