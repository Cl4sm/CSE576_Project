static GList *
_mcd_request_policy_plugins (void)
{
  static gboolean cached = FALSE;
  static GList *policies = NULL;

  if (G_UNLIKELY (!cached))
    {
      const GList *p = NULL;

      for (p = mcp_list_objects (); p != NULL; p = g_list_next (p))
        {
          if (MCP_IS_REQUEST_POLICY (p->data))
            policies = g_list_prepend (policies, g_object_ref (p->data));
        }

      cached = TRUE;
    }

  return policies;
}