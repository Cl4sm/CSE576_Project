int set_action(struct nss_action *act, char *status, char *action, int negated)
{
	enum nsswitch_action a;

	if (!strcasecmp(action, "continue"))
		a = NSS_ACTION_CONTINUE;
	else if (!strcasecmp(action, "return"))
		a = NSS_ACTION_RETURN;
	else
		return 0;

	if (!strcasecmp(status, "SUCCESS")) {
		act[NSS_STATUS_SUCCESS].action = a;
		act[NSS_STATUS_SUCCESS].negated = negated;
	} else if (!strcasecmp(status, "NOTFOUND")) {
		act[NSS_STATUS_NOTFOUND].action = a;
		act[NSS_STATUS_NOTFOUND].negated = negated;
	} else if (!strcasecmp(status, "UNAVAIL")) {
		act[NSS_STATUS_UNAVAIL].action = a;
		act[NSS_STATUS_UNAVAIL].negated = negated;
	} else if (!strcasecmp(status, "TRYAGAIN")) {
		act[NSS_STATUS_TRYAGAIN].action = a;
		act[NSS_STATUS_TRYAGAIN].negated = negated;
	} else
		return 0;

	return 1;
}