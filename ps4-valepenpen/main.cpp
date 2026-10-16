// VALEPENPEN: PIPI ALLA RISCOSSA / PS4 GRAPHICS + CONTROLLER DEMO 01
// Uses the actual parquet, wall and Valentina artwork packed from the project ZIP.
// First native OpenOrbis CPU renderer smoke test; NOT the final isometric engine.
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sstream>
#include <orbis/Pad.h>
#include <orbis/UserService.h>
#include "../../_common/graphics.h"
#include "../../_common/log.h"
#include "packed_art.h"

std::stringstream debugLogStream;
static const int W=1280, H=720;
static const char ALPHA[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";
struct Bitmap {int w,h; uint8_t* px; const uint32_t* palette; bool valid;};
static Bitmap floorPic, wallLeft, wallRight, walkPic[8][8];
static inline Color rgb(unsigned int n) { Color c = {(uint8_t)(n>>16),(uint8_t)(n>>8),(uint8_t)n};return c; }
static inline void box(Scene2D* s,int x,int y,int w,int h,unsigned int col) {
 if(x<0){w+=x;x=0;}if(y<0){h+=y;y=0;}if(x+w>W)w=W-x;if(y+h>H)h=H-y;
 if(w>0 && h>0)s->DrawRectangle(x,y,w,h,rgb(col));
}
static int symbol(char c) {
 for(int i=0;i<64;i++)if(ALPHA[i]==c)return i;
 return -1;
}
static Bitmap decode(const RlePicture* src) {
 Bitmap a={src->w,src->h,0,src->pal,false};
 const size_t n=(size_t)src->w*src->h;
 a.px=(uint8_t*)malloc(n);
 if(!a.px)return a;
 size_t i=0;
 const char* p=src->rle;
 while(*p && p[1] && i<n) {
  int value=symbol(p[0]), run=symbol(p[1]);p+=2;
  if(value<0||value>40||run<0||(size_t)(run+1)>n-i)break;
  memset(a.px+i,value,(size_t)run+1);i+=(size_t)run+1;
 }
 a.valid=(i==n);
 if(!a.valid)memset(a.px,0,n);
 return a;
}
static void drawPic(Scene2D* s,const Bitmap& b,int x,int y,int width,int height){
 if(!b.valid||width<=0||height<=0)return;
 // Pixel-art-like nearest neighbour resizing: independent of extra texture libs.
 int x0=x<0?0:x, y0=y<0?0:y;
 int x1=x+width>W?W:x+width, y1=y+height>H?H:y+height;
 if(x1<=x0||y1<=y0)return;
 // Horizontal spans of equal palette index rather than individual DrawPixel calls.
 for(int dy=y0;dy<y1;dy++){
  int sy=(int)(((int64_t)(dy-y)*b.h)/height);if(sy>=b.h)sy=b.h-1;
  const uint8_t* row=b.px+(size_t)sy*b.w;
  int dx=x0;
  while(dx<x1){
    int sx=(int)(((int64_t)(dx-x)*b.w)/width);if(sx>=b.w)sx=b.w-1;
    uint8_t index=row[sx];
    int end=dx+1;
    // Group the destination pixels with the same index on this scanline.
    while(end<x1){int nx=(int)(((int64_t)(end-x)*b.w)/width);if(nx>=b.w)nx=b.w-1;if(row[nx]!=index)break;end++;}
    if(index)box(s,dx,dy,end-dx,1,b.palette[index]);
    dx=end;
  }
 }
}
static void diamondShadow(Scene2D* s,int cx,int cy,int rx,int ry){
 for(int j=-ry;j<=ry;j++){
  float f=1.f-((float)j*j)/(ry*ry+0.01f);
  int wid=(int)(rx*sqrtf(f));
  box(s,cx-wid,cy+j,2*wid,1,0x6B5D54);
 }
}
static void floorAndWalls(Scene2D* s,bool softWall){
 box(s,0,0,W,H,0x161E22);
 // Four-by-four floor rhombi. Coordinates are world-space and reused for the player.
 for(int sum=0;sum<=6;sum++)for(int a=0;a<4;a++){
   int b=sum-a;if(b<0||b>3)continue;
   int xx=640+(a-b)*128-128, yy=150+(a+b)*64;
   drawPic(s,floorPic,xx,yy,256,128);
 }
 // Two room wall pieces, preserved from the approved original wall package.
 // They stay behind Valentina in the first engine check.
 drawPic(s,wallLeft,248,34,452,352);
 drawPic(s,wallRight,628,30,396,399);
 if(softWall){ // debugging viewport boundary, toggled by TRIANGLE.
  box(s,10,67,210,7,0xE3C0A4);
 }
}
static const uint8_t FONT[][7]={
 {14,17,17,31,17,17,17},{30,17,17,30,17,17,30},{15,16,16,16,16,16,15},
 {30,17,17,17,17,17,30},{31,16,16,30,16,16,31},{31,16,16,30,16,16,16},
 {15,16,16,19,17,17,15},{17,17,17,31,17,17,17},{31,4,4,4,4,4,31},
 {7,2,2,2,18,18,12},{17,18,20,24,20,18,17},{16,16,16,16,16,16,31},
 {17,27,21,21,17,17,17},{17,25,21,19,17,17,17},{14,17,17,17,17,17,14},
 {30,17,17,30,16,16,16},{14,17,17,17,21,18,13},{30,17,17,30,20,18,17},
 {15,16,16,14,1,1,30},{31,4,4,4,4,4,4},{17,17,17,17,17,17,14},
 {17,17,17,17,17,10,4},{17,17,17,21,21,21,10},{17,17,10,4,10,17,17},
 {17,17,10,4,4,4,4},{31,1,2,4,8,16,31},
};
static void txt(Scene2D* s,int x,int y,const char* t,int scale,unsigned int color){
 while(*t){char c=*t++;uint8_t bits[7]={0};
  if(c>='a'&&c<='z')c-=32;
  if(c>='A'&&c<='Z')memcpy(bits,FONT[c-'A'],7);
  if(c=='-')bits[3]=31;
  if(c==':')bits[1]=12,bits[4]=12;
  if(c=='/')bits[0]=1,bits[1]=2,bits[2]=2,bits[3]=4,bits[4]=8,bits[5]=8,bits[6]=16;
  if(c=='.')bits[6]=12;
  if(c=='0')bits[0]=14,bits[1]=17,bits[2]=17,bits[3]=17,bits[4]=17,bits[5]=17,bits[6]=14;
  if(c=='1')bits[0]=4,bits[1]=12,bits[2]=4,bits[3]=4,bits[4]=4,bits[5]=4,bits[6]=14;
  if(c=='2')bits[0]=14,bits[1]=17,bits[2]=1,bits[3]=2,bits[4]=4,bits[5]=8,bits[6]=31;
  if(c=='4')bits[0]=2,bits[1]=6,bits[2]=10,bits[3]=18,bits[4]=31,bits[5]=2,bits[6]=2;
  if(c=='8')bits[0]=14,bits[1]=17,bits[2]=17,bits[3]=14,bits[4]=17,bits[5]=17,bits[6]=14;
  for(int yy=0;yy<7;yy++)for(int xx=0;xx<5;xx++)if(bits[yy]&(1<<(4-xx)))box(s,x+xx*scale,y+yy*scale,scale,scale,color);
  x+=6*scale;
 }
}
static void hud(Scene2D* s,bool pad,bool moving,int dir,int frame){
 box(s,0,0,W,62,0x241E29);box(s,0,59,W,3,0xE89BBA);
 txt(s,26,15,"VALEPENPEN - PIPI ALLA RISCOSSA",3,0xFFE7E5);
 box(s,0,673,W,47,0x241E29);box(s,0,673,W,3,0xE89BBA);
 txt(s,25,688,"STICK / D-PAD: CAMMINA     TRIANGLE: TEST",2,0xFFE0E9);
 txt(s,928,688,moving?"WALK":"IDLE",2,moving?0xFFD0A0:0xD5BCD5);
 if(!pad)txt(s,1053,22,"NO PAD",2,0xFF8383);
}
int main(){
 setvbuf(stdout,NULL,_IONBF,0);
 floorPic=decode(&asset_parquet);
 wallLeft=decode(&asset_wall_a);
 wallRight=decode(&asset_wall_b);
 for(int d=0;d<8;d++)for(int f=0;f<8;f++)walkPic[d][f]=decode(walk_frames[d][f]);
 Scene2D *scene=new Scene2D(W,H,4);
 if(!scene->Init(0x8000000,2))for(;;){}
 int userId=-1,pad=-1;
 if(scePadInit()==0){
  OrbisUserServiceInitializeParams params={};
  params.priority=ORBIS_KERNEL_PRIO_FIFO_LOWEST;
  sceUserServiceInitialize(&params);
  if(sceUserServiceGetInitialUser(&userId)==0)pad=scePadOpen(userId,0,0,NULL);
 }
 float wx=1.5f,wy=1.5f,walkPhase=0.f;
 int direction=0,frame=0,flip=0;
 uint32_t previousButtons=0;bool wallTest=false;
 for(;;){
  float ax=0.f,ay=0.f;
  uint32_t buttons=0;
  if(pad>=0){OrbisPadData info={};
   if(scePadReadState(pad,&info)==0){
    ax=((float)info.leftStick.x-128.f)/127.f;
    ay=((float)info.leftStick.y-128.f)/127.f;
    buttons=info.buttons;
   }
  }
  if(fabsf(ax)<0.17f)ax=0;
  if(fabsf(ay)<0.17f)ay=0;
  if(buttons&ORBIS_PAD_BUTTON_LEFT)ax=-1;
  if(buttons&ORBIS_PAD_BUTTON_RIGHT)ax=1;
  if(buttons&ORBIS_PAD_BUTTON_UP)ay=-1;
  if(buttons&ORBIS_PAD_BUTTON_DOWN)ay=1;
  bool moving=(fabsf(ax)+fabsf(ay)>0.01f);
  if((buttons&ORBIS_PAD_BUTTON_TRIANGLE)&&!(previousButtons&ORBIS_PAD_BUTTON_TRIANGLE))wallTest=!wallTest;
  previousButtons=buttons;
  if(moving){
    float norm=sqrtf(ax*ax+ay*ay);
    if(norm>1.f){ax/=norm;ay/=norm;}
    float vx=(ax+ay)*0.010f,vy=(ay-ax)*0.010f;
    wx+=vx;wy+=vy;
    if(wx<0.2f)wx=0.2f;if(wy<0.2f)wy=0.2f;
    if(wx>3.8f)wx=3.8f;if(wy>3.8f)wy=3.8f;
    int sector=((int)floorf((atan2f(ay,ax)+0.392699f)*1.2732395f)+8)%8;
    direction=(sector+6)%8;
    walkPhase+=0.18f;while(walkPhase>=8.f)walkPhase-=8.f;
    frame=(int)walkPhase;
  }else{walkPhase=0.f;frame=0;}
  floorAndWalls(scene,wallTest);
  const int sx=640+(int)((wx-wy)*128.f);
  const int sy=150+(int)((wx+wy+1.f)*64.f);
  diamondShadow(scene,sx,sy,38,10);
  int bob=moving?((frame==1||frame==5)?-2:0):0;
  drawPic(scene,walkPic[direction][frame],sx-103,sy-194+bob,206,206);
  hud(scene,pad>=0,moving,direction,frame);
  scene->SubmitFlip(flip);
  scene->FrameWait(flip);
  scene->FrameBufferSwap();
  ++flip;
 }
 return 0;
}