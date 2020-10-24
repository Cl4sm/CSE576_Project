void am_ClearServicesSt()
{
    int i;
    for (i = 0; i < MAX_APACHE_SERVICES; i++)
    {
        if (g_stServices[i].szServiceName) {
            free(g_stServices[i].szServiceName);
        }
        if (g_stServices[i].szDisplayName) {
            free(g_stServices[i].szDisplayName);
        }
        if (g_stServices[i].szDescription) {
            free(g_stServices[i].szDescription);
        }
        if (g_stServices[i].szImagePath) {
            free(g_stServices[i].szImagePath);
        }
        if (g_stServices[i].szComputerName) {
            free(g_stServices[i].szComputerName);
        }

    }
    memset(g_stServices, 0, sizeof(ST_APACHE_SERVICE) * MAX_APACHE_SERVICES);

}