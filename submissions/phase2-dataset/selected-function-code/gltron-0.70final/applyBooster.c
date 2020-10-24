	Data *data = game->player[player].data;
	if(data->booster > 0 && data->boost_enabled) {
		float boost = getSettingf("booster_use") * dt / 1000.0f;
		if(boost > data->booster) {
			boost = data->booster;
			data->boost_enabled = 0;
		}
		data->speed += boost;
		data->booster -= boost;
		return 1;
	}
	else {
		float booster_max = getSettingf("booster_max");
		if(data->booster < booster_max) {
			data->booster += getSettingf("booster_regenerate") * dt / 1000.0f;
			if(data->booster > booster_max)
				data->booster = booster_max;
		}
		return 0;
	}
}
