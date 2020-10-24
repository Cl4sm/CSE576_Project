char *get_offset(const char *prefix, char *offset,
		 struct list_head *head, struct list_head **pos)
{
	struct list_head *next;
	struct mnt_list *this;
	size_t plen = strlen(prefix);
	size_t len = 0;

	*offset = '\0';
	next = *pos ? (*pos)->next : head->next;
	while (next != head) {
		char *pstart, *pend;

		this = list_entry(next, struct mnt_list, ordered);
		*pos = next;
		next = next->next;

		if (strlen(this->path) <= plen)
			continue;

		if (!strncmp(prefix, this->path, plen)) {
			pstart = &this->path[plen];

			/* not part of this sub-tree */
			if (*pstart != '/')
				continue;

			/* get next offset */
			pend = pstart;
			while (*pend++) ;
			len = pend - pstart - 1;
			strncpy(offset, pstart, len);
			offset[len] ='\0';
			break;
		}
	}

	while (next != head) {
		char *pstart;

		this = list_entry(next, struct mnt_list, ordered);

		if (strlen(this->path) <= plen + len)
			break;

		pstart = &this->path[plen];

		/* not part of this sub-tree */
		if (*pstart != '/')
			break;

		/* new offset */
		if (!*(pstart + len + 1))
			break;

		/* compare next offset */
		if (pstart[len] != '/' || strncmp(offset, pstart, len))
			break;

		*pos = next;
		next = next->next;
	}

	return *offset ? offset : NULL;
}