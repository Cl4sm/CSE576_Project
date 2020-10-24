void __cl_disp_worker(IN void *context)
{
	cl_disp_msg_t *p_msg;
	cl_dispatcher_t *p_disp = (cl_dispatcher_t *) context;

	cl_spinlock_acquire(&p_disp->lock);

	/* Process the FIFO until we drain it dry. */
	while (cl_qlist_count(&p_disp->msg_fifo)) {
		/* Pop the message at the head from the FIFO. */
		p_msg =
		    (cl_disp_msg_t *) cl_qlist_remove_head(&p_disp->msg_fifo);

		/* we track the tim ethe last message spent in the queue */
		p_disp->last_msg_queue_time_us =
		    cl_get_time_stamp() - p_msg->in_time;

		/*
		 * Release the spinlock while the message is processed.
		 * The user's callback may reenter the dispatcher
		 * and cause the lock to be reaquired.
		 */
		cl_spinlock_release(&p_disp->lock);
		p_msg->p_dest_reg->pfn_rcv_callback((void *)p_msg->p_dest_reg->
						    context,
						    (void *)p_msg->p_data);

		cl_atomic_dec(&p_msg->p_dest_reg->ref_cnt);

		/* The client has seen the data.  Notify the sender as appropriate. */
		if (p_msg->pfn_xmt_callback) {
			p_msg->pfn_xmt_callback((void *)p_msg->context,
						(void *)p_msg->p_data);
			cl_atomic_dec(&p_msg->p_src_reg->ref_cnt);
		}

		/* Grab the lock for the next iteration through the list. */
		cl_spinlock_acquire(&p_disp->lock);

		/* Return this message to the pool. */
		cl_qpool_put(&p_disp->msg_pool, (cl_pool_item_t *) p_msg);
	}

	cl_spinlock_release(&p_disp->lock);
}