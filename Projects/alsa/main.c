#include <stdio.h>
#include <stdlib.h>
#include <alsa/asoundlib.h>
#include <stdbool.h>

void mute()
{
    long min, max;
    snd_mixer_t *handle;
    snd_mixer_selem_id_t *sid;
    const char *card = "default";
    const char *selem_name = "Master";

    snd_mixer_open(&handle, 0);
    snd_mixer_attach(handle, card);
    snd_mixer_selem_register(handle, NULL, NULL);
    snd_mixer_load(handle);

    snd_mixer_selem_id_alloca(&sid);
    snd_mixer_selem_id_set_index(sid, 0);
    snd_mixer_selem_id_set_name(sid, selem_name);
    snd_mixer_elem_t* elem = snd_mixer_find_selem(handle, sid);

    //snd_mixer_selem_get_playback_volume_range(elem, &min, &max);
    //snd_mixer_selem_set_playback_volume_all(elem, volume * max / 100);
	
	int state;
	int temp;
	
	for (int i = 0; i <= SND_MIXER_SCHN_LAST;++i){
		snd_mixer_selem_get_playback_switch(elem, i, &temp);	
		if(temp) {
			state = 1;
		} else 
			state = 0;
	}
	snd_mixer_selem_set_playback_switch_all(elem, !state);
    snd_mixer_close(handle);
}

