hwe_strmatch (struct hwentry *hwe1, struct hwentry *hwe2)
{
	if ((hwe2->vendor && !hwe1->vendor) ||
	    (hwe1->vendor && (!hwe2->vendor ||
			      strcmp(hwe1->vendor, hwe2->vendor))))
		return 1;

	if ((hwe2->product && !hwe1->product) ||
	    (hwe1->product && (!hwe2->product ||
			      strcmp(hwe1->product, hwe2->product))))
		return 1;

	if ((hwe2->revision && !hwe1->revision) ||
	    (hwe1->revision && (!hwe2->revision ||
			      strcmp(hwe1->revision, hwe2->revision))))
		return 1;

	return 0;
}
