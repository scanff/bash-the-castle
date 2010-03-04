#ifndef SOUND_H_INCLUDED
#define SOUND_H_INCLUDED

#define MAX_SOUNDS 100
#define MAXCHAN 32
#define SOUNDFREQ 48000
#define MAXVOL MIX_MAX_VOLUME

class sound_cache {

    public:

    unsigned long sound_id[MAX_SOUNDS];
    Mix_Chunk*  sounds[MAX_SOUNDS];
    Mix_Music*  music;
    char        music_fn[255];
    bool        can_play_sounds;
    bool        can_play_music;
    bool        sound_error;
    int         volume;


    sound_cache() : music(0), can_play_sounds(true), can_play_music(true),
    sound_error(false), volume(MAXVOL)
    {
        memset(music_fn,0,255);

        loopi(MAX_SOUNDS)
        {
          sound_id[i] = 0;
          sounds[i] = 0;
        }

        if(Mix_OpenAudio(SOUNDFREQ, AUDIO_S16MSB, 2, 515) < 0)
        {
            sound_error = true;
            return;
        }

        Mix_AllocateChannels(MAXCHAN);
    };

    ~sound_cache()
    {
        if(music)
        {
           Mix_HaltMusic();
           Mix_FreeMusic(music);
           music = 0;
        }

        loopi(MAX_SOUNDS)
        {
            if (sounds[i])
            {
                Mix_FreeChunk(sounds[i]);
                sounds[i] = 0;
            }
        }

        Mix_CloseAudio();

    };

    Mix_Chunk* load_sound(char* name)
    {
        if (!name) return 0;
        if (sound_error) return 0;

        unsigned long hash = 0;
        const char* ptr = name;

        while (*ptr)
            hash = (hash << 5) + hash + *ptr++;

        loopi(MAX_SOUNDS) {
            if (hash == sound_id[i])
                return sounds[i]; //already loaded return
        }

        //find a spot to load the sound
        loopi(MAX_SOUNDS)
        {
            if (!sound_id[i])
            {
                sound_id[i] = hash;
                sounds[i] = Mix_LoadWAV(make_path(name));
                return sounds[i];
            }
        }

        return 0;


    };

    void volume_up()
    {
       volume+=5;
       set_volume();
    };

    void volume_down()
    {
       volume-=5;
       set_volume();
    };

    void set_volume()
    {
        if (sound_error) return;

        if (volume < 0) volume = 0;
        if (volume > MAXVOL)volume = MAXVOL;

        Mix_VolumeMusic(volume);

    };

    void play_sound(char* name)
    {
        if (!can_play_sounds || sound_error) return;

        Mix_Chunk* s = load_sound(name);
        if (!s) return;

        int chan = Mix_PlayChannel(-1, s, 0);

        if(chan < 0) return;

        Mix_VolumeChunk(s, volume);

    };

    // only allow one music at a time
    void play_music(char* name,bool playlast = false)
    {
        if (!name) return;
        if (!can_play_music || sound_error) return;

        if (!playlast) if(strcmp(music_fn,name)==0) return; // already playing

        if(music) // already playing ?
        {
           Mix_HaltMusic();
           Mix_FreeMusic(music);
           music = 0;
        }

        if((music = Mix_LoadMUS(make_path(name))))
		{
			Mix_PlayMusic(music, -1);
			Mix_VolumeMusic(MAXVOL);
			strcpy(music_fn,name);
		}
    };

    void stop_sound(char* name)
    {
        return; // TODO
        if (sound_error) return;

        unsigned long hash = 0;
        const char* ptr = name;

        while (*ptr)
            hash = (hash << 5) + hash + *ptr++;

        loopi(MAX_SOUNDS)
        {
            if (hash == sound_id[i])
                if (Mix_Playing(sound_id[i])) Mix_HaltChannel(sound_id[i]);
        }

    };

    void stop_music()
    {
        if (sound_error) return;

        if(music) // already playing ?
        {
           Mix_HaltMusic();
           Mix_FreeMusic(music);
           music = 0;
        }

    };

};




#endif // SOUND_H_INCLUDED
