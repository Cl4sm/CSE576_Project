static int check_nss_result(struct nss_source *this, enum nsswitch_status result)
{
	enum nsswitch_status status;
	struct nss_action a;

	/* Check if we have negated actions */
	for (status = 0; status < NSS_STATUS_MAX; status++) {
		a = this->action[status];
		if (a.action == NSS_ACTION_UNKNOWN)
			continue;

		if (a.negated && result != status) {
			if (a.action == NSS_ACTION_RETURN) {
				if (result == NSS_STATUS_SUCCESS)
					return 1;
				else
					return 0;
			}
		}
	}

	a = this->action[result];

	/* Check if we have other actions for this status */
	switch (result) {
	case NSS_STATUS_SUCCESS:
		if (a.action == NSS_ACTION_CONTINUE)
			break;
		return 1;

	case NSS_STATUS_NOTFOUND:
	case NSS_STATUS_UNAVAIL:
	case NSS_STATUS_TRYAGAIN:
		if (a.action == NSS_ACTION_RETURN) {
			return 0;
		}
		break;

	default:
		break;
	}

	return -1;
}