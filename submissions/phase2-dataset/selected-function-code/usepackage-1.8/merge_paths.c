{
   list_node* enode;
   list_node* vnode;
   char* vpath;

   if (elist && vlist)
   {
      for (vnode = list_tail(vlist) ; vnode ; vnode = previous(vnode))
      {
	 vpath = (char*) get_value(vnode);

	 for (enode = head(elist) ; enode ; enode = next(enode))
	    if (!strcmp(vpath, (char*) get_value(enode)))
	    {
	       remove_node(elist, enode, 0);
	       break;
	    }

	 add_to_head(elist, get_value(vnode));
      }
   }
   else if (vlist && head(vlist))
   {
      return(vlist);
   }

   return(elist);
}
