static void spawn_child(cmd, sockfd)
     char	**cmd;
     int	sockfd;
{
    struct pipeline	*curr;

    signal(SIGCHLD,waitonchild);
    child_running = 1;		/* well, not yet. */
    child_unreaped = 1;

    /* We're about to allocate a big stack of descriptors.  Let's make
       sure we don't step on on our own toes.  Dup descriptor 0 onto
       each of the descriptors so that our allocations won't get one
       of them.  If you don't have a descriptor 0, then you're a FREAK! */
    /* Uhoh.  Some of them may already be connected to the parent.
       Bob attaches some funky things in funky places. */
    /* The way I tell if a descriptor has been allocated or not is I
       select() on it.  If I get EBADF, the descriptor has not been
       allocated and I can stomp on it before the fork.  If I don't,
       then I won't stomp on it till after the fork. */
    {
	for (curr = pipe_head; curr; curr = curr->next) {
	    if (curr->child_fd<0)
		continue;
	    if (!valid_descriptor(curr->child_fd))
		dup2(0, curr->child_fd); /* "reserve" it */
	}
    }

    for (curr = pipe_head; curr; curr = curr->next) {
	if (curr->child_fd<0)
	    continue;		/* skip this special channel */
	switch (curr->code) {
	case READABLE:
	case WRITEABLE:
	    if (pipe(curr->pipe) !=0) {
		fprintf(stderr, "%s: totally failed to pipe(2): ", progname);
		perror("");
		exit (EXITCODE_SYSCALLFAILED);
	    }
	    break;
	case DUPLEX:
	case DUPLEX_IO:
	case DUPLEX_OI:
	    if (0!=socketpair(AF_UNIX, SOCK_STREAM, 0/*let it choose*/,
			      curr->pipe)) {
		fprintf(stderr, "%s: totally failed to socketpair(2): ",
			progname);
		perror("");
		exit (EXITCODE_SYSCALLFAILED);
	    }
	}
	if (curr->code == WRITEABLE) {
	    /* we need to invert the polarity for this case, eh, Geordi? */
	    int	t;
	    t = curr->pipe[0];
	    curr->pipe[0] = curr->pipe[1];
	    curr->pipe[1] = t;
	}
    }

    childpid = fork();
    if (childpid<0) {
	fprintf(stderr, "%s: unable to fork: ", progname);
	perror("");
	/* I would clear child_running, but, look at the next line */
	exit(EXITCODE_SYSCALLFAILED);
    }

    /* now there's a child running (assuming no race conditions, which
       is why I set it up above and not here.  I'm stupid, but
       paranoid). */

    if (childpid==0) {
	/* child */
	close(sockfd);		/* can't have the child accidentally
				   stomping on our conversation. */
	for (curr = pipe_head; curr; curr = curr->next) {
	    if (curr->child_fd<0)
		continue;		/* skip this special channel */
	    close(curr->pipe[0]);
	    dup2(curr->pipe[1], curr->child_fd);
	    close(curr->pipe[1]);
	}

	execvp(*cmd, cmd);
	fprintf(stderr, "%s: Unable to exec %s: ", progname, *cmd);
	perror("");

	exit(EXITCODE_EXECFAILED);

    } else {
	/* parent */
	for (curr = pipe_head; curr; curr = curr->next) {
	    if (curr->child_fd<0)
		continue;		/* skip this special channel */
	    close(curr->pipe[1]);
	}
    }
}