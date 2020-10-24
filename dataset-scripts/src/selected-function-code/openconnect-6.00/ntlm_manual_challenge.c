static int ntlm_manual_challenge(struct openconnect_info *vpninfo, struct oc_text_buf *hdrbuf)
{
	struct oc_text_buf *resp;
	char *user;
	unsigned char nonce[8], hash[21], lm_resp[24], nt_resp[24];
	unsigned char *token;
	int token_len;
	int ntlmver;

	if (!vpninfo->auth[AUTH_TYPE_NTLM].challenge)
		return -EINVAL;

	if (ntlm_nt_hash (vpninfo->proxy_pass, (char *) hash))
		return -EINVAL;

	token_len = openconnect_base64_decode((void **)&token,
					      vpninfo->auth[AUTH_TYPE_NTLM].challenge);
	if (token_len < 0)
		return token_len;

	if (token_len < NTLM_CHALLENGE_NONCE_OFFSET + 8 || token[0] != 'N' ||
	    token[1] != 'T' || token[2] != 'L' || token[3] != 'M' ||
	    token[4] != 'S' || token[5] != 'S' || token[6] != 'P' ||
	    token[7] || token[8] != 2 || token[9] || token[10] || token[11]) {
		free(token);
		return -EINVAL;
	}

	/* 0x00080000: Negotiate NTLM2 Key */
	if (token[NTLM_CHALLENGE_FLAGS_OFFSET + 2] & 8) {
		/* NTLM2 session response */
		struct {
			uint32_t srv[2];
			uint32_t clnt[2];
		} sess_nonce;
		unsigned char digest[16];

		ntlmver = 2;
		if (openconnect_random(sess_nonce.clnt, sizeof(sess_nonce.clnt))) {
			free(token);
			return -EIO;
		}

		/* LM response is 8-byte client nonce, NUL-padded to 24 */
		memcpy (lm_resp, sess_nonce.clnt, 8);
		memset (lm_resp + 8, 0, 16);

		/* Session nonce is client nonce + server nonce */
		memcpy (sess_nonce.srv,
			token + NTLM_CHALLENGE_NONCE_OFFSET, 8);

		/* Take MD5 of session nonce */
		if (openconnect_md5(digest, &sess_nonce, sizeof(sess_nonce))) {
			free(token);
			return -EIO;
		}
		ntlm_calc_response (hash, digest, nt_resp);
	} else {
		/* NTLM1 */
		ntlmver = 1;
		memcpy (nonce, token + NTLM_CHALLENGE_NONCE_OFFSET, 8);
		ntlm_calc_response (hash, nonce, nt_resp);
		ntlm_lanmanager_hash (vpninfo->proxy_pass, (char *) hash);
		ntlm_calc_response (hash, nonce, lm_resp);
	}

	resp = buf_alloc();
	buf_append_bytes(resp, ntlm_response_base, sizeof(ntlm_response_base));
	if (buf_error(resp)) {
		free(token);
		return buf_free(resp);
	}
	/* Mask in the NTLM2SESSION flag */
	resp->data[NTLM_RESPONSE_FLAGS_OFFSET + 2] = token[NTLM_CHALLENGE_FLAGS_OFFSET + 2] & 8;

	user = strchr(vpninfo->proxy_user, '\\');
	if (user) {
		*user = 0;
		ntlm_set_string_utf8(resp, NTLM_RESPONSE_DOMAIN_OFFSET, vpninfo->proxy_user);
		*user = '\\';
		user++;
	} else {
		int offset = load_le32(token + NTLM_CHALLENGE_DOMAIN_OFFSET + 4);
		int len = load_le16(token + NTLM_CHALLENGE_DOMAIN_OFFSET);
		if (!len || offset + len >= token_len) {
			free(token);
			buf_free(resp);
			return -EINVAL;
		}
		ntlm_set_string_binary(resp, NTLM_RESPONSE_DOMAIN_OFFSET, token + offset, len);

		user = vpninfo->proxy_user;
	}

	ntlm_set_string_utf8(resp, NTLM_RESPONSE_USER_OFFSET, user);
	ntlm_set_string_utf8(resp, NTLM_RESPONSE_HOST_OFFSET, "UNKNOWN");
	ntlm_set_string_binary(resp, NTLM_RESPONSE_LM_RESP_OFFSET, lm_resp, sizeof(lm_resp));
	ntlm_set_string_binary(resp, NTLM_RESPONSE_NT_RESP_OFFSET, nt_resp, sizeof(nt_resp));

	free(token);

	if (buf_error(resp))
		return buf_free(resp);

	buf_append(hdrbuf, "Proxy-Authorization: NTLM ");
	buf_append_base64(hdrbuf, resp->data, resp->pos);
	buf_append(hdrbuf, "\r\n");

	buf_free(resp);
	vpn_progress(vpninfo, PRG_INFO,
		     _("Attempting HTTP NTLMv%d authentication to proxy\n"),
		     ntlmver);
	return 0;
}