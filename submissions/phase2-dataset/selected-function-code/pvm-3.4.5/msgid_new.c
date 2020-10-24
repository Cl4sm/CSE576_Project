msgid_new()
{
	struct msgid *mp;

	if (!(mp = msgid_get_header()))
		goto oops;

	mp->id = -1;
	mp->otid = -1;
	mp->ctxt = -1;
	mp->complete = 0;
	mp->ubuf = (char *) 0;

	return mp;


oops:
	pvmlogerror("msgid_new() can't get memory\n");
	pvmbailout(0);
	return (struct msgid *) 0;
}
