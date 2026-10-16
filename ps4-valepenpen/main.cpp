// ValePenPen: Pipi alla riscossa / PS4 graphics & walking smoke test 01.
// All 19 embedded images are downscaled from existing ValePenPen original files.
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <sstream>
#include <orbis/Pad.h>
#include <orbis/UserService.h>
#include "../../_common/graphics.h"
#include "../../_common/log.h"
#include "art32.h"
std::stringstream debugLogStream;
static const int W=1920, H=1080;
static unsigned char* tex[19]={};
static int un64(char ch){
 if(ch>='A'&&ch<='Z')return ch-'A';
 if(ch>='a'&&ch<='z')return ch-'a'+26;
 if(ch>='0'&&ch<='9')return ch-'0'+52;
 if(ch=='-')return 62;if(ch=='_')return 63;return 0;
}
static Color rgb(unsigned c){Color x={(unsigned char)(c>>16),(unsigned char)(c>>8),(unsigned char)c};return x;}
static void initArt(){
 for(int i=0;i<19;i++){
  int size=(int)asset[i].w*asset[i].h;
  tex[i]=(unsigned char*)malloc(size);
  if(!tex[i])continue;
  const char* p=asset[i].data;int k=0;
  while(p[0]&&p[1]&&k<size){int ci=un64(p[0]),count=un64(p[1])+1;p+=2;
   while(count--&&k<size)tex[i][k++]=(unsigned char)ci;
  }
  while(k<size)tex[i][k++]=0;
 }
}
static void pic(Scene2D* sc,int id,int left,int top,int zoom){
 if(id<0||id>=19||!tex[id])return;
 const PixArt& p=asset[id];const unsigned char* pixels=tex[id];
 for(int y=0;y<p.h;y++){int sy=top+y*zoom;if(sy<0||sy+zoom>H)continue;
  for(int x=0;x<p.w;x++){int sx=left+x*zoom;if(sx<0||sx+zoom>W)continue;
   int ci=pixels[y*p.w+x];if(ci>0&&ci<=16)sc->DrawRectangle(sx,sy,zoom,zoom,rgb(p.pal[ci]));
  }
 }
}
static void groundShadow(Scene2D* sc,int cx,int cy){
 for(int y=-10;y<=10;y+=2){float t=y/11.f;int rx=(int)(52.f*sqrtf(fmaxf(0.f,1.f-t*t)));
  sc->DrawRectangle(cx-rx,cy+y,2*rx+1,2,rgb(0x634733));
 }
}
int main(){
 setvbuf(stdout,nullptr,_IONBF,0);
 initArt();
 Scene2D* sc=new Scene2D(W,H,4);
 if(!sc->Init(0xC000000,2))for(;;){}
 int pad=-1,user=-1;
 if(scePadInit()==0){OrbisUserServiceInitializeParams p={};p.priority=ORBIS_KERNEL_PRIO_FIFO_LOWEST;
  sceUserServiceInitialize(&p);
  if(sceUserServiceGetInitialUser(&user)==0)pad=scePadOpen(user,0,0,nullptr);
 }
 float u=4.f,v=4.f,walk=0.f;int facing=0,frame=0;
 for(;;){
  float dx=0,dy=0;uint32_t keys=0;
  if(pad>=0){OrbisPadData p={};if(scePadReadState(pad,&p)==0){
   dx=(p.leftStick.x-128.f)/127.f;dy=(p.leftStick.y-128.f)/127.f;keys=p.buttons;
  }}
  if(keys&ORBIS_PAD_BUTTON_LEFT)dx=-1;
  if(keys&ORBIS_PAD_BUTTON_RIGHT)dx=1;
  if(keys&ORBIS_PAD_BUTTON_UP)dy=-1;
  if(keys&ORBIS_PAD_BUTTON_DOWN)dy=1;
  if(fabsf(dx)<.16f)dx=0;if(fabsf(dy)<.16f)dy=0;
  bool moving=dx!=0||dy!=0;
  if(moving){
   u+=(dy+dx)*.046f;v+=(dy-dx)*.046f;
   if(u<.3f)u=.3f;if(u>7.6f)u=7.6f;
   if(v<.3f)v=.3f;if(v>7.6f)v=7.6f;
   float a=atan2f(dy,dx);if(a<0)a+=6.2831853f;
   const int oct=((int)floorf(a*4.f/3.14159265f+.5f))&7;
   const int dirs[8]={6,7,0,1,2,3,4,5};facing=dirs[oct];
   walk+=.20f;
  }else walk=0;
  sc->DrawRectangle(0,0,W,H,rgb(0x17202A));
  sc->DrawRectangle(96,83,1728,870,rgb(0x32271F));
  // Back walls, using both created project wall sprites.
  for(int i=0;i<8;i++){
   pic(sc,1,960+i*92-85,147+i*45-125,2);
   pic(sc,2,960-i*92-85,147+i*45-125,2);
  }
  // Iso parquet diamond: eight by eight authentic ValePenPen parquet thumbnails.
  for(int i=0;i<8;i++)for(int j=0;j<8;j++)
   pic(sc,0,960+(i-j)*92-96,145+(i+j)*45,2);
  int px=(int)(960+(u-v)*92),py=(int)(205+(u+v)*45);
  groundShadow(sc,px,py);
  int spriteIndex=3+facing*2+(moving?((int)walk&1):0);
  pic(sc,spriteIndex,px-100,py-192+(moving?((int)walk&1)*-3:0),5);
  sc->SubmitFlip(frame);sc->FrameWait(frame);sc->FrameBufferSwap();frame++;
 }
 return 0;
}