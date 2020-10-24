static int add_host(struct host **list, struct host *host)
{
	struct host *this, *last;

	if (!*list) {
		*list = host;
		return 1;
	}

	this = *list;
	last = this;
	while (this) {
		if (this->proximity >= host->proximity)
			break;
		last = this;
		this = this->next;
	}

	if (host->cost) {
		while (this) {
			if (this->proximity != host->proximity)
				break;
			if (this->cost >= host->cost)
				break;
			last = this;
			this = this->next;
		}
	}

	if (last == this) {
		host->next = last;
		*list = host;
		return 1;
	}

	last->next = host;
	host->next = this;

	return 1;
}