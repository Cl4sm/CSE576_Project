static void *merge_env_dir_configs(apr_pool_t *p, void *basev, void *addv)
{
    env_dir_config_rec *base = basev;
    env_dir_config_rec *add = addv;
    env_dir_config_rec *res = apr_palloc(p, sizeof(*res));

    const apr_table_entry_t *elts;
    const apr_array_header_t *arr;

    int i;

    /*
     * res->vars = copy_table( p, base->vars );
     * foreach $unsetenv ( @add->unsetenv )
     *     table_unset( res->vars, $unsetenv );
     * foreach $element ( @add->vars )
     *     table_set( res->vars, $element.key, $element.val );
     *
     * add->unsetenv already removed the vars from add->vars,
     * if they preceeded the UnsetEnv directive.
     */
    res->vars = apr_table_copy(p, base->vars);
    res->unsetenv = NULL;

    arr = apr_table_elts(add->unsetenv);
    if (arr) {
        elts = (const apr_table_entry_t *)arr->elts;

        for (i = 0; i < arr->nelts; ++i) {
            apr_table_unset(res->vars, elts[i].key);
        }
    }

    arr = apr_table_elts(add->vars);
    if (arr) {
        elts = (const apr_table_entry_t *)arr->elts;

        for (i = 0; i < arr->nelts; ++i) {
            apr_table_setn(res->vars, elts[i].key, elts[i].val);
        }
    }

    return res;
}