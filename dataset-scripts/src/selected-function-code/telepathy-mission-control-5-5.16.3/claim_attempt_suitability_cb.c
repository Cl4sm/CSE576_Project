static void
claim_attempt_suitability_cb (GObject *source,
                              GAsyncResult *res,
                              gpointer user_data)
{
    ClaimAttempt *claim_attempt = user_data;
    GError *error = NULL;

    if (!mcp_dispatch_operation_policy_handler_is_suitable_finish (
            MCP_DISPATCH_OPERATION_POLICY (source), res, &error))
    {
        if (claim_attempt->context != NULL)
            dbus_g_method_return_error (claim_attempt->context, error);

        claim_attempt->context = NULL;
        g_error_free (error);
    }

    if (--claim_attempt->handler_suitable_pending == 0)
    {
        DEBUG ("all plugins have finished, resolving claim attempt");
        claim_attempt_resolve (claim_attempt);
    }
}