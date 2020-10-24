dcc_searchlist(name, user, type, flag, othername)
	u_char	*name,
		*user;
	int	type,
		flag;
	u_char	*othername;
{
	DCC_list **Client, *NewClient;

	for (Client = (&ClientList); *Client ; Client = (&(**Client).next))
	{
		if ((((**Client).flags&DCC_TYPES) == type) &&
		    ((!name || (!my_stricmp(name, (**Client).description))) ||
		    (othername && (**Client).othername && (!my_stricmp(othername, (**Client).othername)))) &&
		    (my_stricmp(user, (**Client).user)==0))
			return *Client;
	}
	if (!flag)
		return NULL;
	*Client = NewClient = (DCC_list *) new_malloc(sizeof(DCC_list));
	NewClient->flags = type;
	NewClient->read = NewClient->write = NewClient->file = -1;
	NewClient->filesize = filesize;
	NewClient->next = (DCC_list *) 0;
	NewClient->user = NewClient->description = NewClient->othername = NULL;
	NewClient->bytes_read = NewClient->bytes_sent = 0L;
	NewClient->starttime = 0;
	NewClient->buffer = 0;
	NewClient->remname = 0;
	malloc_strcpy(&NewClient->description, name);
	malloc_strcpy(&NewClient->user, user);
	malloc_strcpy(&NewClient->othername, othername);
	time(&NewClient->lasttime);
	return NewClient;
}
