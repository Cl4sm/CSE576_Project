static int
hwe_regmatch (struct hwentry *hwe1, struct hwentry *hwe2)
{
	regex_t vre, pre, rre;
	int retval = 1;

	if (hwe1->vendor &&
	    regcomp(&vre, hwe1->vendor, REG_EXTENDED|REG_NOSUB))
		goto out;

	if (hwe1->product &&
	    regcomp(&pre, hwe1->product, REG_EXTENDED|REG_NOSUB))
		goto out_vre;

	if (hwe1->revision &&
	    regcomp(&rre, hwe1->revision, REG_EXTENDED|REG_NOSUB))
		goto out_pre;

	if ((!hwe1->vendor || !hwe2->vendor ||
	     !regexec(&vre, hwe2->vendor, 0, NULL, 0)) &&
	    (!hwe1->product || !hwe2->product ||
	     !regexec(&pre, hwe2->product, 0, NULL, 0)) &&
	    (!hwe1->revision || !hwe2->revision ||
	     !regexec(&rre, hwe2->revision, 0, NULL, 0)))
		retval = 0;

	if (hwe1->revision)
		regfree(&rre);
out_pre:
	if (hwe1->product)
		regfree(&pre);
out_vre:
	if (hwe1->vendor)
		regfree(&vre);
out:
	return retval;
}
