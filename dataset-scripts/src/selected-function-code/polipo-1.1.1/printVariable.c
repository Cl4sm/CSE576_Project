static void
printVariable(FILE *out, ConfigVariablePtr var, int html, int parseable)
{
    int i;

    switch(var->type) {
    case CONFIG_INT: fprintf(out, "%d", *var->value.i); break;
    case CONFIG_OCTAL: fprintf(out, "0%o", *var->value.i); break;
    case CONFIG_HEX: fprintf(out, "0x%x", *var->value.i); break;
    case CONFIG_TIME:
        {
            int v = *var->value.i;
            if(v == 0) {
                fprintf(out, "0s");
            } else {
                if(v >= 3600 * 24) fprintf(out, "%dd", v/(3600*24));
                v = v % (3600 * 24);
                if(v >= 3600) fprintf(out, "%dh", v / 3600);
                v = v % 3600;
                if(v >= 60) fprintf(out, "%dm", v / 60);
                v = v % 60;
                if(v > 0) fprintf(out, "%ds", v);
            }
        }
        break;
    case CONFIG_BOOLEAN:
        switch(*var->value.i) {
        case 0: fprintf(out, "false"); break;
        case 1: fprintf(out, "true"); break;
        default: fprintf(out, "???"); break;
        }
        break;
    case CONFIG_TRISTATE:
        switch(*var->value.i) {
        case 0: fprintf(out, "false"); break;
        case 1: fprintf(out, "maybe"); break;
        case 2: fprintf(out, "true"); break;
        default: fprintf(out, "???"); break;
        }
        break;
    case CONFIG_TETRASTATE:
        switch(*var->value.i) {
        case 0: fprintf(out, "false"); break;
        case 1: fprintf(out, "reluctantly"); break;
        case 2: fprintf(out, "happily"); break;
        case 3: fprintf(out, "true"); break;
        default: fprintf(out, "???"); break;
        }
        break;
    case CONFIG_PENTASTATE:
        switch(*var->value.i) {
        case 0: fprintf(out, "no"); break;
        case 1: fprintf(out, "reluctantly"); break;
        case 2: fprintf(out, "maybe"); break;
        case 3: fprintf(out, "happily"); break;
        case 4: fprintf(out, "true"); break;
        default: fprintf(out, "???"); break;
        }
        break;
    case CONFIG_FLOAT: fprintf(out, "%f", *var->value.f); break;
    case CONFIG_ATOM: case CONFIG_ATOM_LOWER:
        if(*var->value.a) {
            if((*var->value.a)->length > 0) {
                printString(out, (*var->value.a)->string, html);
            } else {
                if(!parseable)
                    fprintf(out, "(empty)");
            }
        } else {
            if(!parseable)
                fprintf(out, "(none)");
        }
        break;
    case CONFIG_PASSWORD:
        if(!parseable)
            fprintf(out, "(hidden)");
        break;
    case CONFIG_INT_LIST:
        if((*var->value.il) == NULL) {
            if(!parseable)
                fprintf(out, "(not set)");
        } else if((*var->value.il)->length == 0) {
            if(!parseable)
                fprintf(out, "(empty list)");
        } else {
            for(i = 0; i < (*var->value.il)->length; i++) {
                int from = (*var->value.il)->ranges[i].from;
                int to = (*var->value.il)->ranges[i].to;
                assert(from <= to);
                if(from == to)
                    fprintf(out, "%d", from);
                else
                    fprintf(out, "%d-%d", from, to);
                if(i < (*var->value.il)->length - 1)
                    fprintf(out, ", ");
            }
        }
        break;
    case CONFIG_ATOM_LIST: case CONFIG_ATOM_LIST_LOWER:
        if((*var->value.al) == NULL) {
            if(!parseable)
                fprintf(out, "(not set)");
        } else if((*var->value.al)->length == 0) {
            if(!parseable)
                fprintf(out, "(empty list)");
        } else {
            for(i = 0; i < (*var->value.al)->length; i++) {
                AtomPtr atom = (*var->value.al)->list[i];
                if(atom) {
                    if(atom->length > 0)
                        printString(out, atom->string, html);
                    else {
                        if(!parseable)
                            fprintf(out, "(empty)");
                    }
                } else {
                    if(!parseable)
                        fprintf(out, "(none)");
                }
                if(i < (*var->value.al)->length - 1)
                    fprintf(out, ", ");
            }
        }
        break;
    default: abort();
    }
}