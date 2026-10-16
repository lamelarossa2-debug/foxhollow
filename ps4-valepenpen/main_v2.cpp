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

static void texturedWall(Scene2D* scene,const Bitmap& material,bool right){
 // Solid joined walls: two correct 2:1 isometric edges, not repeated loose panels.
 // Each material is sampled from the genuine previously-approved wall PNG.
 if(!material.valid)return;
 for(int x=right?640:256;x<=(right?1024:640);x+=2){
  const int distance=abs(x-640);
  const int bottom=205+distance/2;
  const int top=bottom-190;
  const float side=(float)distance/384.f;
  for(int y=top;y<bottom;y+=2){
   const float t=(float)(y-top)/190.f;
   unsigned col=0xDAC2AB;
   if(t<0.035f)col=0xF7E3CF; // capping
   else if(t<0.082f)col=0xCEB59E;
   else if(t<0.82f){
    const int xx=(int)(material.w*(0.22f+0.55f*side));
    const int yy=(int)(material.h*(0.24f+0.55f*(t-.08f)/.74f));
    const int sx=xx>=material.w?material.w-1:xx;
    const int sy=yy>=material.h?material.h-1:yy;
    const int index=material.px[sy*material.w+sx];
    col=index?material.palette[index]:0xDBBE9F;
    if(right){
     unsigned red=(col>>16)&255, green=(col>>8)&255,blue=col&255;
     col=((red*96/100)<<16)|((green*96/100)<<8)|(blue*96/100);
    }
   }
   else if(t<0.91f)col=0x8D5C38;
   else if(t<0.935f)col=0xC28A57;
   else col=0x694023;
   box(scene,x,y,2,2,col);
  }
 }
}
static void floorAndWalls(Scene2D* scene,bool unused) {
 (void)unused;
 // Matching 1280x720 scene: true 3x3 parquet, continuous 2:1 iso floor.
 for(int y=0;y<H;y+=2){
  float u=(float)y/(float)H;
  unsigned c=((unsigned)(31+8*u)<<16)|((unsigned)(27+9*u)<<8)|(unsigned)(30+8*u);
  box(scene,0,y,W,2,c);
 }
 texturedWall(scene,wallLeft,false);
 texturedWall(scene,wallRight,true);
 for(int sum=0;sum<=4;sum++)for(int a=0;a<3;a++){
  int b=sum-a;if(b<0||b>2)continue;
  drawPic(scene,floorPic,640+(a-b)*128-128,205+(a+b)*64,256,128);
 }
}
struct SpriteBounds { int l,t,r,b,footx; };
static SpriteBounds bounds[8][8];
static void prepareCharacterBounds(){
 for(int d=0;d<8;d++)for(int f=0;f<8;f++){
  const Bitmap& bm=walkPic[d][f];
  SpriteBounds m={bm.w,bm.h,-1,-1,bm.w/2};
  if(bm.valid)for(int y=0;y<bm.h;y++)for(int x=0;x<bm.w;x++){
   if(bm.px[y*bm.w+x]){if(x<m.l)m.l=x;if(x>m.r)m.r=x;if(y<m.t)m.t=y;if(y>m.b)m.b=y;}
  }
  if(m.r>=m.l){
   int left=bm.w,right=0;
   for(int y=m.b-7;y<=m.b;y++)if(y>=0)for(int x=m.l;x<=m.r;x++){
    if(bm.px[y*bm.w+x]){if(x<left)left=x;if(x>right)right=x;}
   }
   if(right>=left)m.footx=(right+left)/2;
  }
  bounds[d][f]=m;
 }
}
static void drawCharacter(Scene2D* scene,int d,int f,int footX,int footY){
 const Bitmap& bm=walkPic[d][f];
 const SpriteBounds& m=bounds[d][f];
 if(!bm.valid||m.r<m.l)return;
 const int ch=m.b-m.t+1, cw=m.r-m.l+1;
 // Exactly 164 screen pixels from top to feet for EVERY direction and frame.
 const int outH=164, outW=(cw*outH+ch/2)/ch;
 if(outW<=0)return;
 const int destX=footX-((m.footx-m.l)*outW+cw/2)/cw;
 const int destY=footY-outH;
 for(int yy=0;yy<outH;yy++){
  const int sy=m.t+(yy*ch/outH);
  if(destY+yy<0||destY+yy>=H)continue;
  const uint8_t* row=bm.px+sy*bm.w;
  int xx=0;
  while(xx<outW){
   const int sx=m.l+xx*cw/outW;
   const uint8_t index=row[sx];
   int last=xx+1;
   while(last<outW&&row[m.l+last*cw/outW]==index)last++;
   if(index)box(scene,destX+xx,destY+yy,last-xx,1,bm.palette[index]);
   xx=last;
  }
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
 box(s,25,24,410,55,0x31242C);box(s,25,24,410,2,0xE89BBA);
 txt(s,43,40,"VALEPENPEN - DEMO 02",3,0xFFE7E5);
 box(s,28,655,840,40,0x31242C);
 txt(s,40,669,"STICK / D-PAD: CAMMINA",2,0xFFE0E9);
 
 if(!pad)txt(s,1080,43,"NO PAD",2,0xFF8383);
}
int main(){
 setvbuf(stdout,NULL,_IONBF,0);
 floorPic=decode(&asset_parquet);
 wallLeft=decode(&asset_wall_a);
 wallRight=decode(&asset_wall_b);
 for(int d=0;d<8;d++)for(int f=0;f<8;f++)walkPic[d][f]=decode(walk_frames[d][f]);
 prepareCharacterBounds();
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
    if(wx>2.8f)wx=2.8f;if(wy>2.8f)wy=2.8f;
    int sector=((int)floorf((atan2f(ay,ax)+0.392699f)*1.2732395f)+8)%8;
    direction=(sector+6)%8;
    walkPhase+=0.18f;while(walkPhase>=8.f)walkPhase-=8.f;
    frame=(int)walkPhase;
  }else{walkPhase=0.f;frame=0;}
  floorAndWalls(scene,wallTest);
  const int sx=640+(int)((wx-wy)*128.f);
  const int sy=205+(int)((wx+wy+1.f)*64.f);
  diamondShadow(scene,sx,sy+1,42,9);
  drawCharacter(scene,direction,frame,sx,sy);
  hud(scene,pad>=0,moving,direction,frame);
  scene->SubmitFlip(flip);
  scene->FrameWait(flip);
  scene->FrameBufferSwap();
  ++flip;
 }
 return 0;
}