static void set_style_default(StyleDef opt) {
    char *string1 = NULL;
    char *string2 = NULL;

    switch (opt.type) {
        case T_INT:
            *(int *)(opt.var) = *(int *)(opt.defval);
            break;
        case T_BOOL:
            *(Bool *)(opt.var) = *(Bool *)(opt.defval);
            break;
        case T_STRING:
            string1 = *(char **)(opt.var);
            string2 = *(char **)(opt.defval);
            SET_STRING(string1, string2);
            *(char **)(opt.var) = string1;
            break;
        case T_FILE:
            string1 = *(char **)(opt.var);
            string2 = set_filename(*(char **)(opt.defval), NULL);
            SET_STRING(string1, string2);
            *(char **)(opt.var) = string1;
            FREE(string2);
            break;
    }
}