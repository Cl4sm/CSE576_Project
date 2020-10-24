void
restore_message_from()
{
	struct mfstack *mfs = mfstack_head.next;

	if (mfs == NULL)
	{
		/*yell("--- restore_message_from: NULL next pointer, fudging..");*/
		malloc_strcpy(&who_from, NULL);
		who_level = LOG_CRAP;
	}
	else
	{
		malloc_strcpy(&who_from, mfs->who_from);
		who_level = mfs->who_level;
		mfstack_head.next = mfs->next;
		new_free(&mfs->who_from);
		new_free(&mfs);
	}
}
