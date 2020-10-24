int WriteAreas()
/*
 *  Write the SLNP AREAS file
 */
{
	struct conf_ent *cp;
	int	ret = 0;

	/* Loop through conference list */
	cp = conf_list;
	while (cp != NULL) {
		/*  Don't add empty conferences  */
		if(cp->count) {
		        if (strcmp (cp->name, MAIL_CONF_NAME)) {
			        if (cp->count > 0)
				        ret = fprintf (ctl_fd, "%07d\011%s\011un\n",
					        cp->number, cp->name);
		        } else {
			        ret = fprintf (ctl_fd, "%07d\011%s\011bn\n",
				        cp->number, cp->name);
		        }
		}
		cp = cp->next;
	}
	fclose (ctl_fd);
	return ret==EOF?ret:0;
}
