struct device_node *of_get_parent(struct device_node *node)
{
	struct device_node *tmp;
	char *p;
	char *ptr;
	
	if((!node->path) || (!node) || (strlen(node->path)==1))
		return NULL;
	
	ptr=strdup(node->path);
	
	p=strrchr(ptr, '/');
	*p=0;
	p=strrchr(ptr, '/');
	*p=0;
	
	tmp=of_find_node_by_path(ptr);
	
	free(ptr);
	
	return tmp;
}