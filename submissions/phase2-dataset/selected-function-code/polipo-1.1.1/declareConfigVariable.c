void
declareConfigVariable(AtomPtr name, int type, void *value, 
                      int (*setter)(ConfigVariablePtr, void*), char *help)
{
    ConfigVariablePtr var, previous, next;

    var = findConfigVariable(name);

    if(var) {
        do_log(L_ERROR, 
               "Configuration variable %s declared multiple times.\n",
               name->string);
        if(var->type != type) {
            exit(1);
        }
    }

    var = malloc(sizeof(ConfigVariableRec));
    if(var == NULL) {
        do_log(L_ERROR, "Couldn't allocate config variable.\n");
        exit(1);
    }

    var->name = retainAtom(name);
    var->type = type;
    switch(type) {
    case CONFIG_INT: case CONFIG_OCTAL: case CONFIG_HEX: case CONFIG_TIME:
    case CONFIG_BOOLEAN: case CONFIG_TRISTATE: case CONFIG_TETRASTATE:
    case CONFIG_PENTASTATE:
        var->value.i = value; break;
    case CONFIG_FLOAT: var->value.f = value; break;
    case CONFIG_ATOM: case CONFIG_ATOM_LOWER: case CONFIG_PASSWORD:
        var->value.a = value; break;
    case CONFIG_INT_LIST:
        var->value.il = value; break;
    case CONFIG_ATOM_LIST: case CONFIG_ATOM_LIST_LOWER: 
        var->value.al = value; break;
    default: abort();
    }
    var->setter = setter;
    var->help = help;

    previous = NULL;
    next = configVariables;
    while(next && strcmp(next->name->string, var->name->string) < 0) {
        previous = next;
        next = next->next;
    }
    if(next && strcmp(next->name->string, var->name->string) == 0) {
        do_log(L_ERROR, "Variable %s declared multiple times.\n",
               next->name->string);
        abort();
    }
    if(previous == NULL) {
        var->next = configVariables;
        configVariables = var;
    } else {
        var->next = next;
        previous->next = var;
    }
}