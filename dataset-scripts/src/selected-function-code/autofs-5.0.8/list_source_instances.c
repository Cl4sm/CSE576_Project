static void list_source_instances(struct map_source *source, struct map_source *instance)
{
	if (!source || !instance) {
		printf("none");
		return;
	}

	if (instance->next)
		list_source_instances(source, instance->next);

	/*
	 * For convienience we map nss instance type "files" to "file".
	 * Check for that and report corrected instance type.
	 */
	if (strcmp(instance->type, "file"))
		printf("%s ", instance->type);
	else {
		if (source->argv && *(source->argv[0]) != '/')
			printf("files ");
		else
			printf("%s ", instance->type);
	}

	return;
}