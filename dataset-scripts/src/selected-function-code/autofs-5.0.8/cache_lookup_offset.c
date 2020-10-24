struct mapent *cache_lookup_offset(const char *prefix, const char *offset, int start, struct list_head *head)
{
	struct list_head *p;
	struct mapent *this;
	/* Keys for direct maps may be as long as a path name */
	char o_key[PATH_MAX];
	/* Avoid "//" at the beginning of paths */
	const char *path_prefix = strlen(prefix) > 1 ? prefix : "";
	size_t size;

	/* root offset duplicates "/" */
	size = snprintf(o_key, sizeof(o_key), "%s%s", path_prefix, offset);
	if (size >= sizeof(o_key))
		return NULL;

	list_for_each(p, head) {
		this = list_entry(p, struct mapent, multi_list);
		if (!strcmp(&this->key[start], o_key))
			return this;
	}
	return NULL;
}