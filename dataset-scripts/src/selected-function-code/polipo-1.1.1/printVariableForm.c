static void
printVariableForm(FILE *out, ConfigVariablePtr var)
{
    char *disabled = "";
    int i;
    
    if(disableConfiguration || !var->setter) disabled = "disabled=true";

    fprintf(out, "<form method=POST action=\"config?\">");
  
    switch(var->type) {
    case CONFIG_INT: case CONFIG_OCTAL: case CONFIG_HEX:
    case CONFIG_TIME: case CONFIG_FLOAT: case CONFIG_ATOM:
    case CONFIG_ATOM_LOWER: case CONFIG_PASSWORD:
    case CONFIG_INT_LIST: case CONFIG_ATOM_LIST: case CONFIG_ATOM_LIST_LOWER:
        fprintf(out, "<input value=\"");
        printVariable(out, var, 1, 1);
        fprintf(out, "\"%s size=14 name=%s %s>\n",
                var->type == CONFIG_PASSWORD ? " type=password" : "",
                var->name->string, disabled);
        break;

    case CONFIG_BOOLEAN:
        {
            static char *states[] = {"false", "true"};
            
            fprintf(out, "<select name=%s %s>", var->name->string, disabled);
            for(i=0; i < sizeof(states) / sizeof(states[0]); i++) {
                if(*var->value.i == i) {
                    fprintf(out, "<option selected>%s</option>", states[i]);
                } else {
                    fprintf(out, "<option>%s</option>", states[i]);
                }
            }
            fprintf(out, "</select>");
            if(var->setter)
                fprintf(out, "<input type=\"submit\" value=\"set\"\n>");
            break;
        }
    
    case CONFIG_TRISTATE:
        {
            static char *states[] = {"false", "maybe", "true"};
            
            fprintf(out, "<select name=%s %s>", var->name->string, disabled);
            for(i=0; i < sizeof(states) / sizeof(states[0]); i++) {
                if(*var->value.i == i) {
                    fprintf(out, "<option selected>%s</option>", states[i]);
                } else {
                    fprintf(out, "<option>%s</option>", states[i]);
                }
            }
            fprintf(out, "</select>");
            if(var->setter)
                fprintf(out, "<input type=\"submit\" value=\"set\"\n>");
            break;
        }

    case CONFIG_TETRASTATE:
        {
            static char *states[] =
                {"false", "reluctantly", "happily", "true"};
            
            fprintf(out, "<select name=%s %s>", var->name->string, disabled);
            for(i=0; i <sizeof(states) / sizeof(states[0]); i++) {
                if(*var->value.i == i) {
                    fprintf(out, "<option selected>%s</option>", states[i]);
                } else {
                    fprintf(out, "<option>%s</option>", states[i]);
                }
            }
            fprintf(out, "</select>");
            if(var->setter)
                fprintf(out, "<input type=\"submit\" value=\"set\"\n>");
            break;
        }

    case CONFIG_PENTASTATE:
        {
            static char *states[] =
                {"no", "reluctantly", "maybe", "happily", "true"};

            fprintf(out, "<select name=%s %s>", var->name->string, disabled);
            for(i=0; i < sizeof(states) / sizeof(states[0]); i++) {
                if(*var->value.i == i) {
                    fprintf(out, "<option selected>%s</option>", states[i]);
                } else {
                    fprintf(out, "<option>%s</option>", states[i]);
                }
            }
            fprintf(out, "</select>");
            if(var->setter)
                fprintf(out,"<input type=\"submit\" value=\"set\"\n>");
            break;
        }
    default: abort();
    }
    fprintf(out, "</form>");
}