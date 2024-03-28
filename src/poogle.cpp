#include <sdl_helper.h>
#include <all_header.h>

#define STOP_VAL 0.5
#define FRACTION_F 0.6

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
TTF_Font *gFont = NULL;
TTF_Font *gFont0 = NULL;

SDL_Surface* gScreenSurface = NULL;
SDL_Surface* gPNGSurface = NULL;

LTexture gTextTexture;
SDL_Texture *background;
SDL_Texture *socar;
SDL_Texture *blackhole;

MotionX ballM;

long long Frame = 0;

bool initAndLoad();

#define MAX_RES 150
#define MAX_SUG 5

WordX word[MAX_WORD];
ExTrie* root = createNode(ROOT);
int NumOfWord;

struct timeval t0;
struct timeval t1;
float elapsed;

char p[200],t[200],stts[200];
int nOresS  = 0;
int nOresP = 0;
WordX resultS[MAX_RES],resultP[MAX_RES];

int wP = 0;
int Select = 0;
bool activeS = 0;
int nOresIL = 0;
bool updateT = true;

bool MouseRBD = false;

char waitingTxt[][100] = {
                    "- Buddy ! Say something...",
                    "- Hay are you alive?",
                    "- I'm feeling sleepy..zzz...",
                    "- I'm Hungry..",
                    "- I wana think , gimme some text.."
                };
int TP = 0;

int mode = 0;

bool needSave = false;

#define BLACKHOLE 1

void attachMotion2Obj(SDL_Rect *r, MotionX *m,bool active){
    if(!active){
        r->x = (int)m->pos.x;
        r->y = (int)m->pos.y;
        return;
    }
    m->pos.x += m->v.x;
    m->pos.y += m->v.y;

    m->v.x+=m->a.x;
    m->v.y+=m->a.y;

    m->rotation += m->v.x;

    r->x = (int)m->pos.x;
    r->y = (int)m->pos.y;
}

void initMotion(MotionX *m,int x, int y, double vx, double vy ,double ax, double ay){
    m->pos.x = (double)x; m->pos.y = (double)y;
    m->v.x = vx; m->v.y = vy;
    m->a.x = ax; m->a.y = ay;
    m->hit.x = 0; m->hit.y = 0;
    m->rotation = 0.00;
}

int main( int argc, char* args[] ){
    printf("\033c");
    load_word("../asset/words/preProcess.txt", root , word , &NumOfWord);
    printf("Window Started..\n");
    initAndLoad();

    bool quit = false;
    SDL_Event event;

    SDL_Rect Sbox;
    Sbox.h = 50; Sbox.w = 50; Sbox.x = 10 ; Sbox.y = 10;

    SDL_Rect Wrdbox;
    Wrdbox.h = 52; Wrdbox.w = 500; Wrdbox.x = 230 ; Wrdbox.y = 225;

    SDL_Rect blackbox;
    blackbox.h = 433; blackbox.w = 420; blackbox.x = SCREEN_WIDTH - blackbox.w ; blackbox.y = 0;

    initMotion(&ballM,20,SCREEN_HEIGHT-Sbox.h,0,0,0,0.098);

    while( !quit )
    {
        while( SDL_PollEvent( &event ))
        {
            if( event.type == SDL_QUIT )
            {
                quit = true;
                break;
            }
            if(event.type == SDL_MOUSEMOTION){
                printf("%d : %d\n",event.motion.state,event.motion.y);
                if(event.motion.state == 1){
                    if(event.motion.x>ballM.pos.x
                    && event.motion.x<ballM.pos.x+Sbox.w
                    && event.motion.y>ballM.pos.y
                    && event.motion.y<ballM.pos.y+Sbox.h){
                        MouseRBD = true;
                        //ballM.pos.x = event.motion.x-Sbox.w/2;
                        //ballM.pos.y = event.motion.y-Sbox.h/2;
                        ballM.v.x = event.motion.xrel;
                        ballM.v.y = event.motion.yrel;
                    }
                }
            }
            if(event.type == SDL_MOUSEBUTTONUP){
                MouseRBD = (event.button.button == 1)?false:MouseRBD;
            }
            if(event.type == SDL_MOUSEBUTTONDOWN){
                printf("%d\n",event.button.button);
            }
            if(event.type == SDL_KEYUP){
                updateT = false;
            }
            if(event.type == SDL_KEYDOWN){
                updateT = true;
                if(event.key.keysym.sym == 13){
                    if(!wP)break;
                    if(!search_word(root,p)){
                        WordX wrd;
                        wrd.link = insert_word(root,p);
                        wrd.score = word2Score(p);
                        addWord2lib(word,wrd,&NumOfWord);
                        needSave = true;
                        break;
                    }
                    if(strcmp(p,"exit")==0){
                        quit = 1; break;
                    }
                    if(strcmp(p,"blackhole")==0){mode = BLACKHOLE;break;}
                    char cnnn[35];
                    if(Select==-1)
                        if(nOresS)trackRoot(resultS[0].link,cnnn);
                        else trackRoot(resultP[0].link,cnnn);
                    else{
                        if(nOresS)trackRoot(resultS[Select].link,cnnn);
                        else trackRoot(resultP[Select].link,cnnn);
                        Select = -1;
                    }
                    strcpy(p,cnnn);
                    wP = strlen(p);
                    if(strcmp(p,"exit")==0)quit = 1;
                    if(strcmp(p,"blackhole")==0){mode = 1;break;}
                }else if(event.key.keysym.sym == SDLK_BACKSPACE){
                    if(!wP)break;
                    wP--;
                    p[wP] = '\0';
                    break;
                }else if(event.key.keysym.sym == SDLK_RIGHT){
                    if(!wP)break;
                    char cnnn[35];
                    if(Select==-1)
                        if(nOresS)trackRoot(resultS[0].link,cnnn);
                        else trackRoot(resultP[0].link,cnnn);
                    else{
                        if(nOresS)trackRoot(resultS[Select].link,cnnn);
                        else trackRoot(resultP[Select].link,cnnn);
                        Select = -1;
                    }
                    strcpy(p,cnnn);
                    wP = strlen(p);
                }else if(event.key.keysym.sym == SDLK_DOWN){
                    if(Select==-1){
                        Select = !activeS;
                        printf("%d %d\n",Select,nOresIL);
                        break;
                    }
                    if(Select<nOresIL-1 && Select<MAX_SUG-1)Select++;
                    printf("%d %d\n",Select,nOresIL);
                    break;
                }else if(event.key.keysym.sym == SDLK_UP){
                    if(Select!=-1)Select--;
                    if(!Select && !activeS)Select = -1;
                    printf("%d %d\n",Select,nOresIL);
                    break;
                }else{
                    printf("%c %d %s\n",event.key.keysym.sym,wP,p);
                    p[wP] = (char)event.key.keysym.sym;
                    wP++;
                    p[wP] = '\0';
                    Select = -1;
                }            
            }
        }

        if(wP)
            strcpy(t,p);
        else
            strcpy(t," \0");

        if(!wP){
            p[0] = ' ';
            p[1] = '\0';
            stts[0] = ' ';
            stts[1] = '\0';
        }      

        if(Select==-1){
            nOresS = 0;
            nOresP = 0;
            nOresIL = 0;

    
            if(wP){
                gettimeofday(&t0, 0);
                suggestion(root,t,0,resultS,MAX_RES,&nOresS);
                sortWord(resultS,nOresS,NO_STATUS);
                gettimeofday(&t1, 0);
                elapsed = timedifference_msec(t0, t1)*1000;
                nOresIL = nOresS;
                char dd[8];
                if(!nOresS){
                    strcpy(t,p);
                    gettimeofday(&t0, 0);
                    int ip = knnWordSearch(word,NumOfWord-1,word2Score(t));
                    searchNear(word,resultP,t,NumOfWord-1,ip,25,1,2,5,0,&nOresP);
                    sortWord(resultP,nOresP,NO_STATUS);
                    gettimeofday(&t1, 0);
                    elapsed = timedifference_msec(t0, t1)*1000;
                    nOresIL = nOresP;
                }
                if(updateT){
                    if(nOresS)SDL_itoa(nOresS,dd,10);
                    else SDL_itoa(nOresP,dd,10);
                    strcpy(stts,dd);
                    if(nOresS)strcat(stts," suggstion found in ");
                    else strcat(stts," prediction found in ");
                    SDL_itoa((int)elapsed,dd,10);
                    strcat(stts,dd);
                    strcat(stts," microseconds");
                    
                }
            }
        }
        //Clear screen
        if(mode == 0)
            SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
        else if(mode == BLACKHOLE)
            SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );  
        //SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
        SDL_RenderClear( gRenderer );

        if(mode == 0)
        SDL_RenderCopy(gRenderer, background, NULL, NULL);
        if(mode == BLACKHOLE){
            SDL_RenderCopy(gRenderer, blackhole, NULL, &blackbox);
        }
        
        if(wP&&!nOresS&&updateT&&Select==-1&&!search_word(root,p)){
            if(ballM.v.y>0)
            ballM.v.y+=1.5;
            else
            ballM.v.y-=1.5;
        }

        if(ballM.pos.y>=SCREEN_HEIGHT-Sbox.h||ballM.pos.y<=0){
            ballM.hit.y = 1;
            if(ballM.pos.y<=0)
                ballM.pos.y = 0;
            else
                ballM.pos.y = SCREEN_HEIGHT-Sbox.h;
            ballM.v.y = (double)(FRACTION_F)*ballM.v.y;
            ballM.v.x = (double)(FRACTION_F)*ballM.v.x;
            ballM.v.y *= -1;
            if(abs(ballM.v.y)<STOP_VAL)ballM.v.y = 0.0000000;
            if(abs(ballM.v.x)<STOP_VAL)ballM.v.x = 0.0000000;
        }
        if(ballM.pos.x<=0||ballM.pos.x>=SCREEN_WIDTH-Sbox.w){
            if(ballM.pos.x>=SCREEN_WIDTH-Sbox.w)
                ballM.pos.x = SCREEN_WIDTH-Sbox.w;
            else
                ballM.pos.x = 0;
            ballM.v.y = (double)(FRACTION_F)*ballM.v.y;
            ballM.v.x = (double)(FRACTION_F)*ballM.v.x;
            ballM.v.x *= -1;
            if(abs(ballM.v.y)<STOP_VAL)ballM.v.y = 0.0000000;
            if(abs(ballM.v.x)<STOP_VAL)ballM.v.x = 0.0000000;
        }
        attachMotion2Obj(&Sbox,&ballM,!MouseRBD);
        SDL_Color textColor = { 0, 0, 0 };
        SDL_Point pnt;
        pnt.x = (int)Sbox.w/2;
        pnt.y = (int)Sbox.h/2;
        //SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
        //SDL_RenderCopy(gRenderer, socar, NULL, &Sbox);
        SDL_RenderCopyEx(gRenderer,socar,NULL,&Sbox,ballM.rotation,&pnt,SDL_FLIP_NONE);
        if(Frame<150){
            textColor = {64,64,64};
            gTextTexture.loadFromRenderedText("- Hi , I'm poogle",gRenderer , gFont0,textColor);
            gTextTexture.render( ballM.pos.x - 10 , ballM.pos.y - gTextTexture.getHeight() - 5 ,gRenderer);
        }else if(Frame<300){
            textColor = {64,64,64};
            gTextTexture.loadFromRenderedText("- I'm here to assist you",gRenderer , gFont0,textColor);
            gTextTexture.render( ballM.pos.x - 10 , ballM.pos.y - gTextTexture.getHeight() - 5 ,gRenderer);
        }else if(!nOresS&&wP&&!mode){
            char tlk[100],sug[35];
            strcpy(tlk,"- Did you mean : ");
            trackRoot(resultP[0].link,sug);
            //strcat(tlk,sug);
            textColor = {51,51,255};
            gTextTexture.loadFromRenderedText(tlk,gRenderer , gFont0,textColor);
            gTextTexture.render( ballM.pos.x - 10 , ballM.pos.y - gTextTexture.getHeight() - 5 ,gRenderer);
            SDL_Point txtP; txtP.x = ballM.pos.x + gTextTexture.getWidth()-10; txtP.y = ballM.pos.y - gTextTexture.getHeight()-5;
            textColor = {64,64,64};
            gTextTexture.loadFromRenderedText(sug,gRenderer , gFont0,textColor);
            gTextTexture.render( txtP.x , txtP.y ,gRenderer);
        }else if(mode == BLACKHOLE){
            if(ballM.pos.x+Sbox.w>blackbox.x
            && ballM.pos.y<blackbox.h)mode = 0;
            textColor = {200,200,200};
            gTextTexture.loadFromRenderedText("- Through me to the black hole..",gRenderer , gFont0,textColor);
            gTextTexture.render( ballM.pos.x - 10 , ballM.pos.y - gTextTexture.getHeight() - 5 ,gRenderer);
        }else if(Frame%450>300&&!wP){
            textColor = {64,64,64};
            gTextTexture.loadFromRenderedText(waitingTxt[TP],gRenderer , gFont0,textColor);
            gTextTexture.render( ballM.pos.x - 10 , ballM.pos.y - gTextTexture.getHeight() - 5 ,gRenderer);
        }else{
            TP = (int)(rand() % 4 + 1);
        }

        textColor = { 0, 0, 0 };
        
        Wrdbox.h = 52;
        for(int i = 0;i<nOresIL&&i<5;i++){
            char c[35];
            activeS = 0;
            if(nOresS)trackRoot(resultS[i].link,c);
            else trackRoot(resultP[i].link,c);
            if(search_word(root,p))activeS = 1;
            if(!nOresS) activeS = 1;
            if(!i&&!activeS)
                textColor = {170,165,180};
            else
                textColor = {15,15,15};
            if(i == Select && Select > -1){
                SDL_Rect Srdbox;
                Srdbox.h = 35; Srdbox.w = 500; Srdbox.x = 230 ; Srdbox.y = 235 + ((i+activeS)*30);
                SDL_SetRenderDrawColor( gRenderer, 205, 205, 208 , 150 );
                SDL_RenderDrawRect(gRenderer,&Srdbox);
                SDL_RenderFillRect(gRenderer,&Srdbox);
            }
            gTextTexture.loadFromRenderedText( c,gRenderer , gFont,textColor);
            gTextTexture.render( 270 , 232 + ((i+activeS)*30) ,gRenderer);
            Wrdbox.h = 52 + (30*(i+activeS));
        }
        SDL_SetRenderDrawColor( gRenderer, 180, 180, 180 , 150 );
        if(mode == 0)
        SDL_RenderDrawRect(gRenderer,&Wrdbox);
        //gTextTexture.loadFromRenderedText( "Another",gRenderer , gFont,textColor);
        //gTextTexture.render( 10, 10 ,gRenderer);

        textColor = { 0, 0, 0 };

        gTextTexture.loadFromRenderedText(p,gRenderer , gFont,textColor);
        gTextTexture.render( 270 , 232 ,gRenderer);

        if(search_word(root,p)||nOresS||!wP)
            SDL_SetRenderDrawColor( gRenderer, 0, 0, 0 , 0 );
        else
            SDL_SetRenderDrawColor( gRenderer, 225, 10, 15 , 255 );

        if(Frame%30<18)
            SDL_RenderDrawLine(gRenderer, 270 + gTextTexture.getWidth() +1 , 236, 270 + gTextTexture.getWidth() + 1 , 232+30);

        textColor = {150,150,160};
        gTextTexture.loadFromRenderedText(stts,gRenderer , gFont0,textColor);
        gTextTexture.render( SCREEN_WIDTH - gTextTexture.getWidth() - 10 , SCREEN_HEIGHT - gTextTexture.getHeight() - 5 ,gRenderer);
        //SDL_RenderCopy(gRenderer, background, NULL, NULL);

        SDL_RenderPresent( gRenderer );
        //SDL_RenderPresent( gRenderer );

        //SDL_BlitSurface( gPNGSurface, NULL, gScreenSurface, NULL );

        Frame++;
        SDL_Delay(30);
    }
	close(gWindow,gRenderer);
    if(needSave){
        saveAll(word,NumOfWord);
        printf("Saved..\n");
    }
	return 0;
}

bool initAndLoad(){
    bool ret = true;
    if( !init("Poogle",gWindow,gRenderer,gScreenSurface) )
	{
		printf( "Failed to initialize!\n" );
        return 0;
	}else {
        if(!loadMedia("../asset/fonts/SerifR.ttf",gRenderer,gFont,&gTextTexture,28) )
		{
			printf( "Failed to load ttf!\n" );
            ret = false;
		}
        if(!loadMedia("../asset/fonts/SerifR.ttf",gRenderer,gFont0,&gTextTexture,14) )
		{
			printf( "Failed to load ttf!\n" );
            ret = false;
		}
        if(!loadMedia("../asset/picture/Project Poogle_2.png",background,gRenderer)){
            printf( "Failed to load png!\n" );
            ret = false;
        }
        if(!loadMedia("../asset/picture/ball.png",socar,gRenderer)){
            printf( "Failed to load png!\n" );
            ret = false;
        }
        if(!loadMedia("../asset/picture/blackhole.jpg",blackhole,gRenderer)){
            printf( "Failed to load png!\n" );
            ret = false;
        }
    }
    return ret;
}