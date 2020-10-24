struct host *new_host(const char *name,
		      struct sockaddr *addr, size_t addr_len,
		      unsigned int proximity, unsigned int weight,
		      unsigned int options)
{
	struct host *new;
	struct sockaddr *tmp2;
	char *tmp1;

	if (!name || !addr)
		return NULL;

	tmp1 = strdup(name);
	if (!tmp1)
		return NULL;

	tmp2 = malloc(addr_len);
	if (!tmp2) {
		free(tmp1);
		return NULL;
	}
	memcpy(tmp2, addr, addr_len);

	new = malloc(sizeof(struct host));
	if (!new) {
		free(tmp1);
		free(tmp2);
		return NULL;
	}

	memset(new, 0, sizeof(struct host));

	new->name = tmp1;
	new->addr_len = addr_len;
	new->addr = tmp2;
	new->proximity = proximity;
	new->weight = weight;
	new->options = options;

	return new;
}