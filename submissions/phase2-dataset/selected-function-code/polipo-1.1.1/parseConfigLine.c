int
parseConfigLine(char *line, char *filename, int lineno, int set)
{
    int x0, x1;
    int i, from, to;
    AtomPtr name, value;
    ConfigVariablePtr var;
    int iv;
    float fv;
    AtomPtr av;
    AtomListPtr alv;
    IntListPtr ilv;

    i = skipWhitespace(line, 0);
    if(line[i] == '\n' || line[i] == '\0' || line[i] == '#')
        return 0;

    x0 = i;
    while(letter(line[i]) || digit(line[i]))
        i++;
    x1 = i;

    i = skipWhitespace(line, i);
    if(line[i] != '=') {
        goto syntax;
    }
    i++;
    i = skipWhitespace(line, i);

    name = internAtomN(line + x0, x1 - x0);
    var = findConfigVariable(name);
    releaseAtom(name);

    if(set && var->setter == NULL)
        return -2;
 
    if(var == NULL) {
        if(!set) {
            do_log(L_ERROR, "%s:%d: unknown config variable ",
                   filename, lineno);
            do_log_n(L_ERROR, line + x0, x1 - x0);
            do_log(L_ERROR, "\n");
        }
        return -1;
    }
    
    i = skipWhitespace(line, i);
    switch(var->type) {
    case CONFIG_INT: case CONFIG_OCTAL: case CONFIG_HEX:
        i = parseInt(line, i, &iv);
        if(i < 0) goto syntax;
        if(set)
            var->setter(var, &iv);
        else
            *var->value.i = iv;
    break;
    case CONFIG_TIME:
        i = parseTime(line, i, &iv);
        if(i < 0) goto syntax;
        i = skipWhitespace(line, i);
        if(line[i] != '\n' && line[i] != '\0' && line[i] != '#')
            goto syntax;
        if(set)
            var->setter(var, &iv);
        else
            *var->value.i = iv;
        break;
    case CONFIG_BOOLEAN:
    case CONFIG_TRISTATE:
    case CONFIG_TETRASTATE:
    case CONFIG_PENTASTATE:
        iv = parseState(line, i, var->type);
        if(iv < 0)
            goto syntax;
        if(set)
            var->setter(var, &iv);
        else
            *var->value.i = iv;
        break;
    case CONFIG_FLOAT: 
        if(!digit(line[i]) && line[i] != '.')
            goto syntax;
        fv = atof(line + i);
        if(set)
            var->setter(var, &fv);
        else
            *var->value.f = fv;
        break;
    case CONFIG_ATOM: case CONFIG_ATOM_LOWER: case CONFIG_PASSWORD:
        i = parseAtom(line, i, &av, (var->type == CONFIG_ATOM_LOWER));
        if(i < 0) goto syntax;
        if(!av) {
            if(!set)
                do_log(L_ERROR, "%s:%d: couldn't allocate atom.\n",
                       filename, lineno);
            return -1;
        }
        i = skipWhitespace(line, i);
        if(line[i] != '\n' && line[i] != '\0' && line[i] != '#') {
            releaseAtom(av);
            goto syntax;
        }
        if(set)
            var->setter(var, &av);
        else {
            if(*var->value.a) releaseAtom(*var->value.a);
            *var->value.a = av;
        }
        break;
    case CONFIG_INT_LIST:
        ilv = makeIntList(0);
        if(ilv == NULL) {
            if(!set)
                do_log(L_ERROR, "%s:%d: couldn't allocate int list.\n",
                       filename, lineno);
            return -1;
        }
        while(1) {
            i = parseInt(line, i, &from);
            if(i < 0) goto syntax;
            to = from;
            i = skipWhitespace(line, i);
            if(line[i] == '-') {
                i = skipWhitespace(line, i + 1);
                i = parseInt(line, i, &to);
                if(i < 0) {
                    destroyIntList(ilv);
                    goto syntax;
                }
                i = skipWhitespace(line, i);
            }
            intListCons(from, to, ilv);
            if(line[i] == '\n' || line[i] == '\0' || line[i] == '#')
                break;
            if(line[i] != ',') {
                destroyIntList(ilv);
                goto syntax;
            }
            i = skipWhitespace(line, i + 1);
        }
        if(set)
            var->setter(var, &ilv);
        else {
            if(*var->value.il) destroyIntList(*var->value.il);
            *var->value.il = ilv;
        }
        break;
    case CONFIG_ATOM_LIST: case CONFIG_ATOM_LIST_LOWER:
        alv = makeAtomList(NULL, 0);
        if(alv == NULL) {
            if(!set)
                do_log(L_ERROR, "%s:%d: couldn't allocate atom list.\n",
                       filename, lineno);
            return -1;
        }
        while(1) {
            i = parseAtom(line, i, &value, 
                          (var->type == CONFIG_ATOM_LIST_LOWER));
            if(i < 0) goto syntax;
            if(!value) {
                if(!set)
                    do_log(L_ERROR, "%s:%d: couldn't allocate atom.\n",
                           filename, lineno);
                return -1;
            }
            atomListCons(value, alv);
            i = skipWhitespace(line, i);
            if(line[i] == '\n' || line[i] == '\0' || line[i] == '#')
                break;
            if(line[i] != ',') {
                destroyAtomList(alv);
                goto syntax;
            }
            i = skipWhitespace(line, i + 1);
        }
        if(set)
            var->setter(var, &alv);
        else {
            if(*var->value.al) destroyAtomList(*var->value.al);
            *var->value.al = alv;
        }
        break;
    default: abort();
    }
    return 1;

 syntax:
    if(!set)
        do_log(L_ERROR, "%s:%d: parse error.\n", filename, lineno);
    return -1;
}