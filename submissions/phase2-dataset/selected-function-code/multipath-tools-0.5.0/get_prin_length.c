{
        int mx_resp_len;
        switch (rq_servact)
        {
                case MPATH_PRIN_RKEY_SA:
                        mx_resp_len =  sizeof(struct prin_readdescr);
                        break;
                case MPATH_PRIN_RRES_SA :
                        mx_resp_len =  sizeof(struct prin_resvdescr);
                        break;
                case MPATH_PRIN_RCAP_SA :
                        mx_resp_len = sizeof(struct prin_capdescr);
                        break;
                case MPATH_PRIN_RFSTAT_SA:
                        mx_resp_len = sizeof(struct print_fulldescr_list) + sizeof(struct prin_fulldescr *)*32;
                        break;
		default:
			condlog(0, "invalid service action, %d", rq_servact);
			mx_resp_len = 0;
			break;
        }
        return mx_resp_len;
}
