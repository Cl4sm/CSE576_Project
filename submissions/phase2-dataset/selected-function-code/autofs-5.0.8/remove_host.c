static void remove_host(struct host **hosts, struct host *host)
{
	struct host *last, *this;

	if (host == *hosts) {
		*hosts = (*hosts)->next;
		host->next = NULL;
		return;
	}

	this = *hosts;
	last = NULL;
	while (this) {
		if (this == host)
			break;
		last = this;
		this = this->next;
	}

	if (!last || !this)
		return;

	last->next = this->next;
	host->next = NULL;

	return;
}