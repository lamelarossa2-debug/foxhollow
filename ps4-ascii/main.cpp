// ROBERTO ASCII REALM - PS4 graphics + DualShock smoke test
// Native OpenOrbis, software ASCII glyph rendering. No copyrighted assets.
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sstream>
#include <orbis/Pad.h>
#include <orbis/UserService.h>
#include "../../_common/graphics.h"
#include "../../_common/log.h"

std::stringstream debugLogStream;
static const int SW = 1920, SH = 1080;
// Native 1920x1080 full-bleed character grid (no black frame/margins).
static const int COLS = 120, ROWS = 60, CW=16, CH=18;
static const int LEFT = 0, TOP = 0;
struct RGB { unsigned char r,g,b; };
static RGB color(int r,int g,int b){ RGB k={(uint8_t)r,(uint8_t)g,(uint8_t)b}; return k; }
static Color toColor(RGB a){ Color c={a.r,a.g,a.b};return c;}
static void rect(Scene2D* s,int x,int y,int w,int h,RGB c) {
 if (w<=0 || h<=0 || x<0 || y<0 || x+w>SW || y+h>SH)return;
 s->DrawRectangle(x,y,w,h,toColor(c));
}
static float hash2(int x,int y) {
 unsigned int n = (unsigned)x*374761393u + (unsigned)y*668265263u + 0x9e3779b9u;
 n=(n^(n>>13))*1274126177u; n^=n>>16;
 return (float)(n&65535)/65535.0f;
}
static float smooth(float x) { return x*x*(3-2*x); }
static float noise(float x,float y) {
 int a=(int)floorf(x), b=(int)floorf(y);
 float u=smooth(x-a), v=smooth(y-b);
 float z0=hash2(a,b)*(1-u)+hash2(a+1,b)*u;
 float z1=hash2(a,b+1)*(1-u)+hash2(a+1,b+1)*u;
 return z0*(1-v)+z1*v;
}
static void glyph(char c, uint8_t *p) {
 // 5x7 monospace. Rows contain 5 left-aligned columns.
 memset(p,0,7);
 switch(c){
 case 'A': {uint8_t a[]={14,17,17,31,17,17,17};memcpy(p,a,7);break;}
 case 'B': {uint8_t a[]={30,17,17,30,17,17,30};memcpy(p,a,7);break;}
 case 'C': {uint8_t a[]={15,16,16,16,16,16,15};memcpy(p,a,7);break;}
 case 'D': {uint8_t a[]={30,17,17,17,17,17,30};memcpy(p,a,7);break;}
 case 'E': {uint8_t a[]={31,16,16,30,16,16,31};memcpy(p,a,7);break;}
 case 'F': {uint8_t a[]={31,16,16,30,16,16,16};memcpy(p,a,7);break;}
 case 'G': {uint8_t a[]={15,16,16,19,17,17,15};memcpy(p,a,7);break;}
 case 'H': {uint8_t a[]={17,17,17,31,17,17,17};memcpy(p,a,7);break;}
 case 'I': {uint8_t a[]={31,4,4,4,4,4,31};memcpy(p,a,7);break;}
 case 'J': {uint8_t a[]={7,2,2,2,18,18,12};memcpy(p,a,7);break;}
 case 'K': {uint8_t a[]={17,18,20,24,20,18,17};memcpy(p,a,7);break;}
 case 'L': {uint8_t a[]={16,16,16,16,16,16,31};memcpy(p,a,7);break;}
 case 'M': {uint8_t a[]={17,27,21,21,17,17,17};memcpy(p,a,7);break;}
 case 'N': {uint8_t a[]={17,25,21,19,17,17,17};memcpy(p,a,7);break;}
 case 'O': {uint8_t a[]={14,17,17,17,17,17,14};memcpy(p,a,7);break;}
 case 'P': {uint8_t a[]={30,17,17,30,16,16,16};memcpy(p,a,7);break;}
 case 'Q': {uint8_t a[]={14,17,17,17,21,18,13};memcpy(p,a,7);break;}
 case 'R': {uint8_t a[]={30,17,17,30,20,18,17};memcpy(p,a,7);break;}
 case 'S': {uint8_t a[]={15,16,16,14,1,1,30};memcpy(p,a,7);break;}
 case 'T': {uint8_t a[]={31,4,4,4,4,4,4};memcpy(p,a,7);break;}
 case 'U': {uint8_t a[]={17,17,17,17,17,17,14};memcpy(p,a,7);break;}
 case 'V': {uint8_t a[]={17,17,17,17,17,10,4};memcpy(p,a,7);break;}
 case 'W': {uint8_t a[]={17,17,17,21,21,21,10};memcpy(p,a,7);break;}
 case 'X': {uint8_t a[]={17,17,10,4,10,17,17};memcpy(p,a,7);break;}
 case 'Y': {uint8_t a[]={17,17,10,4,4,4,4};memcpy(p,a,7);break;}
 case 'Z': {uint8_t a[]={31,1,2,4,8,16,31};memcpy(p,a,7);break;}
 case '0': {uint8_t a[]={14,19,21,21,21,25,14};memcpy(p,a,7);break;}
 case '1': {uint8_t a[]={4,12,4,4,4,4,14};memcpy(p,a,7);break;}
 case '2': {uint8_t a[]={14,17,1,2,4,8,31};memcpy(p,a,7);break;}
 case '3': {uint8_t a[]={30,1,1,14,1,1,30};memcpy(p,a,7);break;}
 case '4': {uint8_t a[]={2,6,10,18,31,2,2};memcpy(p,a,7);break;}
 case '5': {uint8_t a[]={31,16,16,30,1,1,30};memcpy(p,a,7);break;}
 case '6': {uint8_t a[]={15,16,16,30,17,17,14};memcpy(p,a,7);break;}
 case '7': {uint8_t a[]={31,1,2,4,8,8,8};memcpy(p,a,7);break;}
 case '8': {uint8_t a[]={14,17,17,14,17,17,14};memcpy(p,a,7);break;}
 case '9': {uint8_t a[]={14,17,17,15,1,1,30};memcpy(p,a,7);break;}
 case '@': {uint8_t a[]={14,17,23,21,23,16,14};memcpy(p,a,7);break;}
 case '#': {uint8_t a[]={10,10,31,10,31,10,10};memcpy(p,a,7);break;}
 case '$': {uint8_t a[]={4,15,20,14,5,30,4};memcpy(p,a,7);break;}
 case '%': {uint8_t a[]={17,18,4,4,8,19,17};memcpy(p,a,7);break;}
 case '+': {uint8_t a[]={0,4,4,31,4,4,0};memcpy(p,a,7);break;}
 case '*': {uint8_t a[]={0,21,14,31,14,21,0};memcpy(p,a,7);break;}
 case '.': {uint8_t a[]={0,0,0,0,0,12,12};memcpy(p,a,7);break;}
 case ',': {uint8_t a[]={0,0,0,0,4,4,8};memcpy(p,a,7);break;}
 case ':': {uint8_t a[]={0,12,12,0,12,12,0};memcpy(p,a,7);break;}
 case ';': {uint8_t a[]={0,12,12,0,12,4,8};memcpy(p,a,7);break;}
 case '-': {uint8_t a[]={0,0,0,31,0,0,0};memcpy(p,a,7);break;}
 case '=': {uint8_t a[]={0,0,31,0,31,0,0};memcpy(p,a,7);break;}
 case '~': {uint8_t a[]={0,0,9,22,0,0,0};memcpy(p,a,7);break;}
 case '^': {uint8_t a[]={4,10,17,0,0,0,0};memcpy(p,a,7);break;}
 case '/': {uint8_t a[]={1,2,2,4,8,8,16};memcpy(p,a,7);break;}
 case 92: {uint8_t a[]={16,8,8,4,2,2,1};memcpy(p,a,7);break;}
 case '|': {uint8_t a[]={4,4,4,4,4,4,4};memcpy(p,a,7);break;}
 case '(': {uint8_t a[]={2,4,8,8,8,4,2};memcpy(p,a,7);break;}
 case ')': {uint8_t a[]={8,4,2,2,2,4,8};memcpy(p,a,7);break;}
 case '[': {uint8_t a[]={14,8,8,8,8,8,14};memcpy(p,a,7);break;}
 case ']': {uint8_t a[]={14,2,2,2,2,2,14};memcpy(p,a,7);break;}
 case '<': {uint8_t a[]={1,2,4,8,4,2,1};memcpy(p,a,7);break;}
 case '>': {uint8_t a[]={16,8,4,2,4,8,16};memcpy(p,a,7);break;}
 case '!': {uint8_t a[]={4,4,4,4,4,0,4};memcpy(p,a,7);break;}
 case '?': {uint8_t a[]={14,17,1,2,4,0,4};memcpy(p,a,7);break;}
 case '_': {uint8_t a[]={0,0,0,0,0,0,31};memcpy(p,a,7);break;}
 case '"': {uint8_t a[]={10,10,10,0,0,0,0};memcpy(p,a,7);break;}
 case 39:  {uint8_t a[]={4,4,4,0,0,0,0};memcpy(p,a,7);break;}
 default: if(c>='a' && c<='z') glyph(c-'a'+'A',p);break;
 }
}
static void drawChar(Scene2D* scene,int x,int y,char c,RGB ink,int scale) {
 if(c==' ')return;
 uint8_t bits[7];glyph(c,bits);
 for(int j=0;j<7;j++)for(int i=0;i<5;i++)
  if(bits[j] & (1 << (4-i)))rect(scene,x+i*scale,y+j*scale,scale,scale,ink);
}
static void text(Scene2D *s,int x,int y,const char *msg,RGB col,int scale=2) {
 while(*msg){drawChar(s,x,y,*msg,col,scale);x+=6*scale;msg++;}
}
static RGB mix(RGB a,RGB b,float k){
 if(k<0)k=0;if(k>1)k=1;
 return color((int)(a.r*(1-k)+b.r*k),(int)(a.g*(1-k)+b.g*k),(int)(a.b*(1-k)+b.b*k));
}
static void draw_world(Scene2D *s,float cx,float cy,int frame,int pulse,int walking,int face,int shiftX,int shiftY) {
 RGB ink;char ch;
 const float tt=frame*0.027f;
 for(int y=0;y<ROWS;y++){
  for(int x=0;x<COLS;x++){
   float dx=x-COLS*0.5f,dy=y-ROWS*0.5f;
   // Fixed isometric projection: camera follows the player's position.
   float wx=cx+(dx*0.62f +dy*0.53f)*0.78f;
   float wy=cy+(-dx*0.38f+dy*0.62f)*0.78f;
   float n=noise(wx*0.12f,wy*0.12f);
   float d=sqrtf((wx-9.0f)*(wx-9.0f)*0.88f+(wy+3.0f)*(wy+3.0f)*1.12f);
   float shore=11.5f+(noise(wx*0.17f,wy*0.17f)-0.5f)*3.7f;
   float grit=hash2((int)floorf(wx*2.1f),(int)floorf(wy*2.1f));
   int px=LEFT+x*CW,py=TOP+y*CH;
   // Intensity falloff at the edges; stronger torches around player.
   float distance=sqrtf(dx*dx+(dy*1.12f)*(dy*1.12f));
   // Maintain a readable world, with a soft vignette instead of obscuring terrain.
   float spot=0.91f-0.0052f*distance;
   if(spot<0.56f)spot=0.56f;
   // X emits a small travelling LIGHT RING, not a fullscreen brightening.
   float ring=pulse>0 ? (40-pulse)*0.42f : -100.0f;
   float rim=fabsf(distance-ring);
   bool waveFront=(pulse>0 && rim<1.55f && distance<18.0f);
   if(waveFront)spot=fminf(1.0f,spot+(1.55f-rim)*0.40f);
   if(d<shore){
    float wave=sinf(wx*1.4f+tt)*cosf(wy*0.9f+tt*0.71f);
    ch=(wave>0.55f)?'=':((wave<-0.55f)?'-':((grit>0.8f)?'~':'.'));
    float s=0.32f+fmaxf(0,wave)*0.25f;
    ink=mix(color(18,48,71),color(40,151,190),s);
    if(grit>0.995f || (fabsf(d-shore)<0.9f && wave>0.3f)){ch='*';ink=color(77,177,187);}
   }else if(d<shore+1.8f){
    ch=(grit>0.5f)?':':'.';ink=color(117,117,78);
    if(grit>0.89f){ch='+';ink=color(183,156,94);}
   }else{
    float path=fabsf(wy+wx*0.22f+2.4f);
    if(path<1.7f && wx<4.5f){
     ch=(grit>0.71f)?'=':((grit>0.3f)?':':'.');
     ink=color(109,117,107);
    } else if(n>0.67f){
     ch=(grit>0.74f)?'M':((grit>0.35f)?'W':'#');
     ink=(grit>0.69f)?color(79,151,98):color(32,92,68);
    }else if(n>0.56f){
     ch=(grit>0.8f)?'%':((grit>0.45f)?'*':'+');
     ink=(grit>0.55f)?color(104,139,55):color(69,117,51);
    }else if(n<0.30f){
     ch=(grit>0.80f)?'#':((grit>0.54f)?'o':':');
     ink=color(95,105,112);
    }else{
     ch=(grit>0.95f)?'*':((grit>0.73f)?';':((grit>0.45f)?',':'.'));
     ink=color(79,111,68);
    }
    if(grit>0.992f){ ch='^';ink=color(178,135,73);}
    // Ruined stone path runs diagonally through the forest.
    if(wx<-11 && fabsf(wy-12)<2.3f && fmodf(fabsf(wx),6.0f)<3.0f){
      ch=(grit>0.7f)?'#':'=';ink=color(94,107,110);
    }
   }
   // Local cyan pulse wave; the surrounding terrain retains its own shading.
   if(waveFront && hash2(x+frame/3,y)>0.31f){
     ch='*'; ink=mix(ink,color(75,219,230),0.64f);
   }
   ink=mix(color(3,12,21),ink,spot);
   drawChar(s,px+2,py+2,ch,ink,2);
  }
 }
 // Player is visibly animated and drifts briefly as camera follows.
 int X=SW/2-49+shiftX;
 int Y=SH/2-72+shiftY;
 int step=walking ? ((frame/6)%2) : 0;
 int bob=walking ? (step ? 4 : -3) : 0;
 RGB gold=color(244,210,116), robe=color(210,172,117);
 RGB faceColor=face==0?color(239,193,116):color(138,173,157);
 text(s,X+22,Y-25+bob,".^.",color(78,187,184),2);
 text(s,X,Y+bob,    "  /^\\  ",gold,3);
 text(s,X,Y+23+bob, "  (O)  ",faceColor,3);
 text(s,X,Y+46+bob, walking && step ? " \\###/ " : " /###\\ ",robe,3);
 text(s,X,Y+70+bob, walking ? (step?"  /|\\  ":"  / \\  ") : "  / \\  ",color(124,156,171),3);
 if(walking && frame%12<6) text(s,X+12,Y+104,".  .",color(117,180,162),2);
 if(pulse>32) text(s,X-17,Y+36,"*       *",color(92,236,234),3);
}
int main(){
 setvbuf(stdout,NULL,_IONBF,0);
 Scene2D *scene=new Scene2D(SW,SH,4);
 if(!scene->Init(0xC000000,2))for(;;){}
 bool connected=false;int pad=-1;int userID=-1;
 if(scePadInit()==0){
   OrbisUserServiceInitializeParams p={};
   p.priority=ORBIS_KERNEL_PRIO_FIFO_LOWEST;
   sceUserServiceInitialize(&p);
   if(sceUserServiceGetInitialUser(&userID)==0)
     pad=scePadOpen(userID,0,0,NULL);
   connected=pad>=0;
 }
 float cx=0,cy=0,camx=0,camy=0;int pulse=0,frame=0,face=0;uint32_t oldButtons=0;
 for(;;){
   uint32_t buttons=0;
   float oldCx=cx,oldCy=cy;
   if(connected){
     OrbisPadData data={};
     if(scePadReadState(pad,&data)==0){
       buttons=data.buttons;
       float ax=((float)data.leftStick.x-128.0f)/127.0f;
       float ay=((float)data.leftStick.y-128.0f)/127.0f;
       // Stronger camera displacement: the previous movement was hardly visible.
       if(fabsf(ax)>0.17f)cx+=ax*0.38f;
       if(fabsf(ay)>0.17f)cy+=ay*0.38f;
     }
   }
   if(buttons&ORBIS_PAD_BUTTON_LEFT)cx-=0.38f;
   if(buttons&ORBIS_PAD_BUTTON_RIGHT)cx+=0.38f;
   if(buttons&ORBIS_PAD_BUTTON_UP)cy-=0.38f;
   if(buttons&ORBIS_PAD_BUTTON_DOWN)cy+=0.38f;
   bool walking=(fabsf(cx-oldCx)+fabsf(cy-oldCy))>0.012f;
   if(walking)face=(fabsf(cy-oldCy)>fabsf(cx-oldCx))?(cy>oldCy?2:0):(cx>oldCx?1:3);
   // Camera eases toward the player, visibly showing movement.
   camx+=(cx-camx)*0.115f;
   camy+=(cy-camy)*0.115f;
   int shiftX=(int)((cx-camx)*13.0f);
   int shiftY=(int)((cy-camy)*11.0f);
   if((buttons&ORBIS_PAD_BUTTON_CROSS) && !(oldButtons&ORBIS_PAD_BUTTON_CROSS))pulse=40;
   oldButtons=buttons;
   if(pulse>0)pulse--;
   rect(scene,0,0,SW,SH,color(4,9,17));
   draw_world(scene,camx,camy,frame,pulse,walking,face,shiftX,shiftY);
   // Minimal overlay HUD; world now renders across every edge of the screen.
   rect(scene,0,0,SW,52,color(4,19,25));
   text(scene,26,11,"ASCII REALM  -  PS4 TEST 03  /  FULLSCREEN",color(121,231,191),3);
   rect(scene,0,1034,SW,46,color(4,19,25));
   text(scene,25,1046,"LEFT STICK / D-PAD: WALK       X: LOCAL MAGIC PULSE",color(153,207,186),2);
   text(scene,1510,1046,walking ? "WALKING" : "STANDING",walking ? color(248,219,113) : color(113,190,175),2);
   if(!connected)text(scene,1700,15,"NO PAD",color(245,104,101),2);
   scene->SubmitFlip(frame);
   scene->FrameWait(frame);
   scene->FrameBufferSwap();
   frame++;
 }
 return 0;
}