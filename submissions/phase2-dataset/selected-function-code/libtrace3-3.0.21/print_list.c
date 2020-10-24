void print_list(element_t *list)
{
    if(list == NULL)
	return;
	
    switch(list->type)
    {
	case NEXTHEADER: printf("*Nextheader, prefix='%s', target='%s'\n", 
			    list->data->nextheader->prefix, 
			    list->data->nextheader->fieldname);
			    break;
	
	case FIELD: printf("*Field, order = '%d', size = '%d', "
			    "display='%d', name='%s'\n",
			    list->data->field->order, 
			    list->data->field->size, 
			    list->data->field->display,
			    list->data->field->identifier);
			    break;
    };
    /*printf("%s\n", list->data->identifier); */
    print_list(list->next);
}