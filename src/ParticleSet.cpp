#include <stdio.h>
#include <PololuLedStrip.h>

class ParticleSet{
    public :
        int16_t pos;
        rgb_color rgb;
        int8_t step;
        uint8_t count;
        
        int16_t posStart;
        int16_t posEnd;
        float xStart;
        float xEnd;
        float yStart;
        float yEnd;
        float steps;
        float xStep;
        float x;
    
    ParticleSet(uint16_t pos = 0, int8_t step = 1, uint8_t red = 0, uint8_t green = 0, uint8_t blue = 0, uint8_t count = 1){
        this->pos = pos;
        this->rgb = rgb_color(red, green, blue);
        this->step = step;
        this->count = count;
    }

    void SetPos(uint16_t pos){
        if(pos < 0)
            pos = -1;
        this->pos = pos;
    }

    void SetCount(uint8_t count){
        this->count = count;
    }

    void SetColor(uint8_t red, uint8_t green, uint8_t blue){
        this->rgb = rgb_color(red, green, blue);
    }

    void PutColor(rgb_color colors[]){
        for(uint8_t i = 0; i < this->count; i++)
            colors[this->pos + i] = this->rgb;
    }
    void PutColor(rgb_color colors[], rgb_color color){
        for(uint8_t i = 0; i < this->count; i++)
            colors[this->pos + i] = color;
    }

};