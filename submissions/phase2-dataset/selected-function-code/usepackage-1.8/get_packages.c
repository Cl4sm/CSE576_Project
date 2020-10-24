                 linked_list** annotations)
{
#ifdef YYDEBUG
   yydebug = debugging;
#endif

   yyerrors = 0;

   stack_pointer = -1;
   if (include(main_package_filename))
      return(1);

   yyparse();

   if (yyerrors)
      return(2);

   *packages = loaded_packages;
   *groups = loaded_groups;
   *annotations = loaded_annotations;
   return(0);
}
