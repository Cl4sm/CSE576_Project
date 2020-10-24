static void rig_single()
{
    struct pipeline	*curr;
    /* Dear mother of god.  I have to invert the polarity of all the
       pipes.  How the hell am I going to explain this in the manual
       page? */

    for (curr = pipe_head; curr; curr = curr->next) {
	switch (curr->code) {
	case READABLE:
	    curr->code = WRITEABLE;
	    break;
	case WRITEABLE:
	    curr->code = READABLE;
	    break;
	default:
	    /* I don't need to diddle the duplex cases really */
	    break;
	}

	/* so that select and I/O will work */
	curr->pipe[0] = curr->child_fd;
	curr->pipe[1] = -1;
    }
    
}