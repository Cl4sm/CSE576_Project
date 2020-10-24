{
   list_node* node;
   match_t* match;

   if ( !matches )
      return(1);

   for (node = head(matches) ; node ; node = next(node))
   {
      match = (match_t*) get_value(node);

      switch (match->type)
      {
	 case MATCH_WILD:
	    return(1);

	 case MATCH_PREFIX:
	    if (!strncasecmp(text, match->text, strlen(match->text)))
               return(1);
	    break;

	 case MATCH_EXACT:
	    if (!strcasecmp(text, match->text))
               return(1);
	    break;
      }
   }
   return(0);
}
