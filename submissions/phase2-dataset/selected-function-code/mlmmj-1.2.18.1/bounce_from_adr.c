char *bounce_from_adr(const char *recipient, const char *listadr,
		      const char *listdelim, const char *mailfilename,
		      const char *listdir)
{
	char *bounceaddr, *myrecipient, *mylistadr;
	char *indexstr, *listdomain, *a = NULL, *mymailfilename;
	char *staticbounceaddr, *staticbounceaddr_localpart;
	char *staticbounceaddr_domain;
	size_t len;

	mymailfilename = mystrdup(mailfilename);
	if (!mymailfilename) {
		return NULL;
	}

	indexstr = get_index_from_filename(mymailfilename);
	if (!indexstr) {
		myfree(mymailfilename);
		return NULL;
	}

	myrecipient = mystrdup(recipient);
	if (!myrecipient) {
		myfree(mymailfilename);
		return NULL;
	}
	a = strchr(myrecipient, '@');
	if (a)
		*a = '=';

	mylistadr = mystrdup(listadr);
	if (!mylistadr) {
		myfree(mymailfilename);
		myfree(myrecipient);
		return NULL;
	}

	listdomain = strchr(mylistadr, '@');
	if (!listdomain) {
		myfree(mymailfilename);
		myfree(myrecipient);
		myfree(mylistadr);
		return NULL;
	}
	*listdomain++ = '\0';

	/* 11 = "bounces-" + "-" + "@" + NUL */
	len = strlen(mylistadr) + strlen(listdelim) + strlen(myrecipient)
		   + strlen(indexstr) + strlen(listdomain) + 11;

	staticbounceaddr = ctrlvalue(listdir, "staticbounceaddr");
	if (staticbounceaddr) {
		staticbounceaddr_localpart = genlistname(staticbounceaddr);
		staticbounceaddr_domain = genlistfqdn(staticbounceaddr);

		/* localpart + "-" + domain */
		len += strlen(staticbounceaddr_localpart) + 1
				+ strlen(staticbounceaddr_domain);
	} else {
		staticbounceaddr_localpart = NULL;
		staticbounceaddr_domain = NULL;
	}

	bounceaddr = mymalloc(len);
	if (!bounceaddr) {
		myfree(staticbounceaddr);
		myfree(staticbounceaddr_localpart);
		myfree(staticbounceaddr_domain);
		myfree(myrecipient);
		myfree(mylistadr);
		return NULL;
	}

	if (staticbounceaddr) {
		snprintf(bounceaddr, len, "%s%s%s-bounces-%s-%s@%s", 
			staticbounceaddr_localpart, listdelim, mylistadr,
			indexstr, myrecipient, staticbounceaddr_domain);

		myfree(staticbounceaddr);
		myfree(staticbounceaddr_localpart);
		myfree(staticbounceaddr_domain);
	} else {
		snprintf(bounceaddr, len, "%s%sbounces-%s-%s@%s", mylistadr, listdelim,
			indexstr, myrecipient, listdomain);
	}

	myfree(myrecipient);
	myfree(mylistadr);
	myfree(indexstr);
	myfree(mymailfilename);

	return bounceaddr;
}