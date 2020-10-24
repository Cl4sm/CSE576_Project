static Bool set_style_value(StyleDef opt, char *value) {
    Bool res = True;
    char *string1 = NULL;
    char *string2 = NULL;

    switch (opt.type) {
        case T_INT:
            *(int *)(opt.var) = atoi(value);
            break;
        case T_BOOL:
            *(Bool *)(opt.var) = getbool(value);
            break;
        case T_STRING:
            string1 = *(char **)(opt.var);
            SET_STRING(string1, value);
            *(char **)(opt.var) = string1;
            break;
        case T_FILE:
            string1 = *(char **)(opt.var);
            string2 = set_filename(value, NULL);
            SET_STRING(string1, string2);
            *(char **)(opt.var) = string1;
            FREE(string2);
            break;
        default: res = False;
    }
    return res;
}