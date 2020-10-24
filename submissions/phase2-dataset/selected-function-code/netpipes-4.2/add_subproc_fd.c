void add_subproc_fd(fd, code, sid)
     int	fd;
     enum pipe_polarity	code;
     int	sid;
{
    struct pipeline	*temp;
    temp = malloc(sizeof(*temp));
    temp->child_fd = fd;
    temp->code = code;
    temp->session_id = sid;
    temp->syn_received = 0;
    temp->bytes_left = 0;
    temp->specact = NOTHING;
    temp->next = 0;
    /* add to tail */
    {
	struct pipeline	**curr;
	for (curr = &pipe_head; *curr; curr = &( (*curr)->next ) )
	    ;
	*curr = temp;
    }
}