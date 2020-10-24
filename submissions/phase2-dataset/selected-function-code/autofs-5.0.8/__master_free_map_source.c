static void __master_free_map_source(struct map_source *source, unsigned int free_cache)
{
	if (source->type)
		free(source->type);
	if (source->format)
		free(source->format);
	if (free_cache && source->mc)
		cache_release(source);
	if (source->lookup) {
		struct map_source *instance;

		instance = source->instance;
		while (instance) {
			if (instance->lookup)
				close_lookup(instance->lookup);
			instance = instance->next;
		}
		close_lookup(source->lookup);
	}
	if (source->argv)
		free_argv(source->argc, source->argv);
	if (source->instance) {
		struct map_source *instance, *next;

		instance = source->instance;
		while (instance) {
			next = instance->next;
			__master_free_map_source(instance, 0);
			instance = next;
		}
	}

	free(source);

	return;
}