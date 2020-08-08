#include <PololuLedStrip.h>
#include <ParticleSet.cpp>

PololuLedStrip<5> strip;
#define LED_COUNT 300
rgb_color colors[LED_COUNT];
rgb_color C_OFF;

void PlayFirework();
void PlayStackFill();
void PlayColoredStars();

void setup() {
  delay(2000);
  randomSeed(analogRead(0));
  Serial1.begin(9600);
}

void loop() {
    //PlayStackFill();
    PlayColoredStars();
    PlayFirework();
}




float f0(float x){
    return -pow(x, 2);
}
float f1(float x){
    return -pow(x, 2);
}
float m(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void PlayFirework(){
    // init floater
    ParticleSet* floater = new ParticleSet(0, 1, 212, 94, 255, 7);
    floater->posStart = 0;
    floater->posEnd = 240;
    floater->xStart = -3;
    floater->xEnd = 0.5;
    floater->yStart = f0(floater->xStart);
    floater->yEnd = f0(floater->xEnd);
    floater->steps = abs((floater->posEnd - floater->posStart) / floater->step) / 1.5;
    floater->xStep = (floater->xEnd - floater->xStart) / floater->steps;
    floater->x = floater->xStart;

    // init explosion sets
    uint16_t NUM_OF_PARTICLE_SETS = 15;
    ParticleSet* sets[NUM_OF_PARTICLE_SETS];
    for(uint8_t i = 0; i < NUM_OF_PARTICLE_SETS; i++){
        uint8_t step = (random(0, 100) > 50) ? -1 : 1;
        sets[i] = new ParticleSet(0, step, 0, random(152, 252), random(152, 252), random(1, 4));
        sets[i]->posStart = floater->posEnd;
        sets[i]->posEnd = -10;
        bool r = random(0,2) == 0;
        sets[i]->xStart = r ? (float)random(50, 70) / 10 : (float)random(-40, 30) / 10;
        sets[i]->xEnd = r ? (float)random(-40, 30) / 10 : (float)random(50, 70) / 10;
        sets[i]->yStart = f1(sets[i]->xStart);
        sets[i]->yEnd = f1(sets[i]->xEnd);
        sets[i]->steps = abs((sets[i]->posEnd - sets[i]->posStart) / sets[i]->step) / ((float)random(10, 20) / 10);
        sets[i]->xStep = (sets[i]->xEnd - sets[i]->xStart) / sets[i]->steps;
        sets[i]->x = sets[i]->xStart;
    }

    
    // travel forward
    while(true){
        if((floater->xStart < floater->xEnd && floater->x > floater->xEnd) 
            || (floater->xStart > floater->xEnd && floater->x < floater->xEnd))
            break;
        int16_t pos = m(f0(floater->x), floater->yStart, floater->yEnd, floater->posStart,  floater->posEnd);
        floater->PutColor(colors, C_OFF);
        floater->SetCount(m(f0(floater->x), floater->yStart, floater->yEnd, 9, 3));
        floater->SetPos(pos);
        floater->PutColor(colors);
        floater->x += floater->xStep;
        strip.write(colors, LED_COUNT);
    }
    floater->PutColor(colors, C_OFF);
    // release instance
    delete floater;

// ***************************************
    

    // explosion
    bool done = false;
    while(!done){
        done = true;
        for(uint8_t i = 0; i < NUM_OF_PARTICLE_SETS; i++){
            if(sets[i]->pos < 0 || LED_COUNT - 1 < sets[i]->pos)
                continue;
            done = false;
            int16_t pos = m(f1(sets[i]->x), sets[i]->yStart, sets[i]->yEnd, sets[i]->posStart, sets[i]->posEnd);
            sets[i]->PutColor(colors, C_OFF);
            sets[i]->SetCount(m(f1(sets[i]->x), sets[i]->yStart, sets[i]->yEnd, 5, 1));
            sets[i]->SetPos(pos);
            sets[i]->PutColor(colors);
            sets[i]->x += sets[i]->xStep;
        }
        strip.write(colors, LED_COUNT);
    }

    // release instances
    for(uint8_t i = 0; i < NUM_OF_PARTICLE_SETS; i++)
        delete sets[i];
}


void PlayStackFill(){
    ParticleSet* floater = new ParticleSet(0, 1, 255, 255, 255, 2);
    for(uint16_t i = 0; i < LED_COUNT; i++){
        for(uint16_t j = 0; j < LED_COUNT - i + 4; j += 4){
            floater->PutColor(colors, C_OFF);
            floater->SetPos(j);
            floater->PutColor(colors);
            strip.write(colors, LED_COUNT);
        }
        for(uint16_t j = LED_COUNT - 1 - i; j < LED_COUNT; j += 1){
            floater->SetPos(j);
            floater->PutColor(colors);
            strip.write(colors, LED_COUNT);
        }
    }
}


void PlayColoredStars(){
    float step = 1;
    while(true){
        step += 1;
        uint16_t p = random(LED_COUNT);
        float r = random(random(255));
        float g = random(random(255));
        float b = random(random(255));
        for(float n = 0; n < 255; n+=step){
            colors[p].red = n * (r / 255);
            colors[p].green = n * (g / 255);
            colors[p].blue = n * (b / 255);
            strip.write(colors, LED_COUNT);
        }
        for(float k = 255; k > 0; k-=step){
            colors[p].red = k * (r / 255);
            colors[p].green = k * (g / 255);
            colors[p].blue = k * (b / 255);
            strip.write(colors, LED_COUNT);
        }
        colors[p].red = 0;
        colors[p].green = 0;
        colors[p].blue = 0;

        if(step > 200)
            break;
    }
}