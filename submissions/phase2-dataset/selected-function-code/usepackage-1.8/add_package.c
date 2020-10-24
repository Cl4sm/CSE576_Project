{
   variable_t* evar;
   variable_t* vvar;
   list_node* enode;
   list_node* vnode;
   list_node* rnode;
   group_t* group;
   char* name;
   int got_one;
   char* text;
   
   if (package->requires)
   {
      DEBUG(stderr, "(pre-using required packages list)\n");

      for (rnode=list_tail(package->requires) ; rnode ; rnode=previous(rnode))
      {
         name = (char*) get_value(rnode);

         if (group = get_group(name))
            use_group(group);
         else
            use_package(name);
      }
   }

   for (vnode = head(package->variables) ; vnode ; vnode = next(vnode))
   {
      vvar = get_value(vnode);
      got_one = 0;
      for (enode = head(the_environment) ; enode ; enode = next(enode))
      {
         evar = get_value(enode);
         if (!strcmp(vvar->name, evar->name))
         {
            remove_node(the_environment, enode, 0);
            add_to_tail(the_environment, update_var(evar, vvar));
            got_one = 1;
            break;
         }
      }
      if (!got_one)
      {
         enode = get_into_env(vvar);
         evar = get_value(enode);
         set_value(enode, update_var(evar, vvar));
      }
   }
   
   for (vnode = head(package->scripts) ; vnode ; vnode = next(vnode))
   {
      text = ((script_t*) get_value(vnode))->text;
      add_to_tail(the_scripts, (void*) text);
   }   
}
