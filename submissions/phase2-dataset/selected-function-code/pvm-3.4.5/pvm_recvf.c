int (*
pvm_recvf(new))()
	int (*new)__ProtoGlarp__((int,int,int));
{
	int (*old)() = recv_match;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_RECVF,TEV_EVENT_ENTRY))
			TEV_FIN;
	}

	recv_match = new ? new : def_match;

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_RECVF,TEV_EVENT_EXIT))
			TEV_FIN;
		TEV_ENDEXCL;
	}

	return old;
}
