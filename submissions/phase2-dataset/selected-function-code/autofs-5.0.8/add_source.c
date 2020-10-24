struct nss_source *add_source(struct list_head *head, char *source)
{
	struct nss_source *s;
	char *tmp;
	enum nsswitch_status status;

	s = malloc(sizeof(struct nss_source));
	if (!s)
		return NULL;

	memset(s, 0, sizeof(struct nss_source));
	INIT_LIST_HEAD(&s->list);

	tmp = strdup(source);
	if (!tmp) {
		free(s);
		return NULL;
	}
	s->source = tmp;

	for (status = 0; status < NSS_STATUS_MAX; status++)
		s->action[status].action = NSS_ACTION_UNKNOWN;

	list_add_tail(&s->list, head);

	return s;
}