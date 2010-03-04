#ifndef _FONTS_H_
#define _FONTS_H_

class fonts {
    public:

    TTF_Font* fontset_system;
    int color_r;
    int color_g;
    int color_b;


    fonts() :  fontset_system(0)
    {
        if (TTF_Init() == -1) return;
        fontset_system = TTF_OpenFont(make_path("font.ttf"), 20);
        if (!fontset_system) exit(0);
    };



    ~fonts()
    {
        TTF_CloseFont(fontset_system);
        TTF_Quit();
    };

    void change_color(int r, int g, int b)
    {
        color_r = r;
        color_g = g;
        color_b = b;
    };


    int get_length_px(char* t)
    {
        int w,h;
        TTF_SizeText(fontset_system, t, &w, &h);
        return w;
    };


    int text(SDL_Surface* s, const char* textin,int x, int y,int limit_width,int align = 0)
    {

        if (strlen(textin) <=0) return 0;

        SDL_Color tmpfontcolor = {color_r,color_g,color_b,0};

        SDL_Surface *resulting_text = 0;


        resulting_text = TTF_RenderText_Blended(fontset_system, textin, tmpfontcolor); //better/slower

        if (!resulting_text)
            return 0;


        if (align == 1) x = x - get_length_px((char*)textin);

        SDL_Rect ds = { x, y , limit_width ,FONT_SIZE};

        if (limit_width) {

            SDL_Rect sr = { 0, 0 , limit_width ,FONT_SIZE};
            SDL_BlitSurface( resulting_text,&sr, s, &ds );

        } else SDL_BlitSurface( resulting_text,0, s, &ds );

        SDL_FreeSurface(resulting_text);

        return get_length_px((char*)textin); // ????

    };
};

#endif //_FONTS_H_
