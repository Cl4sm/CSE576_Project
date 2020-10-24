static void dfsssp_context_destroy(void *context)
{
	dfsssp_context_t *dfsssp_ctx = (dfsssp_context_t *) context;
	vertex_t *adj_list = (vertex_t *) (dfsssp_ctx->adj_list);
	uint32_t i = 0;
	link_t *link = NULL, *tmp = NULL;

	/* free adj_list */
	for (i = 0; i < dfsssp_ctx->adj_list_size; i++) {
		link = adj_list[i].links;
		while (link) {
			tmp = link;
			link = link->next;
			free(tmp);
		}
	}
	free(adj_list);
	dfsssp_ctx->adj_list = NULL;
	dfsssp_ctx->adj_list_size = 0;

	/* free srcdest2vl table and the split count information table
	   (can be done, because dfsssp_context_destroy is called after
	    osm_get_dfsssp_sl)
	 */
	vltable_dealloc(&(dfsssp_ctx->srcdest2vl_table));
	dfsssp_ctx->srcdest2vl_table = NULL;

	if (dfsssp_ctx->vl_split_count) {
		free(dfsssp_ctx->vl_split_count);
		dfsssp_ctx->vl_split_count = NULL;
	}
}