pvmnametag(tag, found)
	int tag;
	int *found;
{
	static char buf[32];

	int i, j;

	for (i = sizeof(nametaglist)/sizeof(nametaglist[0]); i-- > 0; )
		if (tag >= nametaglist[i].first && tag <= nametaglist[i].last) {
			j = tag - nametaglist[i].first;
			if (found)
				*found = 1;
			return nametaglist[i].names[j];
		}
	sprintf(buf, "%d", tag);
	if (found)
		*found = 0;
	return buf;
}
