static apr_status_t htdbm_save(htdbm_t *htdbm, int *changed)
{
    apr_datum_t key, val;

    if (!htdbm->username)
        return APR_SUCCESS;

    key.dptr = htdbm->username;
    key.dsize = strlen(htdbm->username);
    if (apr_dbm_exists(htdbm->dbm, key))
        *changed = 1;

    val.dsize = strlen(htdbm->ctx.passwd);
    if (!htdbm->comment)
        val.dptr  = htdbm->ctx.passwd;
    else {
        val.dptr = apr_pstrcat(htdbm->ctx.pool, htdbm->ctx.passwd, ":",
                               htdbm->comment, NULL);
        val.dsize += (strlen(htdbm->comment) + 1);
    }
    return apr_dbm_store(htdbm->dbm, key, val);
}