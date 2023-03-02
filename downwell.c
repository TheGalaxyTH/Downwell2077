#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_mixer.h"

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

typedef struct Image{
    SDL_Surface *surface;
    SDL_Texture *texture;
    SDL_Rect rect;
}Image;

typedef struct Font{
    TTF_Font *font;
    SDL_Surface *surface;
    SDL_Texture *texture;
    SDL_Rect rect;
    SDL_Color color;
}Font;

Image start1={NULL,NULL};
Image start2={NULL,NULL};
Image start3={NULL,NULL};
Image gameBG={NULL,NULL};
Image tower={NULL,NULL};
Image normalL={NULL,NULL};
Image normalR={NULL,NULL};
Image jumpingL={NULL,NULL};
Image jumpingR={NULL,NULL};
Image firingL={NULL,NULL};
Image firingR={NULL,NULL};
Image bullet={NULL,NULL};
Image block1={NULL,NULL};
Image block2={NULL,NULL};
Image block3={NULL,NULL};
Image bulletExplode={NULL,NULL};
Image gate={NULL,NULL};
Image portal={NULL,NULL};
Image walk0L={NULL,NULL};
Image walk1L={NULL, NULL};
Image fly0L={NULL, NULL};
Image fly1L={NULL, NULL};
Image walk0R={NULL,NULL};
Image walk1R={NULL, NULL};
Image fly0R={NULL, NULL};
Image fly1R={NULL, NULL};
Image energyMine={NULL, NULL};
Image goldMine={NULL, NULL};
Image deathUI={NULL, NULL};
Image pauseUI={NULL,NULL};
Image menuBox={NULL,NULL};
Image energyBar={NULL,NULL};
Image hpBar={NULL,NULL};
Image shop={NULL,NULL};
Image mineRoom={NULL,NULL};
Image itemBox={NULL,NULL};
Image gear[11];
Image items[3];
Image gearSelect = {NULL,NULL};
Image gearBox = {NULL,NULL};
Image welcome = {NULL,NULL};
Image bossImage[6];
Image abilityUI = {NULL,NULL};
Image endBG = {NULL,NULL};
Mix_Music* BGM = NULL;
Mix_Chunk* chunks[5];
Font microsoftBlack = {NULL, NULL, NULL};

int screenWidth = 1920;
int screenHeight = 1080;
int level = 1;
int HP = 8;
int maxHP= 8;
int maxCharge = 8;
int charge = 8;
int bulletDamage=2;
int money = 0;
int combo = 0;
int haveGear[11] = {0};
int bossHPmax = 2400;
int bossHP = 2400;
clock_t abilityTimer;
int abilityDuration;
const int abilityCD = 60 * CLOCKS_PER_SEC;

void QUIT();
void loadImage(Image *image, const char *filename);
void presentImage(Image *image, int x, int y);
void presentAlphaImage(Image *image,double alpha, int x, int y);
void loadFont(Font *font, const char *filename, int fontSize, int r, int g, int b, int a);
void presentFont(Font *font, const char *text, int x, int y);
void loadAll();
void start();
void init();
int gamePlay();
void createRect(SDL_Rect *rect,int x,int y,int w,int h);
void mainBody();
int death();
int pause();
void infoPresent(int isInRoom);
void gearSelectUI();
void shopUI(int side);
void mineUI(int *haveMine, int side, int mineType);
int endUI();


int SDL_main(int argc, char *argv[]){ // main函数
    loadAll();
    if (Mix_PlayingMusic() == 0){
        Mix_PlayMusic(BGM, -1);
    }
    start();
    init();
    mainBody();
    SDL_Quit();
    return 0;
}

void QUIT(){    // 释放内存并退出
    level = -1;
    // SDL_FreeSurface()
    SDL_FreeSurface(start1.surface);
    SDL_FreeSurface(start2.surface);
    SDL_FreeSurface(start3.surface);
    SDL_FreeSurface(abilityUI.surface);
    SDL_FreeSurface(gameBG.surface);
    SDL_FreeSurface(tower.surface);
    SDL_FreeSurface(normalL.surface);
    SDL_FreeSurface(normalR.surface);
    SDL_FreeSurface(jumpingL.surface);
    SDL_FreeSurface(jumpingR.surface);
    SDL_FreeSurface(firingL.surface);
    SDL_FreeSurface(firingR.surface);
    SDL_FreeSurface(bullet.surface);
    SDL_FreeSurface(block1.surface);
    SDL_FreeSurface(block2.surface);
    SDL_FreeSurface(block3.surface);
    SDL_FreeSurface(bulletExplode.surface);
    SDL_FreeSurface(gate.surface);
    SDL_FreeSurface(portal.surface);
    SDL_FreeSurface(walk0L.surface);
    SDL_FreeSurface(walk1L.surface);
    SDL_FreeSurface(fly0L.surface);
    SDL_FreeSurface(fly1L.surface);
    SDL_FreeSurface(walk0R.surface);
    SDL_FreeSurface(walk1R.surface);
    SDL_FreeSurface(fly0R.surface);
    SDL_FreeSurface(fly1R.surface);
    SDL_FreeSurface(energyMine.surface);
    SDL_FreeSurface(goldMine.surface);
    SDL_FreeSurface(deathUI.surface);
    SDL_FreeSurface(pauseUI.surface);
    SDL_FreeSurface(menuBox.surface);
    SDL_FreeSurface(gearSelect.surface);
    SDL_FreeSurface(gearBox.surface);
    SDL_FreeSurface(welcome.surface);
    SDL_FreeSurface(endBG.surface);
    for(int i = 0; i < 11; i++){
        SDL_FreeSurface(gear[i].surface);
    }
    for(int i = 0; i <3; i++){
        SDL_FreeSurface(items[i].surface);
    }
    for(int i = 0; i < 6; i++){
        SDL_FreeSurface(bossImage[i].surface);
    }
    // SDL_DestroyTexture()
    SDL_DestroyTexture(start1.texture);
    SDL_DestroyTexture(start2.texture);
    SDL_DestroyTexture(start3.texture);
    SDL_DestroyTexture(abilityUI.texture);
    SDL_DestroyTexture(gameBG.texture);
    SDL_DestroyTexture(tower.texture);
    SDL_DestroyTexture(normalL.texture);
    SDL_DestroyTexture(normalR.texture);
    SDL_DestroyTexture(jumpingL.texture);
    SDL_DestroyTexture(jumpingR.texture);
    SDL_DestroyTexture(firingL.texture);
    SDL_DestroyTexture(firingR.texture);
    SDL_DestroyTexture(bullet.texture);
    SDL_DestroyTexture(block1.texture);
    SDL_DestroyTexture(block2.texture);
    SDL_DestroyTexture(block3.texture);
    SDL_DestroyTexture(bulletExplode.texture);
    SDL_DestroyTexture(gate.texture);
    SDL_DestroyTexture(portal.texture);
    SDL_DestroyTexture(walk0L.texture);
    SDL_DestroyTexture(walk1L.texture);
    SDL_DestroyTexture(fly0L.texture);
    SDL_DestroyTexture(fly1L.texture);
    SDL_DestroyTexture(walk0R.texture);
    SDL_DestroyTexture(walk1R.texture);
    SDL_DestroyTexture(fly0R.texture);
    SDL_DestroyTexture(fly1R.texture);
    SDL_DestroyTexture(energyMine.texture);
    SDL_DestroyTexture(goldMine.texture);
    SDL_DestroyTexture(deathUI.texture);
    SDL_DestroyTexture(pauseUI.texture);
    SDL_DestroyTexture(menuBox.texture);
    SDL_DestroyTexture(gearSelect.texture);
    SDL_DestroyTexture(gearBox.texture);
    SDL_DestroyTexture(welcome.texture);
    SDL_DestroyTexture(endBG.texture);
    for(int i = 0; i < 11; i++){
        SDL_DestroyTexture(gear[i].texture);
    }
    for(int i = 0; i < 3; i++){
        SDL_DestroyTexture(items[i].texture);
    }
    for(int i = 0; i < 6; i++){
        SDL_DestroyTexture(bossImage[i].texture);
    }
    // 音乐
    Mix_HaltMusic();
    Mix_FreeMusic(BGM);
    for(int i = 0; i <5; i++){
        Mix_FreeChunk(chunks[i]);
    }
    Mix_Quit();
    // 其他
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void loadImage(Image *image, const char *filename){     // 加载图片
    image->surface = IMG_Load(filename);
    image->texture = SDL_CreateTextureFromSurface(renderer,image->surface);
    image->rect.w = (int)((image->surface->w)*(screenWidth/1920.0));
    image->rect.h = (int)((image->surface->h)*(screenHeight/1080.0));
}

void loadFont(Font *font, const char *filename, int fontSize, int r, int g, int b, int a){  // 加载字体
    font->font = TTF_OpenFont(filename, fontSize);
    font->color.r = (unsigned char)r;
    font->color.g = (unsigned char)g;
    font->color.b = (unsigned char)b;
    font->color.a = (unsigned char)a;
}

void presentImage(Image *image, int x, int y){  // 显示图片
    image->rect.x = x;
    image->rect.y = y;
    SDL_RenderCopy(renderer, image->texture, NULL, &(image->rect));
}

void presentAlphaImage(Image *image,double alpha, int x, int y){    // 显示半透明的图片
    SDL_SetTextureBlendMode(image->texture,SDL_BLENDMODE_BLEND);
    SDL_SetTextureAlphaMod(image->texture, alpha);
    image->rect.x = x;
    image->rect.y = y;
    SDL_RenderCopy(renderer, image->texture, NULL, &(image->rect));
    SDL_SetTextureAlphaMod(image->texture, 255);
}

void presentFont(Font *font, const char *text, int x, int y){   // 显示字体
    font->surface = TTF_RenderUTF8_Blended(font->font, text, font->color);
    font->texture = SDL_CreateTextureFromSurface(renderer, font->surface);
    font->rect.x = x;
    font->rect.y = y;
    font->rect.w = (int)((font->surface->w)*(screenWidth/1920.0));
    font->rect.h = (int)((font->surface->h)*(screenHeight/1080.0));
    SDL_RenderCopy(renderer, font->texture, NULL, &(font->rect));
    SDL_FreeSurface(font->surface);
    SDL_DestroyTexture(font->texture);
    TTF_CloseFont(font->font);
}

void loadAll(){     // 加载所有资源
    SetConsoleOutputCP(65001); // 使用UTF-8字符编码
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    screenWidth = GetSystemMetrics(SM_CXSCREEN);
    screenHeight = GetSystemMetrics(SM_CYSCREEN);
    window = SDL_CreateWindow("Downwell",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,
                              screenWidth,screenHeight,SDL_WINDOW_SHOWN);
    SDL_RenderSetLogicalSize(renderer, screenWidth, screenHeight);
    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
    renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
    Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_CHANNELS, 4096);

    loadImage(&start2,"res/BG/start2.jpg");
    loadImage(&start3,"res/BG/start3.png");
    loadImage(&start1,"res/BG/start1.png");
    loadImage(&abilityUI,"res/BG/abilityUI.png");
    loadImage(&tower,"res/BG/tower.png");
    loadImage(&gameBG, "res/BG/gameBG.png");
    loadImage(&normalL,"res/lucy/normalL.png");
    loadImage(&normalR,"res/lucy/normalR.png");
    loadImage(&jumpingL,"res/lucy/jumpingL.png");
    loadImage(&jumpingR,"res/lucy/jumpingR.png");
    loadImage(&firingL,"res/lucy/firingL.png");
    loadImage(&firingR,"res/lucy/firingR.png");
    loadImage(&bullet,"res/lucy/bullet.png");
    loadImage(&bulletExplode,"res/lucy/bulletExplode.png");
    loadImage(&block1,"res/block/block1.png");
    loadImage(&block2,"res/block/block2.png");
    loadImage(&block3,"res/block/block3.png");
    loadImage(&gate,"res/BG/gate.png");
    loadImage(&portal,"res/block/portal.png");
    loadImage(&walk0L,"res/enemy/walk0L.png");
    loadImage(&walk1L,"res/enemy/walk1L.png");
    loadImage(&fly0L,"res/enemy/fly0L.png");
    loadImage(&fly1L,"res/enemy/fly1L.png");
    loadImage(&walk0R,"res/enemy/walk0R.png");
    loadImage(&walk1R,"res/enemy/walk1R.png");
    loadImage(&fly0R,"res/enemy/fly0R.png");
    loadImage(&fly1R,"res/enemy/fly1R.png");
    loadImage(&energyMine, "res/enemy/energyMine.png");
    loadImage(&goldMine,"res/enemy/goldMine.png");
    loadImage(&deathUI,"res/BG/deathUI.png");
    loadImage(&pauseUI,"res/BG/pauseUI.png");
    loadImage(&menuBox, "res/BG/menuBox.png");
    loadImage(&itemBox, "res/BG/itemBox.png");
    loadImage(&shop, "res/BG/shop.png");
    loadImage(&mineRoom, "res/BG/mineRoom.png");
    loadImage(&energyBar,"res/lucy/energyBar.png");
    loadImage(&hpBar,"res/lucy/hpBar.png");
    loadImage(&gearSelect,"res/BG/gearSelect.png");
    loadImage(&gearBox,"res/item/gearBox.png");
    loadImage(&welcome,"res/BG/welcome.png");
    loadImage(&endBG, "res/BG/endBG.png");
    for(int i = 0; i < 11; i++){
        char directory[20];
        sprintf(directory,"res/item/%02d.png",i);
        loadImage(&gear[i], directory);
    }
    for(int i = 0; i < 3; i++){
        char directory[20];
        sprintf(directory,"res/item/%03d.png",i);
        loadImage(&items[i], directory);
    }
    for(int i = 0; i < 6; i++){
        char directory[20];
        sprintf(directory,"res/enemy/boss/%02d.png",i);
        loadImage(&bossImage[i], directory);
    }
    // 声音
    BGM = Mix_LoadMUS("res/music/BGM.mp3");
    for(int i = 0; i < 5; i++){
        char directory[20];
        sprintf(directory,"res/music/%02d.mp3",i);
        chunks[i] = Mix_LoadWAV(directory);
    }
}

void start(){   // 进入游戏界面
    SDL_RenderClear(renderer);
    presentImage(&start1,0,0);
    SDL_RenderPresent(renderer);
    SDL_Delay(2000);
    SDL_RenderClear(renderer);
    presentImage(&start2,0,0);
    SDL_RenderPresent(renderer);
    SDL_Delay(2000);
    SDL_RenderClear(renderer);
    presentImage(&start3,0,0);
    SDL_RenderPresent(renderer);
    while(1) {
        SDL_Event startEvent;
        SDL_PollEvent(&startEvent);
        switch (startEvent.type) {
            case SDL_QUIT:
                QUIT();
                break;
            case SDL_KEYDOWN:
                switch (startEvent.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        QUIT();
                        break;
                    case SDLK_SPACE:
                        return;
                    default:
                        break;
                }
                break;
            default:
                break;
        }
    }
}

void init(){    // 初始化各项属性
    maxHP = 8;
    HP = maxHP;
    maxCharge = 8;
    charge = maxCharge;
    bulletDamage = 2;
    level = 1;
    money = 0;
    for(int i=0; i<11; i++) {
        haveGear[i] = 0;
    }
}

int gamePlay(){     // 单局游戏（最核心部分）；返回值：0死亡，1通过，2退出，3重玩，4结局
    const int heroY = 400;
    double heroX = 960;
    double bulletY = 0;
    double depth = 0;
    double depthMax = 30000 + level * 1500;
    double laserDepth = 0;
    double v_move = 0;
    double v_fall = 0;
    double v_bullet = 0;
    double t_shoot = 0.09;
    double a_bullet = 0.12;
    const double g = 0.016;
    const double v_bullet_max = 10;
    const double v_fall_max = 3;
    const double v_move_max = 1.2;
    const double v_enemy_max = 0.25;
    clock_t timerLeftDown = clock()-2*CLOCKS_PER_SEC;
    clock_t timerRightDown = clock()-2*CLOCKS_PER_SEC;
    clock_t timerShoot = clock()-2*CLOCKS_PER_SEC;
    clock_t bulletHit = clock()-2*CLOCKS_PER_SEC;
    clock_t FPS_timer = clock()-2*CLOCKS_PER_SEC;
    clock_t invincibilityTimer = clock()-2*CLOCKS_PER_SEC;
    clock_t bossCDTimer = clock()-20*CLOCKS_PER_SEC;
    clock_t bossTimer = clock()-20*CLOCKS_PER_SEC;
    clock_t bossBombTimer = clock()-20*CLOCKS_PER_SEC;
    int leftUp = 0;
    int rightUp = 0;
    int E_Down = 0;
    int isOnGround = 0;
    int isStomping = 0;
    int leftBoundary = 680;
    int rightBoundary = 1240;
    int direction = 1;
    int isShooting = 0;
    int isJumping = 0;
    int bulletExist = 0;
    int hitY = -70;
    int pauseReturn = 0;
    abilityDuration = 5 * CLOCKS_PER_SEC;
    int invincibilityDuration = 1 * CLOCKS_PER_SEC;
    int bossCD = 0.5 * CLOCKS_PER_SEC;
    int bossAbility = 0;
    int bossBomb = 0;
    int bossBombX[2] = {0, 0};
    int bossBombDepth[2] = {0,0};
    int bossMissileX[3] = {8,8, 8};
    double bossMissileDepth = 0;
    int bossMissile = 0;
    bossHP = bossHPmax;
    abilityTimer = clock()-60*CLOCKS_PER_SEC;
    charge = maxCharge;
    combo = 0;
    if(level == 12){
        depthMax = 1000000;
    }

    // 义体效果
    if(haveGear[5]){
        abilityDuration = 7*CLOCKS_PER_SEC;
    }
    if(haveGear[7]){
        invincibilityDuration = 2 * CLOCKS_PER_SEC;
    }
    if(haveGear[9]){
        bulletDamage = 3;
    }else {
        bulletDamage = 2;
    }

    // 随机生成障碍方块
    int blockType[1500][8] = {0};
    srand((unsigned int)time(NULL)+clock());
    for(int i=0; i<1500-3; i+=6){
        int scene = 0;
        if(rand()%100<90){
            scene = rand()%8;
        }
        for(int k=0; k<3; k++){
            for(int j=0; j<8; j++){
                blockType[i+k][j]=0;
            }
        }
        switch(scene){
            case 1:
                for(int j=0; j<8; j+=6){
                    int row1 = rand()%3;
                    blockType[i+row1][j] = 1;
                    blockType[i+row1][j+1] = 1;
                }
                break;
            case 2:
                ;
                int row2 = rand()%3;
                int col2 = rand()%8;
                for(int k=row2-1; k<=row2+1; k++){
                    if(k<0||k>2){
                        k++;
                    }else {
                        for(int j=col2-1; j<=col2+1; j++){
                            if(j<0||j>7){
                                j++;
                            }else if(rand()%100<70){
                                blockType[i+k][j] = 2;
                            }
                        }
                    }
                }
                break;
            case 3:
                ;
                int row3 = rand()%3;
                int col3 = rand()%7;
                blockType[i+row3][col3] = 3;
                blockType[i+row3][col3+1] = 3;
                for(int k=row3; k<=row3+1; k++){
                    if(k>2){
                        k++;
                    }else {
                        for(int j=col3-3; j<=col3-1; j++){
                            if(j<0||j>7){
                                j++;
                            }else if(rand()%100<70){
                                blockType[i+k][j] = 2;
                            }
                        }
                        for(int j=col3+2; j<=col3+4; j++){
                            if(j<0||j>7){
                                j++;
                            }else if(rand()%100<70){
                                blockType[i+k][j] = 2;
                            }
                        }
                    }
                }
                break;
            case 4:
                ;
                int row4 = rand()%3;
                int col4 = rand()%7;
                blockType[i+row4][col4] = 3;
                blockType[i+row4][col4+1] = 3;
                break;
            case 5:
                ;
                int row5 = rand() % 3;
                int col5 = (rand() % 2) * 6;
                blockType[i+row5][col5] = 1;
                blockType[i+row5][col5+1] = 1;
                for(int k=row5; k<=row5+1; k++){
                    if(k>2){
                        k++;
                    }else {
                        for(int j=col5-3; j<=col5-1; j++){
                            if(j<0||j>7){
                                j++;
                            }else if(rand()%100<70){
                                blockType[i+k][j] = 2;
                            }
                        }
                        for(int j=col5+2; j<=col5+4; j++){
                            if(j<0||j>7){
                                j++;
                            }else if(rand()%100<70){
                                blockType[i+k][j] = 2;
                            }
                        }
                    }
                }
                break;
            case 6:
                ;
                int row6 = rand() % 3;
                int col6 = rand() % 6;
                for(int j=col6; j<col6+3; j++){
                    blockType[i+row6][j] = 2;
                }
                break;
            case 7:
                ;
                int row7 = rand() % 3;
                int col7 = (rand() % 2) * 6;
                blockType[i+row7][col7] = 1;
                blockType[i+row7][col7+1] = 1;
                if(col7 == 0){
                    blockType[i+row7][col7+2] = 3;
                    blockType[i+row7][col7+3] = 3;
                }else {
                    blockType[i+row7][col7-1] = 3;
                    blockType[i+row7][col7-2] = 3;
                }
                break;
            default:
                break;
        }
    }

     // 随机生成敌人
    struct Enemy{
        int HP;
        double abs_y;
        int isActive;
        int type;
        int direction;
        int isOnGround;
        double v_x;
        double v_y;
        double x;
        double y;
        SDL_Rect rect;
    };
    struct Enemy enemy[10000];
    int enemyNum=0;
    int probability_enemySpawn=(int)(10+1.0*level);
    if(level == 12){
        probability_enemySpawn = 8;
    }
    for(int Row=1; Row<1500-3-1; Row++){
        for(int Col=0; Col<8; Col++){
            if(blockType[Row][Col]==0 && (blockType[Row+1][Col]!=0 || blockType[Row-1][Col]!=0) && rand()%100<probability_enemySpawn){
                int enemyType = rand()%100;
                if(blockType[Row-1][Col]!=0){
                    if(enemyType <25){
                        enemy[enemyNum].type = 2;
                    }else {
                        enemy[enemyNum].type = 3;
                    }
                }else{
                    if(enemyType <35){
                        enemy[enemyNum].type = 0;
                    }else {
                        enemy[enemyNum].type = 1;
                    }
                }
                switch(enemy[enemyNum].type){
                    case 0: // 爬行且带刺
                        enemy[enemyNum].HP = 4 + level/4;
                        enemy[enemyNum].abs_y = 70*(Row+1) + 1080*2 - walk0L.rect.h-10;
                        enemy[enemyNum].rect.w = walk0L.rect.w;
                        enemy[enemyNum].rect.h = walk0L.rect.h;
                        break;
                    case 1: // 爬行不带刺
                        enemy[enemyNum].HP = 8 + level/4;
                        enemy[enemyNum].abs_y = 70*(Row+1) + 1080*2 - walk1L.rect.h-10;
                        enemy[enemyNum].rect.w = walk1L.rect.w;
                        enemy[enemyNum].rect.h = walk1L.rect.h;
                        break;
                    case 2: // 飞行且带刺
                        enemy[enemyNum].HP = 6 + level/4;
                        enemy[enemyNum].abs_y = 70*(Row+1) + 1080*2 - fly0L.rect.h-10;
                        enemy[enemyNum].rect.w = fly0L.rect.w;
                        enemy[enemyNum].rect.h = fly0L.rect.h;
                        break;
                    case 3: // 飞行不带刺
                        enemy[enemyNum].HP = 6 + level/4;
                        enemy[enemyNum].abs_y = 70*(Row+1) + 1080*2 - fly1L.rect.h-10;
                        enemy[enemyNum].rect.w = fly1L.rect.w;
                        enemy[enemyNum].rect.h = fly1L.rect.h;
                        break;
                    default:
                        break;
                }
                enemy[enemyNum].rect.x = 70*Col+680;
                enemy[enemyNum].isActive = 0;
                enemy[enemyNum].direction = 1;
                enemy[enemyNum].x = enemy[enemyNum].rect.x;
                enemy[enemyNum].isOnGround = 0;
                enemy[enemyNum].v_x = 0;
                enemy[enemyNum].v_y = 0;
                enemyNum++;
            }
        }
    }

    // 随机生成商店和奖励房间
    int shopSide = rand() % 2;
    int shopRow = 200;
    for(int i = 420; i > 0; i--){
        if(blockType[i][shopSide*7] == 0 && blockType[i+1][shopSide*7] == 1){
            shopRow = i;
            break;
        }
    }
    int mineSide = (shopSide + 3) % 2;
    int mineRow = 200;
    int mineType = rand()%2;
    int haveMine = 1;
    for(int i = 200; i < 500; i++){
        if(blockType[i][mineSide*7] == 0 && blockType[i+1][mineSide*7] == 1){
            mineRow = i;
            break;
        }
    }

    // 循环--------------------------------------------------------
    while(1){
        // 捕捉操作
        SDL_Event actionEvent;
        SDL_PollEvent(&actionEvent);
        isJumping = 0;
        switch (actionEvent.type){
            case SDL_QUIT:
                QUIT();
                break;
            case SDL_KEYDOWN:
                switch (actionEvent.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        pauseReturn = pause();
                        break;
                    case SDLK_SPACE:
                    case SDLK_w:
                    case SDLK_RSHIFT:
                        if(isOnGround == 1){
                            isOnGround = 0;
                            isJumping = 1;
                        }else{
                            isShooting = 1;
                        }
                        break;
                    case SDLK_a:
                    case SDLK_LEFT:
                        timerLeftDown = clock();
                        leftUp = 0;
                        direction = 0;
                        break;
                    case SDLK_d:
                    case SDLK_RIGHT:
                        timerRightDown = clock();
                        rightUp = 0;
                        direction = 1;
                        break;
                    case SDLK_e:
                        E_Down = 1;
                        break;
                    default:
                        break;
                }
                break;
            case SDL_KEYUP:
                switch (actionEvent.key.keysym.sym) {
                    case SDLK_SPACE:
                    case SDLK_w:
                    case SDLK_RSHIFT:
                        isShooting = 0;
                        break;
                    case SDLK_a:
                    case SDLK_LEFT:
                        leftUp=1;
                        break;
                    case SDLK_d:
                    case SDLK_RIGHT:
                        rightUp=1;
                        break;
                    case SDLK_e:
                        E_Down = 0;
                        break;
                    default:
                        break;
                }
                break;
            default:
                break;
        }

        // 暂停选项判定 0继续游戏，1重新开始，2退出游戏
        switch( pauseReturn ){
            case 1: // 重新开始
                return 3;
                break;
            case 2: // 退出游戏
                return 2;
                break;
            default: // 继续
                break;
        }

        // 控制FPS
        int FPS_duration = (int)((1.0/250.0)*CLK_TCK - (clock()-FPS_timer));
        if(FPS_duration >0 && rand()%100 < 20){
            SDL_Delay(1);
        }
        FPS_timer = clock();

        // 生成方块碰撞体积
        struct Blocks{
            SDL_Rect rect;
            int label;
        }blocks[24][8];
        if (depth >= 1080){
            int startIndex = 0;
            if(depth >= 1080*2){
                startIndex = (int)(depth-1080*2)/70;
            }
            int row=0;
            for (int i= startIndex; i<=(int)((depth-1080)/70)+8; i++){
                for(int j=0; j<8; j++){
                    switch(blockType[i][j]){
                        case 1:
                            createRect(&blocks[row][j].rect, 680 + j*70, 70*i-(int)(depth-1080*2), block1.rect.w, block1.rect.h);
                            j++;
                            break;
                        case 2:
                            createRect(&blocks[row][j].rect, 680 + j*70, 70*i-(int)(depth-1080*2), block2.rect.w, block2.rect.h);
                            break;
                        case 3:
                            createRect(&blocks[row][j].rect, 680 + j*70, 70*i-(int)(depth-1080*2), block3.rect.w, block3.rect.h);
                            break;
                        default:
                            createRect(&blocks[row][j].rect, 0, 0, 0, 0);
                            break;
                    }
                    blocks[row][j].label = i;
                }
                row++;
            }
        }else {
            for(int i=0; i<24; i++){
                for(int j=0; j<8; j++){
                    createRect(&blocks[i][j].rect, 0, 0, 0, 0);
                    blocks[i][j].label = i;

                }
            }
        }

        // 商店
        SDL_Rect heroCollision;
        createRect(&heroCollision, (int)heroX, heroY, normalL.rect.w, normalL.rect.h);
        SDL_Rect shopPortal;
        createRect(&shopPortal, 680 + shopSide * (1205-680), 70*shopRow-(int)(depth-1080*2), portal.rect.w, portal.rect.h);
        SDL_Rect shopResult;
        if(SDL_IntersectRect(&shopPortal, &heroCollision, &shopResult) == SDL_TRUE){
            if(direction == 0){
                direction = 1;
            }else if(direction == 1){
                direction = 0;
            }
            shopUI(shopSide);
            if(shopSide == 0){
                leftUp = 1;
                heroX += 35;
                timerRightDown = (clock_t)(clock() - 1.75*CLOCKS_PER_SEC);
            }else {
                heroX -= 35;
                rightUp = 1;
                timerLeftDown = (clock_t)(clock() - 1.75*CLOCKS_PER_SEC);
            }
        }
        // 奖励房间
        SDL_Rect minePortal;
        createRect(&minePortal, 680 + mineSide * (1205-680), 70*mineRow-(int)(depth-1080*2), portal.rect.w, portal.rect.h);
        SDL_Rect mineResult;
        if(SDL_IntersectRect(&minePortal, &heroCollision, &mineResult) == SDL_TRUE){
            if(direction == 0){
                direction = 1;
            }else if(direction == 1){
                direction = 0;
            }
            mineUI(&haveMine, mineSide, mineType);
            if(mineSide == 0){
                leftUp = 1;
                heroX += 35;
                timerRightDown = (clock_t)(clock() - 1.75*CLOCKS_PER_SEC);
            }else {
                heroX -= 35;
                rightUp = 1;
                timerLeftDown = (clock_t)(clock() - 1.75*CLOCKS_PER_SEC);
            }
        }

        // 主动技能
        if(E_Down == 1 && clock() - abilityTimer > abilityCD && haveGear[0] == 1){
            abilityTimer = clock();
        }

        // 敌人运动
        for(int i = 0; i <enemyNum; i++){
            enemy[i].y = enemy[i].abs_y - depth;
            enemy[i].rect.y = (int)enemy[i].y+1;
            if(enemy[i].rect.y >= -100 && enemy[i].rect.y <= 1180 && enemy[i].HP > 0){
                enemy[i].isActive = 1;
            }else{
                enemy[i].isActive = 0;
            }
            if(enemy[i].isActive == 1 && clock() - abilityTimer > abilityDuration){
                // 水平速度
                if(enemy[i].type >= 2 || enemy[i].y <= heroY + 2*heroCollision.h) {
                    double a_enemy_x = 0.1 * g;
                    if (heroX > enemy[i].x) {
                        if(enemy[i].v_x + a_enemy_x < v_enemy_max){
                            enemy[i].v_x += a_enemy_x;
                        }else {
                            enemy[i].v_x = v_enemy_max;
                        }
                    } else if (heroX < enemy[i].x) {
                        if(enemy[i].v_x - a_enemy_x > -v_enemy_max){
                            enemy[i].v_x -= a_enemy_x;
                        }else {
                            enemy[i].v_x = -v_enemy_max;
                        }
                    }
                    if(enemy[i].v_x >= 0){
                        enemy[i].direction = 1;
                    }else {
                        enemy[i].direction = 0;
                    }
                }
                // 竖直速度
                if(enemy[i].type >= 2) {
                    double a_enemy_y = 0.06 * g;
                    if (heroY >= enemy[i].y) {
                        if(enemy[i].v_y + a_enemy_y <= v_fall_max * 0.1){
                            enemy[i].v_y += a_enemy_y;
                        }else {
                            enemy[i].v_y = v_fall_max * 0.1;
                        }
                    } else {
                        if(enemy[i].v_y - a_enemy_y >= -v_fall_max * 0.1){
                            enemy[i].v_y -= a_enemy_y;
                        }else {
                            enemy[i].v_y = -v_fall_max * 0.1;
                        }
                    }
                    if(enemy[i].isOnGround !=0){
                        enemy[i].v_y = 0;
                    }
                }else {
                    if(enemy[i].isOnGround == 0){
                        if(enemy[i].v_y < v_fall_max && enemy[i].v_y+g <= v_fall_max) {
                            enemy[i].v_y += g;
                        }else if(enemy[i].v_y+g >= v_fall_max){
                            enemy[i].v_y = v_fall_max;
                        }
                    }else if(enemy[i].v_y >0){
                        enemy[i].v_y = 0;
                    }
                }
                // 与方块碰撞
                int enemyGroundCounter = 0;
                leftBoundary = 680;
                rightBoundary = 1240;
                for(int k = 0; k < 24; k++){
                    for(int j = 0; j < 8; j++){
                        SDL_Rect result;
                        if(SDL_IntersectRect(&enemy[i].rect, &blocks[k][j].rect, &result) == SDL_TRUE){
                            if(enemy[i].y <= blocks[k][j].rect.y + blocks[k][j].rect.h  && enemy[i].v_y <0 && blockType[blocks[k][j].label][j]!=3
                               && enemy[i].y >= blocks[k][j].rect.y + blocks[k][j].rect.h - 3 ){ // 敌人头部碰到方块底部
                                enemy[i].v_y = 0;
                                enemy[i].abs_y += blocks[k][j].rect.y + blocks[k][j].rect.h - enemy[i].y +1;
                            }
                            if(enemy[i].y + enemy[i].rect.h >= blocks[k][j].rect.y && enemy[i].y + enemy[i].rect.h <= blocks[k][j].rect.y + 3
                            && enemy[i].x + enemy[i].rect.w > blocks[k][j].rect.x +3 && enemy[i].x < blocks[k][j].rect.x +blocks[k][j].rect.w -3){ // 敌人底部碰到方块顶部
                                if(!(enemy[i].type >= 2 && blockType[blocks[k][j].label][j] == 3)) {
                                    enemyGroundCounter++;
                                    if(enemy[i].v_y >0){
                                        enemy[i].v_y = 0;
                                    }
                                }
                            }
                            if(enemy[i].x + enemy[i].rect.w >= blocks[k][j].rect.x && enemy[i].x + enemy[i].rect.w <= blocks[k][j].rect.x + 3
                               && enemy[i].y < blocks[k][j].rect.y + blocks[k][j].rect.h - 3 && enemy[i].y + enemy[i].rect.h > blocks[k][j].rect.y + 3
                               && blockType[blocks[k][j].label][j]!=3){ // 敌人右边碰到方块左边
                                rightBoundary = blocks[k][j].rect.x;
                            }
                            if(enemy[i].x <= blocks[k][j].rect.x + blocks[k][j].rect.w && enemy[i].x >= blocks[k][j].rect.x + blocks[k][j].rect.w - 3
                               && enemy[i].y < blocks[k][j].rect.y + blocks[k][j].rect.h - 3 && enemy[i].y + enemy[i].rect.h > blocks[k][j].rect.y + 3
                               && blockType[blocks[k][j].label][j]!=3){ // 敌人左边碰到方块右边
                                leftBoundary = blocks[k][j].rect.x + blocks[k][j].rect.w;
                            }
                        }
                    }
                }
                if(enemyGroundCounter != 0){
                    enemy[i].isOnGround = 1;
                }else {
                    enemy[i].isOnGround = 0;
                }
                // 位移
                enemy[i].y += enemy[i].v_y;
                enemy[i].abs_y += enemy[i].v_y;
                enemy[i].rect.y = (int)enemy[i].y;
                if(enemy[i].x + enemy[i].v_x > (double)leftBoundary && enemy[i].x+enemy[i].rect.w + enemy[i].v_x <(double)rightBoundary && enemy[i].v_x != 0.0){
                    enemy[i].x += enemy[i].v_x;
                }else if(enemy[i].x + enemy[i].v_x<=(double)leftBoundary){
                    enemy[i].x = (double)leftBoundary +1 ;
                }else if(enemy[i].x + enemy[i].rect.w + enemy[i].v_x >= (double)rightBoundary){
                    enemy[i].x = (double)rightBoundary - enemy[i].rect.w ;
                }
                enemy[i].rect.x = (int)enemy[i].x;
            }
        }

        // Boss
        SDL_Rect bossRect;
        createRect(&bossRect, 0, 0, 0 ,0);
        if(level == 12){
            createRect(&bossRect, 680, 778, 560, 302);
            if(clock() - abilityTimer > abilityDuration) {
                laserDepth += 0.2;
            }
            if(laserDepth + bossImage[4].rect.h > depth + heroY && clock() - invincibilityTimer > invincibilityDuration){   // 上层激光
                HP -= 2;
                invincibilityTimer = clock();
            }
            if(clock() - bossCDTimer >= bossCD && clock() - abilityTimer > abilityDuration){
                bossAbility = rand()%3 + 1;
                bossTimer = clock();
                bossCDTimer = clock();
            }
            switch(bossAbility){
                case 1:     // 两侧激光
                    if(clock() - bossTimer > 1*CLOCKS_PER_SEC && clock() - bossTimer < 3*CLOCKS_PER_SEC
                    && clock() - invincibilityTimer > invincibilityDuration && (heroX < 680+70.0 || heroX + normalL.rect.w > 1240 - 70)){
                        invincibilityTimer = clock();
                        HP-=3;
                    }
                    if(clock() - bossTimer >= 3*CLOCKS_PER_SEC){
                        bossAbility = 0;
                    }
                    bossCDTimer = clock();
                    break;
                case 2:     // 导弹
                    if(clock() - bossTimer >= 3*CLOCKS_PER_SEC && bossMissileDepth <= depth - 100){
                        bossMissile = 0;
                        bossAbility = 0;
                        for(int i=0; i<3; i++){ bossMissileX[i] = 8;}
                    }else if(bossMissile == 0){
                        for(int i=0; i<3; i++) {
                            do {
                                bossMissileX[i] = rand()%8;
                            } while((bossMissileX[i] == bossMissileX[0] && i!=0)||(bossMissileX[i] == bossMissileX[1] && i!=1));
                        }
                        bossMissileDepth = depth + 1080;
                        bossMissile = 1;
                    }else if(bossMissile == 1){
                        if(clock() - abilityTimer > abilityDuration) {
                            bossMissileDepth -= 3 * v_enemy_max;
                        }
                        for(int i=0; i<3; i++){
                            SDL_Rect missileRect;
                            createRect(&missileRect, 680 + bossMissileX[i]*70, (int)(bossMissileDepth - depth), 70, 70);
                            SDL_Rect result;
                            if(SDL_IntersectRect(&missileRect, &heroCollision, &result) == SDL_TRUE && clock() - invincibilityTimer > invincibilityDuration){
                                invincibilityTimer = clock();
                                HP-=2;
                            }
                        }
                    }
                    bossCDTimer = clock();
                    break;
                case 3:     // 定点爆破
                    if(clock() - abilityTimer > abilityDuration) {
                        if (clock() - bossTimer < 1 * CLOCKS_PER_SEC && bossBomb == 0) { // 第1次瞄准
                            bossBomb = 1;
                            bossBombX[0] = (int) heroX - 12;
                            bossBombDepth[0] = (int) depth;
                        }
                        if (clock() - bossTimer >= 1 * CLOCKS_PER_SEC && bossBomb == 1) { // 第1次爆破
                            bossBomb = 2;
                            Mix_PlayChannel(-1, chunks[3], 0);
                            SDL_Rect bomber;
                            bossBombX[1] = bossBombX[0];
                            bossBombDepth[1] = bossBombDepth[0];
                            createRect(&bomber, bossBombX[0], bossBombDepth[0] + 400 - 12 - (int) depth,
                                       bossImage[3].rect.w, bossImage[3].rect.h);
                            SDL_Rect result;
                            if (SDL_IntersectRect(&bomber, &heroCollision, &result) == SDL_TRUE &&
                                clock() - invincibilityTimer > invincibilityDuration) {
                                invincibilityTimer = clock();
                                HP -= 2;
                            }
                            bossBombTimer = clock();
                        }
                        if (clock() - bossTimer < 2 * CLOCKS_PER_SEC && bossBomb == 2) { // 第2次瞄准
                            bossBomb = 3;
                            bossBombX[0] = (int) heroX - 12;
                            bossBombDepth[0] = (int) depth;
                        }
                        if (clock() - bossTimer >= 2 * CLOCKS_PER_SEC && bossBomb == 3) { // 第2次爆破
                            bossBomb = 4;
                            Mix_PlayChannel(-1, chunks[3], 0);
                            SDL_Rect bomber;
                            bossBombX[1] = bossBombX[0];
                            bossBombDepth[1] = bossBombDepth[0];
                            createRect(&bomber, bossBombX[0], bossBombDepth[0] + 400 - 12 - (int) depth,
                                       bossImage[3].rect.w, bossImage[3].rect.h);
                            SDL_Rect result;
                            if (SDL_IntersectRect(&bomber, &heroCollision, &result) == SDL_TRUE &&
                                clock() - invincibilityTimer > invincibilityDuration) {
                                invincibilityTimer = clock();
                                HP -= 2;
                            }
                            bossBombTimer = clock();
                        }
                        if (clock() - bossTimer < 3 * CLOCKS_PER_SEC && bossBomb == 4) { // 第3次瞄准
                            bossBomb = 5;
                            bossBombX[0] = (int) heroX - 12;
                            bossBombDepth[0] = (int) depth;
                        }
                        if (clock() - bossTimer >= 3 * CLOCKS_PER_SEC && bossBomb == 5) { // 第3次爆破
                            bossBomb = 0;
                            Mix_PlayChannel(-1, chunks[3], 0);
                            SDL_Rect bomber;
                            bossBombX[1] = bossBombX[0];
                            bossBombDepth[1] = bossBombDepth[0];
                            createRect(&bomber, bossBombX[0], bossBombDepth[0] + 400 - 12 - (int) depth,
                                       bossImage[3].rect.w, bossImage[3].rect.h);
                            SDL_Rect result;
                            if (SDL_IntersectRect(&bomber, &heroCollision, &result) == SDL_TRUE &&
                                clock() - invincibilityTimer > invincibilityDuration) {
                                invincibilityTimer = clock();
                                HP -= 2;
                            }
                            bossBombTimer = clock();
                            bossAbility = 0;
                        }
                    }else {
                        bossTimer = clock();
                    }
                    bossCDTimer = clock();
                    break;
                default:
                    break;
            }
        }

        // 角色与方块碰撞判定
        leftBoundary = 680;
        rightBoundary = 1240;
        int groundCounter = 0;
        for(int i=0; i<24; i++){
            for(int j=0; j<8; j++){
                SDL_Rect result;
                if(SDL_IntersectRect(&heroCollision, &blocks[i][j].rect, &result) == SDL_TRUE){
                    if(heroY <= blocks[i][j].rect.y + blocks[i][j].rect.h  && v_fall <0 && blockType[blocks[i][j].label][j]!=3
                       && heroY >= blocks[i][j].rect.y + blocks[i][j].rect.h - 3 ){ // 角色头部碰到方块底部
                        v_fall = 0;
                        depth += blocks[i][j].rect.y + blocks[i][j].rect.h - heroY +1;
                        if(blockType[blocks[i][j].label][j] == 2){
                            blockType[blocks[i][j].label][j] = 0;
                        }
                    }
                    if(heroY + heroCollision.h >= blocks[i][j].rect.y && heroY + heroCollision.h <= blocks[i][j].rect.y + 3
                    && heroX + heroCollision.w > blocks[i][j].rect.x +3 && heroX < blocks[i][j].rect.x +blocks[i][j].rect.w -3){ // 角色底部碰到方块顶部
                        groundCounter++;
                        isShooting = 0;
                    }
                    if(heroX + heroCollision.w >= blocks[i][j].rect.x && heroX + heroCollision.w <= blocks[i][j].rect.x + 3
                       && heroY < blocks[i][j].rect.y + blocks[i][j].rect.h - 3 && heroY + heroCollision.h > blocks[i][j].rect.y + 3
                       && blockType[blocks[i][j].label][j]!=3){ // 角色右边碰到方块左边
                        rightBoundary = blocks[i][j].rect.x + 3;
                    }
                    if(heroX <= blocks[i][j].rect.x + blocks[i][j].rect.w && heroX >= blocks[i][j].rect.x + blocks[i][j].rect.w - 3
                       && heroY < blocks[i][j].rect.y + blocks[i][j].rect.h - 3 && heroY + heroCollision.h > blocks[i][j].rect.y + 3
                       && blockType[blocks[i][j].label][j]!=3){ // 角色左边碰到方块右边
                        leftBoundary = blocks[i][j].rect.x + blocks[i][j].rect.w - 3 ;
                    }
                }
            }
        }
        if(groundCounter!=0){
            isOnGround = 1;
        }else{
            isOnGround = 0;
        }

        // 角色与敌人碰撞判定
        for(int i=0; i<enemyNum; i++){
            SDL_Rect result;
            if(enemy[i].isActive != 0 && enemy[i].HP > 0 && SDL_IntersectRect(&heroCollision, &enemy[i].rect, &result) == SDL_TRUE){
                if(heroY + heroCollision.h <= enemy[i].rect.y + 10 && (enemy[i].type == 1 || enemy[i].type == 3)){
                    isStomping = 1;         // 踩死
                    enemy[i].HP = 0;
                    combo++;
                    if(haveGear[1] == 1 && rand()%100 < 5 && HP <= maxHP - 1){
                        HP += 1;
                    }
                    money += rand()%8 + 1;
                    if(haveGear[3]){
                        money += 1;
                    }
                    if(haveGear[10] || charge >= maxCharge - 4){
                        charge = maxCharge;
                    }else {
                        charge += 4;
                    }
                }else if(clock() - invincibilityTimer > invincibilityDuration){
                    if(haveGear[6] && rand()%100 < 50){
                        HP -= 1;
                    }else{
                        HP -= 2;
                    }
                    if(haveGear[4]){
                        enemy[i].HP = 0;
                    }
                    invincibilityTimer = clock();
                }
            }
        }

        // 角色水平移动
        if(((double)(clock()-timerLeftDown)/CLOCKS_PER_SEC <= 2.0) && leftUp == 0){
            v_move = -v_move_max;
        }else if(((double)(clock()-timerRightDown)/CLOCKS_PER_SEC <= 2.0) && rightUp == 0){
            v_move = v_move_max;
        }else {
            v_move = 0;
        }
        if(heroX+v_move > (double)leftBoundary && heroX+normalL.rect.w+v_move <(double)rightBoundary && v_move != 0.0){
            heroX = heroX + v_move;
        }else if(heroX+v_move <= (double)leftBoundary){
            heroX = (double)leftBoundary + 1;
        }else if(heroX+normalL.rect.w+v_move >= (double)rightBoundary){
            heroX = (double)rightBoundary - normalL.rect.w - 1;
        }

        // 子弹运动
        if(isOnGround){
            charge = maxCharge;
        }
        if((double)(clock()-timerShoot)/CLOCKS_PER_SEC > t_shoot && isShooting && charge>0 && bulletExist==0) {
            Mix_PlayChannel(-1, chunks[0], 0);
            timerShoot = clock();
            bulletExist = 1;
            bullet.rect.x = (int) (heroX + normalL.rect.w / 2.0 - bullet.rect.w / 2.0);
            bullet.rect.y = heroY + normalL.rect.h;
            bulletY = (double) bullet.rect.y;
            v_bullet = v_bullet_max;
            charge--;
        }
        if(v_bullet<=0 ){
            bulletExist =0;
        }else if(bulletExist){
            v_bullet -= a_bullet;
            bulletY += v_bullet;
            bullet.rect.y = (int)bulletY;
        }

        // 子弹与方块、敌人碰撞
        if(bulletExist){
            for(int i=0; i<24; i++){
                for(int j=0; j<8; j++){
                    SDL_Rect result;
                    if(SDL_IntersectRect(&bullet.rect, &blocks[i][j].rect, &result) == SDL_TRUE
                    && bullet.rect.y + bullet.rect.h >= blocks[i][j].rect.y
                    && bullet.rect.y + bullet.rect.h <= blocks[i][j].rect.y + 70
                    && blockType[blocks[i][j].label][j] != 3){
                        bulletExist = 0;
                        if(blockType[blocks[i][j].label][j]==2){
                            blockType[blocks[i][j].label][j]=0;
                        }
                        hitY = blocks[i][j].rect.y;
                        bulletHit = clock();
                    }
                }
            }
            for(int i = 0; i<enemyNum; i++){
                SDL_Rect result;
                if(SDL_IntersectRect(&bullet.rect, &enemy[i].rect, &result) == SDL_TRUE
                && enemy[i].HP > 0 && enemy[i].isActive != 0){
                    bulletExist = 0;
                    enemy[i].HP -= bulletDamage;
                    if(enemy[i].HP <= 0){ // 射击杀死敌人
                        combo ++;
                        if(haveGear[1] == 1 && rand()%100 < 5 && HP <= maxHP - 1){
                            HP += 1;
                        }
                        money += rand()%8 + 1;
                        if(haveGear[3]){
                            money += 1;
                        }
                        if(haveGear[8]){
                            if(charge <= maxCharge - 2){
                                charge += 2;
                            }else {
                                charge = maxCharge;
                            }
                        }
                    }
                    hitY = enemy[i].rect.y;
                    bulletHit = clock();
                }
            }
            if(level == 12 && bulletY >= 778){
                bulletHit = clock();
                bulletExist = 0;
                bossHP -= bulletDamage;
                hitY = 950;
            }
        }

        // 角色竖直运动
        if(charge==0){
            isShooting = 0;
        }
        if(isOnGround == 0 && isShooting == 0 && isStomping == 0){ // 自然下落
            if(v_fall < v_fall_max && v_fall+g <= v_fall_max) {
                v_fall += g;
            }else if(v_fall+g >= v_fall_max){
                v_fall = v_fall_max;
            }
        }else if(isStomping){ // 踩怪
            Mix_PlayChannel(-1, chunks[1], 0);
            isStomping = 0;
            v_fall = -0.5*v_fall_max;
        }else if(isJumping){ // 跳跃
            Mix_PlayChannel(-1, chunks[4], 0);
            v_fall = -0.85*v_fall_max;
        }else if(isShooting){ // 射击
            v_fall = 0.25;
        }else if(isOnGround == 1 && v_fall>0){ // 在地上
            v_fall = 0;
        }
        depth += v_fall;
        int towerY= (int)depth%3240;

        // 连杀判定
        if(isOnGround){
            if(combo >=8) {
                if (combo >= 20 && HP < maxHP) {
                    HP++;
                }
                if (combo >= 50) {
                    maxCharge++;
                    charge = maxCharge;
                }
                money += 100;
                if (haveGear[3] == 1) {
                    money += 20;
                }
            }
            combo = 0;
        }

        // 渲染背景
        presentImage(&gameBG,0,0);
        presentImage(&tower, 645, -towerY);
        presentImage(&tower, 645, 3240 - towerY);

        // 渲染子弹击中特效
        if(clock()-bulletHit <= 0.05*CLOCKS_PER_SEC && isShooting == 1){
            presentImage(&bulletExplode,bullet.rect.x+bullet.rect.w/2-bulletExplode.rect.w/2,hitY-bulletExplode.rect.h);
        }

        // 渲染主角
        double heroAlpha = 255;
        if(clock() - invincibilityTimer <= invincibilityDuration){
            heroAlpha = 255.0/2.0;
        }
        if(direction == 0){
            if(v_fall < 0){
                presentAlphaImage(&jumpingL,heroAlpha,(int)heroX,heroY);
            }else if(isShooting){
                presentAlphaImage(&firingL,heroAlpha,(int)heroX,heroY);
            }else {
                presentAlphaImage(&normalL,heroAlpha,(int)heroX,heroY);
            }
        }else if(direction == 1){
            if(v_fall < 0){
                presentAlphaImage(&jumpingR,heroAlpha,(int)heroX,heroY);
            }else if(isShooting){
                presentAlphaImage(&firingR,heroAlpha,(int)heroX,heroY);
            }else {
                presentAlphaImage(&normalR,heroAlpha,(int)heroX,heroY);
            }
        }

        // 渲染敌人
        for(int i = 0; i <enemyNum; i++){
            if(enemy[i].isActive != 0 && enemy[i].HP >0) {
                if (enemy[i].direction == 1) {
                    switch (enemy[i].type) {
                        case 0:
                            presentImage(&walk0R, enemy[i].rect.x, enemy[i].rect.y);
                            break;
                        case 1:
                            presentImage(&walk1R, enemy[i].rect.x, enemy[i].rect.y);
                            break;
                        case 2:
                            presentImage(&fly0R, enemy[i].rect.x, enemy[i].rect.y);
                            break;
                        case 3:
                            presentImage(&fly1R, enemy[i].rect.x, enemy[i].rect.y);
                            break;
                        default:
                            break;
                    }
                } else {
                    switch (enemy[i].type) {
                        case 0:
                            presentImage(&walk0L, enemy[i].rect.x, enemy[i].rect.y);
                            break;
                        case 1:
                            presentImage(&walk1L, enemy[i].rect.x, enemy[i].rect.y);
                            break;
                        case 2:
                            presentImage(&fly0L, enemy[i].rect.x, enemy[i].rect.y);
                            break;
                        case 3:
                            presentImage(&fly1L, enemy[i].rect.x, enemy[i].rect.y);
                            break;
                        default:
                            break;
                    }
                }
            }
        }

        // 渲染障碍物
        for(int i=0; i<24; i++){
            for(int j=0; j<8; j++){
                switch(blocks[i][j].rect.h){
                    case 60:
                        presentImage(&block1,blocks[i][j].rect.x,blocks[i][j].rect.y);
                        j++;
                        break;
                    case 70:
                        presentImage(&block2,blocks[i][j].rect.x,blocks[i][j].rect.y);
                        break;
                    case 5:
                        presentImage(&block3,blocks[i][j].rect.x,blocks[i][j].rect.y);
                        break;
                    default:
                        break;
                }
            }
        }
        if(depth>= depthMax){
            presentImage(&gate, 680, 1080-(int)(depth-depthMax));
        }
        presentImage(&portal, shopPortal.x, shopPortal.y);
        presentImage(&portal, minePortal.x, minePortal.y);

        // 渲染子弹
        if(bulletExist){
            presentImage(&bullet,bullet.rect.x,bullet.rect.y);
        }

        // 渲染boss
        if(level == 12){
            presentImage(&bossImage[5], 680, 778); // boss本体
            if(laserDepth + 100 >= depth){  // 上层激光
                presentImage(&bossImage[4], 593,(int)(laserDepth - depth));
            }
            switch(bossAbility){
                case 1: // 两侧激光
                    if(clock() - bossTimer > 1.5*CLOCKS_PER_SEC && clock() - bossTimer < 6.5*CLOCKS_PER_SEC){
                        presentImage(&bossImage[0], 636, 0);
                    }else if(clock() - bossTimer <= 1.5*CLOCKS_PER_SEC){
                        presentAlphaImage(&bossImage[0], 51,636, 0);
                    }
                    break;
                case 2: // 导弹
                    for(int i = 0; i <3; i++){
                        presentImage(&bossImage[1], bossMissileX[i]*70 + 680, (int)(bossMissileDepth - depth));
                    }
                    break;
                case 3: // 爆破
                    presentImage(&bossImage[2], bossBombX[0], (int)(bossBombDepth[0] - depth + 400 - 12));
                    break;
                default:
                    break;
            }
            if(clock() - bossBombTimer < 0.25*CLOCKS_PER_SEC){
                presentImage(&bossImage[3], bossBombX[1], (int)(bossBombDepth[1] - depth + 400 - 12));
            }
        }

        // 渲染信息
        infoPresent(0);
        if(clock() - abilityTimer <= abilityDuration){
            presentImage(&abilityUI , 0, 0);
        }
        SDL_RenderPresent(renderer);

        // 通关/死亡 判定
        if(1080-(int)(depth-depthMax) <= heroY){
            if(combo >=8) {
                if (combo >= 20 && HP < maxHP) {
                    HP++;
                }
                if (combo >= 50) {
                    maxCharge++;
                    charge = maxCharge;
                }
                money += 100;
                if (haveGear[3] == 1) {
                    money += 20;
                }
            }
            combo = 0;
            return 1; // 通关
        }
        if(HP<=0){
            SDL_Delay(2 * CLOCKS_PER_SEC);
            return 0; // 死亡
        }
        if(bossHP <= 0){
            return 4; // 结局
        }

    }

    return 0;
}

void createRect(SDL_Rect *rect,int x,int y,int w,int h){    // 创建矩形
    rect->x = x;
    rect->y = y;
    rect->w = w;
    rect->h = h;
}

void mainBody(){    // 关联不同关卡
    while(level<=12 && level > 0){
        int returnValue;
        returnValue = gamePlay();
        switch (returnValue) {
            case 0: // 死亡
                Mix_HaltMusic();
                init();
                if(death() == 0){
                    Mix_HaltMusic();
                    Mix_PlayMusic(BGM, -1);
                    init();
                }else {
                    QUIT();
                }
                break;
            case 1:  // 通过
                if(level == 12){
                    Mix_HaltMusic();
                    init();
                    if(death() == 0){
                        Mix_HaltMusic();
                        Mix_PlayMusic(BGM, -1);
                        init();
                    }else {
                        QUIT();
                    }
                }else {
                    gearSelectUI();
                    level++;
                }
                break;
            case 2:  // 退出
                QUIT();
                break;
            case 3: // 重玩
                Mix_HaltMusic();
                Mix_PlayMusic(BGM, -1);
                init();
                break;
            case 4: // 结局
                if(endUI() == 0){
                    Mix_HaltMusic();
                    Mix_PlayMusic(BGM, -1);
                    init();
                }else {
                    QUIT();
                }
                break;
            default:
                break;
        }
    }
}

int death(){ // 死亡界面；返回值： 0重新开始，1退出游戏
    int baseX = 211;
    int baseY = 547;
    int offsetY = 117;
    int option = 0;
    while(1) {
        SDL_RenderClear(renderer);
        SDL_Event deathEvent;
        SDL_PollEvent(&deathEvent);
        switch (deathEvent.type) {
            case SDL_QUIT:
                QUIT();
                break;
            case SDL_KEYDOWN:
                switch (deathEvent.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        return 1;
                        break;
                    case SDLK_SPACE:
                    case SDLK_RSHIFT:
                    case SDLK_w:
                        return option;
                        break;
                    case SDLK_a:
                        option = abs(option - 1 + 2) % 2;
                        break;
                    case SDLK_d:
                        option = (option + 1) % 2;
                        break;
                    default:
                        break;
                }
                break;
            default:
                break;
        }
        presentImage(&deathUI, 0 , 0);
        presentImage(&menuBox, baseX, baseY + option*offsetY);
        SDL_RenderPresent(renderer);
    }
}

int pause(){     // 暂停界面；返回值： 0继续游戏，1重新开始，2退出游戏
    int baseX = 211;
    int baseY = 485;
    int offsetY = 140;
    int option = 0;
    Mix_PauseMusic();
    while(1) {
        SDL_RenderClear(renderer);
        SDL_Event deathEvent;
        SDL_PollEvent(&deathEvent);
        switch (deathEvent.type) {
            case SDL_QUIT:
                QUIT();
                break;
            case SDL_KEYDOWN:
                switch (deathEvent.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        Mix_ResumeMusic();
                        return 0;
                        break;
                    case SDLK_SPACE:
                    case SDLK_RSHIFT:
                    case SDLK_w:
                        if(option == 0) {
                            Mix_ResumeMusic();
                        }
                        return option;
                        break;
                    case SDLK_a:
                        option = (option - 1 + 3) % 3;
                        break;
                    case SDLK_d:
                        option = (option + 1) % 3;
                        break;
                    default:
                        break;
                }
                break;
            default:
                break;
        }
        presentImage(&pauseUI, 0 , 0);
        presentImage(&menuBox, baseX, baseY + option*offsetY);
        SDL_RenderPresent(renderer);
    }
}

void infoPresent(int isInRoom){     // 显示各项信息
    if(HP < 0){
        HP = 0;
    }
    // 血条和蓝条
    struct imagePosition{
        int baseX;
        int baseY;
        int offsetX;
        int offsetY;
    };
    struct imagePosition blueBar = {55,1000, 50, -30};
    struct imagePosition redBar = {55, 35, 50, 30};
    for(int i = 0; i < HP; i++){
        presentImage(&hpBar, redBar.baseX + (i%8)*redBar.offsetX, redBar.baseY + (i/8)*redBar.offsetY);
    }
    for(int i = 0; i < charge; i++){
        presentImage(&energyBar, blueBar.baseX + (i%8)*blueBar.offsetX, blueBar.baseY + (i/8)*blueBar.offsetY);
    }

    // 字体
    loadFont(&microsoftBlack, "res/font/microsoftBlack.ttf", 35, 255, 74, 74, 0);
    char HP_font[10];
    sprintf(HP_font, "%02d/%02d",HP,maxHP);
    presentFont(&microsoftBlack, HP_font, 470, 28);     // 血量
    loadFont(&microsoftBlack, "res/font/microsoftBlack.ttf", 35, 59, 211, 255, 0);
    char charge_font[10];
    sprintf(charge_font, "%02d/%02d",charge,maxCharge);
    presentFont(&microsoftBlack, charge_font, 470, 985);    // 蓝量
    loadFont(&microsoftBlack, "res/font/microsoftBlack.ttf", 35, 59, 211, 255, 0);
    char levelFont[4];
    sprintf(levelFont,"%d",level);
    presentFont(&microsoftBlack,levelFont,1450,15);     // 关卡数
    loadFont(&microsoftBlack, "res/font/microsoftBlack.ttf", 35, 252, 190, 0, 0);
    char moneyFont[8];
    sprintf(moneyFont,"%d",money);
    presentFont(&microsoftBlack,moneyFont,1700,15);     // 金钱

    if(isInRoom == 0) {
        // 连击
        if(combo >= 8){
            loadFont(&microsoftBlack, "res/font/microsoftBlack.ttf",35, 252, 190, 0, 0);
            char comboFont[10];
            sprintf(comboFont, "%d连击！",combo);
            presentFont(&microsoftBlack,comboFont,1700,63);
        }
        // 义体
        struct imagePosition gearPos = {1354, 231, 209, 215};
        for (int i = 0; i < 11; i++) {
            int row = i / 3;
            int col = i % 3;
            if (haveGear[i] == 1) {
                if(i == 0 && clock() - abilityTimer < abilityCD){
                    presentAlphaImage(&gear[i], 150 ,gearPos.baseX + col * gearPos.offsetX, gearPos.baseY + row * gearPos.offsetY);
                }else {
                    presentImage(&gear[i], gearPos.baseX + col * gearPos.offsetX,gearPos.baseY + row * gearPos.offsetY);
                }
            }
        }
        // boss 血量
        if(level == 12) {
            struct imagePosition bossHP_pos = {1300, 1000, 50, 0};
            int  endIndex = 8;
            if(bossHP != bossHPmax) {
                endIndex = bossHP / 300 + 1;
            }
            for(int i = 0; i < endIndex; i++){
                presentImage(&hpBar, bossHP_pos.baseX + (i%8)*bossHP_pos.offsetX, bossHP_pos.baseY);
            }
            loadFont(&microsoftBlack, "res/font/microsoftBlack.ttf", 35, 255, 74, 74, 0);
            char bossHP_font[12];
            sprintf(bossHP_font, "%d/%d",bossHP,bossHPmax);
            presentFont(&microsoftBlack, bossHP_font, 1720, 985);
        }
    }
}

void gearSelectUI(){    // 选择义体
    int baseX = 178;
    int baseY = 245;
    int offsetX = 244;
    int option = 0;
    int optionNum = 3;
    int optionalGear[3] = {12,12,12};
    // 生成可选义体
    if(12 - level >= 3){
        optionNum = 3;
    }else if(12 - level >= 2){
        optionNum = 2;
    }else {
        optionNum = 1;
    }
    for(int i = 0; i < optionNum; i++){
        do {
            optionalGear[i] = rand() % 11;
        } while(haveGear[optionalGear[i]] != 0 || (optionalGear[i] == optionalGear[1] && i != 1)
                || (optionalGear[i] == optionalGear[0] && i != 0));
    }
    char* gearName[11] = {"斯安威斯坦","击杀治疗","生物塑料血管","歧路司义眼","震慑通电","纳米继电器",
                         "皮下护甲","突触信号优化件","智能连接","射弹发射系统","踝部加固"};
    char* gearDescription[11] = {"按下E后5秒内敌人时间停止，冷却时间60秒","击杀一个敌人有5%的概率回复1点生命值",
                                 "物品治疗效果+1","每次击杀获得金钱+1，连击奖励＋20%","受到敌人的碰撞伤害后放电将其击杀",
                                 "增加斯安威斯坦的持续时间2秒","受到伤害时有50%的概率只减少一点生命值","增加受到伤害后的无敌时间1秒",
                                 "通过射击击杀敌人可以回复2点能量","射击伤害+1","踩死敌人后回满能量"};
    // 循环 ------------------------------------------------
    while(1) {
        SDL_RenderClear(renderer);
        SDL_Event gearEvent;
        SDL_PollEvent(&gearEvent);
        switch (gearEvent.type) {
            case SDL_QUIT:
                QUIT();
                break;
            case SDL_KEYDOWN:
                switch (gearEvent.key.keysym.sym) {
                    case SDLK_SPACE:
                    case SDLK_RSHIFT:
                    case SDLK_w:
                        Mix_PlayChannel(-1, chunks[0], 0);
                        haveGear[optionalGear[option]] = 1;
                        return;
                        break;
                    case SDLK_a:
                        option = (option - 1 + optionNum) % optionNum;
                        break;
                    case SDLK_d:
                        option = (option + 1) % optionNum;
                        break;
                    default:
                        break;
                }
                break;
            default:
                break;
        }
        // 渲染
        presentImage(&gearSelect, 0 , 0 );
        presentImage(&gearBox, 145 + option*offsetX, 220 );
        loadFont(&microsoftBlack, "res/font/microsoftBlack.ttf", 70, 59, 211, 255, 0);
        presentFont(&microsoftBlack,gearName[optionalGear[option]],150,500);     // 义体名称
        loadFont(&microsoftBlack, "res/font/microsoftBlack.ttf", 35, 255, 74, 74, 0);
        presentFont(&microsoftBlack,gearDescription[optionalGear[option]],150, 650); //  义体描述
        for (int i = 0; i < 3 ; i++){
            if(optionalGear[i] != 12){
                presentImage(&gear[optionalGear[i]], baseX + i*offsetX , baseY);
            }
        }
        SDL_RenderPresent(renderer);
    }
}

void shopUI(int side){  // 商店界面
    const int heroY = 400;
    double heroX = 960;
    double v_move = 0;
    const double v_move_max = 1.3;
    clock_t timerLeftDown = clock()-2*CLOCKS_PER_SEC;
    clock_t timerRightDown = clock()-2*CLOCKS_PER_SEC;
    clock_t FPS_timer = clock()-2*CLOCKS_PER_SEC;
    int leftBoundary = 680;
    int rightBoundary = 1240;
    int direction = side;
    int isShopping = 0;
    int option = 0;
    int leftDown = 0;
    int rightDown = 0;
    int spaceDown = 0;
    charge = maxCharge;
    struct imagePosition{
        int baseX;
        int baseY;
        int offsetX;
        int offsetY;
    };
    struct imagePosition itemBoxPos = {1385, 181, 285, 266};
    SDL_Rect portalRect;
    createRect(&portalRect,0,heroY + normalL.rect.h - portal.rect.h, portal.rect.w,  portal.rect.h);
    SDL_Rect heroCollision;
    createRect(&heroCollision, (int)heroX, heroY, normalL.rect.w, normalL.rect.h);
    SDL_Rect merchantRect;
    createRect(&merchantRect,862, 274,194,170);
    if(side == 1){
        portalRect.x = 680;
        heroX = 680+70;
    }else{
        portalRect.x = 1240 - portalRect.w;
        heroX = 1240 - 70 - heroCollision.w;
    }
    char* itemName[4] = {"返回","倾力治","电池","负重增强件"};
    char* itemDescription[4] = {"退出交易","回复2点生命值","增加1点能量上限","增加1点生命上限"};

    while(1){

        // 捕捉操作
        int leftDown1 = 0;
        int rightDown1 = 0;
        int spaceDown1 = 0;
        SDL_Event actionEvent;
        SDL_PollEvent(&actionEvent);
        switch (actionEvent.type){
            case SDL_QUIT:
                QUIT();
                break;
            case SDL_KEYDOWN:
                switch (actionEvent.key.keysym.sym) {
                    case SDLK_SPACE:
                    case SDLK_w:
                    case SDLK_RSHIFT:
                        spaceDown = 1;
                        spaceDown1 = 1;
                        break;
                    case SDLK_a:
                    case SDLK_LEFT:
                        timerLeftDown = clock();
                        leftDown = 1;
                        leftDown1 = 1;
                        break;
                    case SDLK_d:
                    case SDLK_RIGHT:
                        timerRightDown = clock();
                        rightDown = 1;
                        rightDown1 = 1;
                        break;
                    default:
                        break;
                }
                break;
            case SDL_KEYUP:
                switch (actionEvent.key.keysym.sym) {
                    case SDLK_SPACE:
                    case SDLK_w:
                    case SDLK_RSHIFT:
                        spaceDown = 0;
                        break;
                    case SDLK_a:
                    case SDLK_LEFT:
                        leftDown = 0;
                        break;
                    case SDLK_d:
                    case SDLK_RIGHT:
                        rightDown = 0;
                        break;
                    default:
                        break;
                }
                break;
            default:
                break;
        }
        // 购买
        SDL_Rect result1;
        if(SDL_IntersectRect(&heroCollision, &merchantRect,&result1) == SDL_TRUE && spaceDown1 && isShopping == 0) {
            isShopping = 1;
            option = 1;
            spaceDown1 = 0;
        }
        if(isShopping){
            if(leftDown1){
                option = (option - 1 + 4) % 4;
            }
            if(rightDown1){
                option = (option + 1) % 4;
            }
            if(spaceDown1){
                switch(option){
                    case 0:
                        isShopping = 0;
                        break;
                    case 1:
                        if(money >= 300){
                            Mix_PlayChannel(-1, chunks[2], 0);
                            money -= 300;
                            if(haveGear[2]){
                                if(HP <= maxHP - 3){
                                    HP += 3;
                                }else {
                                    HP = maxHP;
                                }
                            }else {
                                if (HP <= maxHP - 2) {
                                    HP += 2;
                                } else {
                                    HP = maxHP;
                                }
                            }
                        }
                        break;
                    case 2:
                        if(money >= 500){
                            Mix_PlayChannel(-1, chunks[2], 0);
                            money -= 500;
                            maxCharge += 1;
                            charge = maxCharge;
                        }
                        break;
                    case 3:
                        if(money >= 500){
                            Mix_PlayChannel(-1, chunks[2], 0);
                            money -= 500;
                            maxHP++;
                            if(haveGear[2]){
                                if(HP <= maxHP - 2){
                                    HP += 2;
                                }else {
                                    HP = maxHP;
                                }
                            }else {
                                HP++;
                            }
                        }
                        break;
                    default:
                        break;
                }
            }
        }

        // 角色水平移动
        if(isShopping == 0) {
            if (((double) (clock() - timerLeftDown) / CLOCKS_PER_SEC <= 2.0) && leftDown == 1) {
                v_move = -v_move_max;
                direction = 0;
            } else if (((double) (clock() - timerRightDown) / CLOCKS_PER_SEC <= 2.0) && rightDown == 1) {
                v_move = v_move_max;
                direction = 1;
            } else {
                v_move = 0;
            }
            if (heroX + v_move > (double) leftBoundary && heroX + normalL.rect.w + v_move < (double) rightBoundary &&
                v_move != 0.0) {
                heroX = heroX + v_move;
            } else if (heroX + v_move <= (double) leftBoundary) {
                heroX = (double) leftBoundary + 1;
            } else if (heroX + normalL.rect.w + v_move >= (double) rightBoundary) {
                heroX = (double) rightBoundary - normalL.rect.w - 1;
            }
        }
        heroCollision.x = (int)heroX;
        SDL_Rect result2;
        if(SDL_IntersectRect(&heroCollision, &portalRect, &result2) == SDL_TRUE){
            return;
        }
        // 渲染
        SDL_RenderClear(renderer);
        presentImage(&shop, 0, 0);  // 背景
        infoPresent(1);     // 信息
        presentImage(&portal, portalRect.x, portalRect.y);  // 传送门
        if(direction == 0){     // 人物
            presentImage(&normalL,(int)heroX,heroY);
        }else if(direction == 1){
            presentImage(&normalR,(int)heroX,heroY);
        }
        if(isShopping){
            loadFont(&microsoftBlack, "res/font/microsoftBlack.ttf", 70, 59, 211, 255, 0);
            presentFont(&microsoftBlack,itemName[option],1385,725);     // 物品名称
            loadFont(&microsoftBlack, "res/font/microsoftBlack.ttf", 35, 255, 74, 74, 0);
            presentFont(&microsoftBlack,itemDescription[option],1385, 875); //  物品描述
            if( option >= 1) {
                presentImage(&items[option - 1], 855, 638); // 物品
            }
            presentImage(&itemBox, itemBoxPos.baseX + (option%2)*itemBoxPos.offsetX, itemBoxPos.baseY + (option/2)*itemBoxPos.offsetY); // 物品框
        }
        SDL_Rect result3;
        if(SDL_IntersectRect(&heroCollision, &merchantRect,&result3) == SDL_TRUE) {  // 欢迎光临对话框
            presentImage(&welcome, 866, 130);
        }
        SDL_RenderPresent(renderer);
    }
}

void mineUI(int *haveMine, int side, int mineType){     // 奖励房间界面
    const int heroY = 400;
    double heroX = 960;
    double v_move = 0;
    const double v_move_max = 1.3;
    clock_t timerLeftDown = clock()-2*CLOCKS_PER_SEC;
    clock_t timerRightDown = clock()-2*CLOCKS_PER_SEC;
    clock_t FPS_timer = clock()-2*CLOCKS_PER_SEC;
    int leftBoundary = 680;
    int rightBoundary = 1240;
    int direction = side;
    int leftDown = 0;
    int rightDown = 0;
    int spaceDown = 0;
    charge = maxCharge;
    SDL_Rect portalRect;
    createRect(&portalRect,0,heroY + normalL.rect.h - portal.rect.h, portal.rect.w,  portal.rect.h);
    SDL_Rect heroCollision;
    createRect(&heroCollision, (int)heroX, heroY, normalL.rect.w, normalL.rect.h);
    SDL_Rect mineRect;
    if(*haveMine) {
        if (mineType == 0) {
            createRect(&mineRect, 940, 400, goldMine.rect.w, goldMine.rect.h);
        } else {
            createRect(&mineRect, 940, 400, energyMine.rect.w, energyMine.rect.h);
        }
    }else {
        createRect(&mineRect, 0, 0, 0, 0);
    }
    if(side == 1){
        portalRect.x = 680;
        heroX = 680+70;
    }else{
        portalRect.x = 1240 - portalRect.w;
        heroX = 1240 - 70 - heroCollision.w;
    }

    while(1){
        // 捕捉操作
        SDL_Event actionEvent;
        SDL_PollEvent(&actionEvent);
        switch (actionEvent.type){
            case SDL_QUIT:
                QUIT();
                break;
            case SDL_KEYDOWN:
                switch (actionEvent.key.keysym.sym) {
                    case SDLK_SPACE:
                    case SDLK_w:
                    case SDLK_RSHIFT:
                        spaceDown = 1;
                        break;
                    case SDLK_a:
                    case SDLK_LEFT:
                        timerLeftDown = clock();
                        leftDown = 1;
                        break;
                    case SDLK_d:
                    case SDLK_RIGHT:
                        timerRightDown = clock();
                        rightDown = 1;
                        break;
                    default:
                        break;
                }
                break;
            case SDL_KEYUP:
                switch (actionEvent.key.keysym.sym) {
                    case SDLK_SPACE:
                    case SDLK_w:
                    case SDLK_RSHIFT:
                        spaceDown = 0;
                        break;
                    case SDLK_a:
                    case SDLK_LEFT:
                        leftDown = 0;
                        break;
                    case SDLK_d:
                    case SDLK_RIGHT:
                        rightDown = 0;
                        break;
                    default:
                        break;
                }
                break;
            default:
                break;
        }

        // 角色水平移动与碰撞
        if (((double) (clock() - timerLeftDown) / CLOCKS_PER_SEC <= 2.0) && leftDown == 1) {
            v_move = -v_move_max;
            direction = 0;
        } else if (((double) (clock() - timerRightDown) / CLOCKS_PER_SEC <= 2.0) && rightDown == 1) {
            v_move = v_move_max;
            direction = 1;
        } else {
            v_move = 0;
        }
        if (heroX + v_move > (double) leftBoundary && heroX + normalL.rect.w + v_move < (double) rightBoundary &&
        v_move != 0.0) {
            heroX = heroX + v_move;
        } else if (heroX + v_move <= (double) leftBoundary) {
            heroX = (double) leftBoundary + 1;
        } else if (heroX + normalL.rect.w + v_move >= (double) rightBoundary) {
            heroX = (double) rightBoundary - normalL.rect.w - 1;
        }
        heroCollision.x = (int)heroX;
        SDL_Rect result1;
        if(SDL_IntersectRect(&heroCollision, &mineRect, &result1) == SDL_TRUE && *haveMine) {
            *haveMine = 0;
            if(mineType == 0){
                money += 200;
            }else {
                if(rand()%100 < 50){
                    if(HP <= maxHP - 2){
                        HP += 2;
                    }else{
                        HP = maxHP;
                    }
                }else{
                    maxHP += 1;
                }
            }
        }
        SDL_Rect result2;
        if(SDL_IntersectRect(&heroCollision, &portalRect, &result2) == SDL_TRUE){
            return;
        }
        // 渲染
        SDL_RenderClear(renderer);
        presentImage(&mineRoom, 0, 0);  // 背景
        infoPresent(1);     // 信息
        presentImage(&portal, portalRect.x, portalRect.y);  // 传送门
        if(direction == 0){     // 人物
            presentImage(&normalL,(int)heroX,heroY);
        }else if(direction == 1){
            presentImage(&normalR,(int)heroX,heroY);
        }
        if(*haveMine) {  // 矿石
            if (mineType == 0) {
                presentImage(&goldMine, 940, 400);
            } else {
                presentImage(&energyMine, 940, 400);
            }
        }

        SDL_RenderPresent(renderer);
    }
}

int endUI(){ // 游戏结局界面；返回值： 0重开，1退出
    while(1) {
        SDL_RenderClear(renderer);
        SDL_Event endEvent;
        SDL_PollEvent(&endEvent);
        switch (endEvent.type) {
            case SDL_QUIT:
                QUIT();
                break;
            case SDL_KEYDOWN:
                switch (endEvent.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        return 1;
                        break;
                    case SDLK_RETURN:
                        return 0;
                        break;
                    default:
                        break;
                }
                break;
            default:
                break;
        }
        presentImage(&endBG, 0, 0);
        SDL_RenderPresent(renderer);
    }
}