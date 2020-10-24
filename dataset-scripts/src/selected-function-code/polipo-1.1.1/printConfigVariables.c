void
printConfigVariables(FILE *out, int html)
{
    ConfigVariablePtr var;
    int entryno = 0;

#define PRINT_SEP() \
    do {if(html) fprintf(out, "</td><td>"); else fprintf(out, " ");} while(0)

    if(html) {
        fprintf(out, "<table>\n");
        fprintf(out, "<tbody>\n");
    }

    if(html) {
        alternatingHttpStyle(out, "configlist");
        fprintf(out,
                "<table id=configlist>\n"
                "<thead>\n"
                "<tr><th>variable name</th>"
                "<th>current value</th>"
                "<th>new value</th>"
                "<th>description</th>\n"
                "</thead><tbody>\n"
);
    }

    /* configFile is not a config variable, for obvious bootstrapping reasons.
       CHUNK_SIZE is hardwired for now. */

    fprintf(out,
	    html ?
	    "<tr class=\"even\"><td>configFile</td><td>%s</td><td></td><td>"
	    "Configuration file.</td></tr>\n" :
	    "configFile %s Configuration file.\n",
	    configFile && configFile->length > 0 ?
	    configFile->string : "(none)");
    fprintf(out,
	    html ?
	    "<tr class=\"odd\"><td>CHUNK_SIZE</td><td>%d</td><td></td><td>"
	    "Unit of chunk memory allocation.</td></tr>\n" :
	    "CHUNK_SIZE %d Unit of chunk memory allocation.\n", CHUNK_SIZE);
    
    var = configVariables;
    while(var != NULL) {
      if(html) {
          if(entryno % 2)
              fprintf(out, "<tr class=odd>");
          else
              fprintf(out, "<tr class=even>");
          fprintf(out, "<td>");
      }

      fprintf(out, "%s", var->name->string);

      fprintf(out, html ? "<br/>" : " "); 
      
      fprintf(out, html ? "<i>" : "");    
      
      switch(var->type) {
      case CONFIG_INT: case CONFIG_OCTAL: case CONFIG_HEX:
	  fprintf(out, "integer"); break;
      case CONFIG_TIME: fprintf(out, "time"); break;
      case CONFIG_BOOLEAN: fprintf(out, "boolean"); break;
      case CONFIG_TRISTATE: fprintf(out, "tristate"); break;
      case CONFIG_TETRASTATE: fprintf(out, "4-state"); break;
      case CONFIG_PENTASTATE: fprintf(out, "5-state"); break;
      case CONFIG_FLOAT: fprintf(out, "float"); break;
      case CONFIG_ATOM: case CONFIG_ATOM_LOWER: case CONFIG_PASSWORD:
          fprintf(out, "atom"); break;
      case CONFIG_INT_LIST: fprintf(out, "intlist"); break;
      case CONFIG_ATOM_LIST: case CONFIG_ATOM_LIST_LOWER:
	  fprintf(out, "list"); break;
      default: abort();
      }
        
      fprintf(out, html ? "</i>" : "");

      PRINT_SEP();

      printVariable(out, var, html, 0);

      PRINT_SEP();
	
      if(html) {
	printVariableForm(out, var);
	PRINT_SEP();
      }

      fprintf(out, "%s", var->help?var->help:"");
      if(html)
	fprintf(out, "</td></tr>\n");
      else
	fprintf(out, "\n");

      entryno++;
      var = var->next;
    }
    if(html) {
        fprintf(out, "</tbody>\n");
        fprintf(out, "</table>\n");
    }
    return;
#undef PRINT_SEP
}