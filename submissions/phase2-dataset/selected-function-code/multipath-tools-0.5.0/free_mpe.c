void
free_mpe (struct mpentry * mpe)
{
	if (!mpe)
		return;

	if (mpe->wwid)
		FREE(mpe->wwid);

	if (mpe->selector)
		FREE(mpe->selector);

	if (mpe->getuid)
		FREE(mpe->getuid);

	if (mpe->uid_attribute)
		FREE(mpe->uid_attribute);

	if (mpe->alias)
		FREE(mpe->alias);

	if (mpe->prio_name)
		FREE(mpe->prio_name);

	if (mpe->prio_args)
		FREE(mpe->prio_args);

	FREE(mpe);
}
