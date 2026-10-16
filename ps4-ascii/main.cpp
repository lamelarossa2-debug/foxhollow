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
static const int COLS = 104, ROWS = 47, CW=16, CH=18;
static const int LEFT = 128, TOP = 120;
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
static void draw_world(Scene2D *s,float cx,float cy,int frame,int pulse) {
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
   float spot=1.0f-sqrtf(dx*dx*0.00031f+dy*dy*0.00108f);
   if(spot<0.18f)spot=0.18f;
   float boost=pulse>0?0.18f*(1.0f-fminf(1.0f,sqrtf(dx*dx+dy*dy)/(pulse*2.2f+1.0f))):0.0f;
   spot=fminf(1.0f,spot+boost);
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
   // Edge brightness and shading with atmospheric depth.
   ink=mix(color(3,12,21),ink,spot);
   drawChar(s,px+2,py+2,ch,ink,2);
  }
 }
 // Actual player, fixed at center, with visible silhouette.
 int X=LEFT+(COLS/2)*CW-18, Y=TOP+(ROWS/2)*CH-35;
 RGB shade=color(24,34,41),gold=color(255,208,112),robe=color(206,169,124);
 // Slight glow around player, without sprite assets.
 text(s,X-14,Y-20,"   .  ",color(50,86,79),2);
 text(s,X,Y,"  ^  ",gold,3);
 text(s,X,Y+25," (@) ",gold,3);
 text(s,X,Y+50," /#\\ ",robe,3);
 text(s,X,Y+75," / \\ ",shade,3);
 if(pulse>0 && pulse%8<5)text(s,X-22,Y+38,"*     *",color(103,224,203),3);
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
 float cx=0,cy=0;int pulse=0,frame=0;uint32_t oldButtons=0;
 for(;;){
   uint32_t buttons=0;
   if(connected){
     OrbisPadData data={};
     if(scePadReadState(pad,&data)==0){
       buttons=data.buttons;
       float ax=((float)data.leftStick.x-128.0f)/127.0f;
       float ay=((float)data.leftStick.y-128.0f)/127.0f;
       if(fabsf(ax)>0.17f)cx+=ax*0.10f;
       if(fabsf(ay)>0.17f)cy+=ay*0.10f;
     }
   }
   if(buttons&ORBIS_PAD_BUTTON_LEFT)cx-=0.11f;
   if(buttons&ORBIS_PAD_BUTTON_RIGHT)cx+=0.11f;
   if(buttons&ORBIS_PAD_BUTTON_UP)cy-=0.11f;
   if(buttons&ORBIS_PAD_BUTTON_DOWN)cy+=0.11f;
   if((buttons&ORBIS_PAD_BUTTON_CROSS) && !(oldButtons&ORBIS_PAD_BUTTON_CROSS))pulse=40;
   oldButtons=buttons;
   if(pulse>0)pulse--;
   rect(scene,0,0,SW,SH,color(4,9,17));
   rect(scene,LEFT-17,TOP-17,COLS*CW+34,ROWS*CH+34,color(17,34,45));
   rect(scene,LEFT-10,TOP-10,COLS*CW+20,ROWS*CH+20,color(2,8,14));
   draw_world(scene,cx,cy,frame,pulse);
   rect(scene,110,52,1695,42,color(4,19,25));
   text(scene,137,62,"ASCII REALM  -  PS4 RENDER TEST 02",color(121,231,191),3);
   rect(scene,110,990,1695,38,color(4,19,25));
   text(scene,135,998,"LEFT STICK / D-PAD: MOVE     X: PULSE     NATIVE ASCII ENGINE",color(153,207,186),2);
   if(!connected)text(scene,1490,64,"NO PAD",color(245,104,101),2);
   scene->SubmitFlip(frame);
   scene->FrameWait(frame);
   scene->FrameBufferSwap();
   frame++;
 }
 return 0;
}