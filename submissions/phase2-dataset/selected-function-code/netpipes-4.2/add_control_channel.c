void add_control_channel()
{
    struct pipeline	*temp;
    temp = malloc(sizeof(*temp));
    temp->child_fd = -1;
    temp->pipe[0] = -10;
    temp->code = DUPLEX_OI;
    temp->session_id = SESSION_ENCAP;
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

    pl_encapsulate_control = temp;
}