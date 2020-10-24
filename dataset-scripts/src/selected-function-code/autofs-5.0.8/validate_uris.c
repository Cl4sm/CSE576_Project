static void validate_uris(struct list_head *list)
{
	struct list_head *next;

	next = list->next;
	while (next != list) {
		struct ldap_uri *this;

		this = list_entry(next, struct ldap_uri, list);
		next = next->next;

		/* At least we get some basic validation */
		if (!ldap_is_ldap_url(this->uri)) {
			list_del(&this->list);
			free(this->uri);
			free(this);
		}
	}

	return;			
}