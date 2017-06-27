/*=============================================================================
		A simple pong game written in C using SDL 
		
To compile install libsdl2 and run:
	gcc main.c -lSDL2 -o pong
	./pong
===============================================================================*/

#include<stdio.h>
#include<SDL2/SDL.h>
#include<SDL2/SDL_render.h>

#define WINDOW_WIDTH 720
#define WINDOW_HEIGHT 480

int winw,winh,pad1y,pad2y;	

typedef struct {
	SDL_Rect rect;
	int xvel,yvel;
}Obj;

Obj newObj(int x,int y,int w,int h,int xvel,int yvel){
	Obj new;
	new.rect.w=w;	//width
	new.rect.h=h;	//height
	new.rect.x=x;	//x position of the left topmost point
	new.rect.y=y;	//y position of the left topmost point
	new.xvel=xvel;	//x velocity
	new.yvel=yvel;	//y velocity
	return new;
}

void update(Obj * object,int isball,int up){
	if (isball==1){		//execute if it is the ball
		object->rect.x+=object->xvel;
		object->rect.y+=object->yvel;
		if( (object->rect.x>winw-object->rect.w) || (object->rect.x<0) ){				//ball goes out of screen
			SDL_Delay(1000);
			object->xvel=-object->xvel;
			object->rect.x=winw/2;
			object->rect.y=winh/2;
		}
		if(object->rect.x>winw-20-object->rect.w && object->rect.y>pad2y && object->rect.y<pad2y+100){	//ball hits pad2
			object->xvel=-object->xvel;
			object->rect.x=winw-20-object->rect.w;
		}
		if(object->rect.x<20 && object->rect.y>pad1y && object->rect.y<pad1y+100){			//ball hits pad1
			object->xvel=-object->xvel;
			object->rect.x=20;
		}	
		if(object->rect.y>winh-object->rect.h){								//ball hits the bottom edge of the window
			object->yvel=-object->yvel;
			object->rect.y=winh-object->rect.h;
		}
		if(object->rect.y<0){										//ball hits the top edge of the window
			object->yvel=-object->yvel;
			object->rect.y=0;
		}
	}else {			//execute if it is one of the pads
		if(up==1)
			object->rect.y-=object->yvel;
		else
			object->rect.y+=object->yvel;
		if(object->rect.y>winh-object->rect.h){								//pad hits the bottom edge of the window
			object->rect.y=winh-object->rect.h;
		}
		if(object->rect.y<0){										//pad hits the top edge of the window
			object->rect.y=0;
		}
	}
	return;
}

int main(int argc,char argv[]){
	
	SDL_Window * win=NULL;
	SDL_Renderer * renderer=NULL;
	
	//initializing sdl
	if(SDL_Init(SDL_INIT_VIDEO)!=0)
	{
		printf("SDL error %s\n",SDL_GetError());
		return 1;
	}
	
	//creating a window and a renderer
	win = SDL_CreateWindow("Pong",
				SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,
				WINDOW_WIDTH,WINDOW_HEIGHT,0);
	if(win==NULL){
		printf("Window cannot be created %s\n",SDL_GetError());
		return 1;
	}	
	SDL_GetWindowSize(win,&winw,&winh);
	renderer = SDL_CreateRenderer(win,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);	
	if(renderer==NULL){
		printf("Renderer cannot be created %s\n",SDL_GetError());
		return 1;
	}	

	//creating objects
	Obj pad1 = newObj(5,(winh-100)/2,15,100,0,10);
	Obj pad2 = newObj(winw-20,(winh-100)/2,15,100,0,10);	
	Obj ball = newObj(winw/2,winh/2,10,10,5,5);
	Obj cline = newObj((winw-2)/2,0,2,winh,0,0);

	int terminate=0,up=0,down=0,w=0,s=0;
	while(!terminate)				//game loop
	{
		SDL_Event e;
		while(SDL_PollEvent(&e)!=0){		//handling events
			switch (e.type){
				case SDL_QUIT:
					terminate=1;
					continue;
					break;
				case SDL_KEYDOWN:
					switch(e.key.keysym.sym){
						case SDLK_ESCAPE:
							terminate=1;
							break;
						case SDLK_UP:
							up=1;
							break;
						case SDLK_DOWN:
							down=1;
							break;
						case SDLK_w:
							w=1;
							break;
						case SDLK_s:
							s=1;
							break;
					}
					break;	
				case SDL_KEYUP:
					switch(e.key.keysym.sym){
						case SDLK_UP:
							up=0;
							break;
						case SDLK_DOWN:
							down=0;
							break;
						case SDLK_w:
							w=0;
							break;
						case SDLK_s:
							s=0;
							break;
					}
					break;		
			}
		}

		pad1y=pad1.rect.y;				//copies y pos of pad1 and pad2 to global variables pad1y and pad2y
		pad2y=pad2.rect.y;				//to be used by the update function to check for ball and pad collision.

		update(&ball,1,0);				//updates ball position

		SDL_SetRenderDrawColor(renderer,0,0,255,0);	//set renderer color to blue	
		SDL_RenderClear(renderer);			//renders blue background
	
		if(up) update(&pad2,0,1);
		if(down) update(&pad2,0,0);
		if(w) update(&pad1,0,1);
		if(s) update(&pad1,0,0);

		SDL_SetRenderDrawColor(renderer,255,255,255,0); //set renderer color to white
		SDL_RenderFillRect(renderer,&(cline.rect));	//render the center line	
		SDL_RenderFillRect(renderer,&(ball.rect));	//render the ball
		SDL_RenderFillRect(renderer,&(pad1.rect));	//render pad1
		SDL_RenderFillRect(renderer,&(pad2.rect));	//render pad2	

		SDL_RenderPresent(renderer);			//render all of that to the screen finally
		SDL_Delay(1000/60);				
		
	}



	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0;
}
