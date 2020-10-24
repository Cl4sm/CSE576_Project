int yyerror(const char *s)
{
    element_t *tmp;
    
    fprintf(stderr, "XXX %s\n"
		    "XXX %s on line %d\n"
		    "XXX Falling back to generic_decode()\n", 
		    file, s, lines);
    /* 
     * Clear the list so we don't do partial matching...makes it a bit
     * more obvious that something is broken perhaps.
     * XXX Not sure if it is better to parse none of the packet, or part 
     * of the packet in the event of error? Feel free to remove this if
     * that is desired.
     */

    while(el_list != NULL)
    {
	tmp = el_list;
	el_list = el_list->next;

	switch(tmp->type)
	{
	    case FIELD: free(tmp->data->field); break;
	    case NEXTHEADER: free(tmp->data->nextheader); break;
	}
	free(tmp->data);	
	free(tmp);
	printf("deleting...\n");
    }

    return 0;
}