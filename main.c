#include <windows.h>
#include <stdio.h>
#include <commctrl.h>
#include <gl/gl.h>

int w=800,h=600;
WNDPROC origTestProc,editProc,openglProc;
WNDPROC origExecMove;

HWND hPointVel,hPointVelBG,hPointVelLabel;
HWND hHelpLabel;

//HWND hAngleLabel,hAngleEdit,hAngleBkgnd;

HWND hwnd;
HWND opengl;
HWND hTest;

HWND hButtonOpen;
HWND hButtonStartServer,hStatus;
HWND hButtonErase;
HWND hButtonRunTrajectory;

//Конструкционные параметры (отступы креплений швп, и их рычаги)
HWND hA1OffsetLabel,hA2OffsetLabel;
HWND hA1LeverLabel,hA2LeverLabel;
HWND hA1OffsetInput,hA2OffsetInput;
HWND hA1LeverInput,hA2LeverInput;

HWND hLinetoButton,hArctoButton;
HWND hXLinLabel,hYLinLabel,hCLinLabel,hLinVelLabel;
HWND hXCircEndLabel,hYCircEndLabel,hCCircEndLabel,hXCircAuxLabel,hYCircAuxLabel,hCCircAuxLabel,hCircVelLabel;
HWND hXLin,hYLin,hCLin,hLinVel;
HWND hXCircEnd,hYCircEnd,hCCircEnd,hXCircAux,hYCircAux,hCCircAux,hCircVel;
HWND hPosLabel;
HWND hXactual,hYactual,hZactual,hVelactual;
HWND hA1,hA2,hA3,hA4,hA5;
HWND hAxesLabel;

HWND hResetErrorButton;

HWND hXPlusButton,hXMinusButton,hYPlusButton,hYMinusButton;
HWND hIncLabel,hIncValue;

SOCKET s;
struct sockaddr rsa;
int ralen=16;
#include "draw.c"
PIXELFORMATDESCRIPTOR pfd = { 0x28,   // size of this pfd
	1,                     // version number
	PFD_DRAW_TO_WINDOW |   // support window
	PFD_SUPPORT_OPENGL |
        PFD_DOUBLEBUFFER,   // support OpenGL,      // double buffered
	0,         // RGBA type
	24,                    // 24-bit color depth
	0, 0, 0, 0, 0, 0,      // color bits ignored
	0,                     // no alpha buffer
	0,                     // shift bit ignored
        0,                     // no accumulation buffer
	0, 0, 0, 0,            // accum bits ignored
	32,                    // 32-bit z-buffer
	0,                     // no stencil buffer
	0,                     // no auxiliary buffer
	0,                     // main layer
	0,                     // reserved
	0, 0, 0                // layer masks ignored
};

HDC hDC;
HGLRC hRC;
int iPixelFormat;

 LVITEM li;
 //LPSTR_TEXTCALLBACK;

float *pointsIn,*pointsOut;
int pointNum=0;

#include "windowproc.c"

DWORD WINAPI thread(void*param){

  WSADATA wd;
  WSAStartup(MAKEWORD(2,2)/*0x202*/,&wd);
  s=socket(2,2,17);
  struct sockaddr sa;
  char *buf;
  char lbuf[256];
  buf=malloc(1501);
  buf[1500]=0;
  sa.sa_family=2;
  sa.sa_data[0]=40001/256;
  sa.sa_data[1]=40001%256;
  sa.sa_data[2]=0;
  sa.sa_data[3]=0;
  sa.sa_data[4]=0;
  sa.sa_data[5]=0;
  int len=1;

  setsockopt(s,SOL_SOCKET,SO_REUSEADDR,(const char*)&len,1);
  if(bind(s,&sa,16)){
    printf("bind failed\n");
    return 1;
  }
  SetWindowText(hStatus,"Awating for coordinates");
  do{
    memset(buf,0,1501);
    len=recvfrom(s,buf,1500,0,&rsa,&ralen);
    if(!len) break;

    L1=*(float*)(buf+8);
    L2=*(float*)(buf+12);
    Axis3=*(float*)(buf+16);
    velReceived=*(float*)(buf+20);
    status=*(unsigned int*)(buf+24);
    sernumback=*(unsigned int*)(buf+28);
    sprintf(lbuf,"%08X\n",status);
    SetWindowText(hStatus,lbuf);
    doCalc();
    traj[curpt][0]=x;
    traj[curpt][1]=y;
    curpt++;
    if(curpt==POINTS)
     curpt=0;
  }while(len>0);
  printf("ops\n");
  return 0;
}

DWORD WINAPI threadSend(void*param){
 char lbuf[1501];
 while(1){
/*
  if(!(status&192)||(status&3))
   continue;
*/
  if(currentPoint<0)
   continue;
  printf("[%i] %i\n",currentPoint,trajectory[currentPoint].type);
  memset(lbuf,0,32);
  if(trajectory[currentPoint].type==0)
   lbuf[0]=0;
  else if(trajectory[currentPoint].type==1)
   lbuf[0]=1;
  else
   continue;
  if(sernumback==sernum)
   sernum++;
  else
   continue;
  memcpy(lbuf+4,&trajectory[currentPoint].x,4);
  memcpy(lbuf+8,&trajectory[currentPoint].y,4);
  memcpy(lbuf+12,&trajectory[currentPoint].c,4);
  memcpy(lbuf+16,&trajectory[currentPoint].vel,4);
  memcpy(lbuf+20,&trajectory[currentPoint].xa,4);
  memcpy(lbuf+24,&trajectory[currentPoint].ya,4);
  memcpy(lbuf+28,&trajectory[currentPoint].ca,4);
  memcpy(lbuf+32,&sernum,4);

  if(sendto(s,lbuf,36,0,&rsa,16)==-1)
   printf("send failed\n");

  printf("current point:%i(%i)\n",currentPoint,trajectoryLength);

  currentPoint++;
  if(currentPoint==trajectoryLength){
   EnableWindow(hButtonRunTrajectory,1);
   currentPoint=-1;
  }
}
}

//int APIENTRY wWinMain(HINSTANCE h1,HINSTANCE h2,LPWSTR fr,int bh){
  int main(){
  MSG msg;
  char buf[256];
  WNDCLASSEXA wcex;
  li.pszText=malloc(257);
  li.mask=LVIF_TEXT|LVIF_STATE|LVIF_COLUMNS;
  li.stateMask=0;
  li.iSubItem=0;
  li.state=0;

  li.iItem=0;
  li.cColumns=7;

  wcex.cbSize = sizeof(WNDCLASSEX);
  wcex.style          = CS_HREDRAW | CS_VREDRAW;
  wcex.lpfnWndProc    = WinProc;
  wcex.cbClsExtra     = 0;
  wcex.cbWndExtra     = 0;
  wcex.hInstance      = 0;
  wcex.hIcon          = 0;
  wcex.hCursor        = 0;
  wcex.hbrBackground  = 0;
  wcex.lpszMenuName   = 0;
  wcex.lpszClassName  = "HELLO";
  wcex.hIconSm        =0;
  int ap;

printf("Codepage:%u\n",GetACP());
  if(!RegisterClassExA(&wcex)){
    sprintf(buf,"RegisterClassEx failed with error %08X\n",GetLastError());
    MessageBox(0,buf,"Error",0);
    return 0;
  }

  hwnd = CreateWindow("STATIC", "tank welding",WS_VISIBLE|SS_GRAYRECT, 0, 0, 1500, 900, 0, 0,0, 0);
  if(!hwnd){ 
    sprintf(buf,"error:%08X\n",GetLastError());
    MessageBox(0,buf,"nope",0);
    return 0;
  }
  SetWindowLongPtr(hwnd,GWLP_WNDPROC,(LONG_PTR)&WinProc2);

  opengl=CreateWindowA("HELLO","",WS_CHILD|WS_VISIBLE,10,10,w,h,hwnd,0,0,0);
  openglProc=(WNDPROC)SetWindowLongPtr(opengl,GWLP_WNDPROC,(LONG_PTR)&WinProcOpenGL);
  hButtonOpen = CreateWindowA("BUTTON","Open points' file",WS_CHILD|WS_VISIBLE,820,530,140,26,hwnd,0,0,0);
  hButtonRunTrajectory=CreateWindowA("BUTTON","Run trajectory",WS_CHILD|WS_VISIBLE,820,560,140,26,hwnd,0,0,0);

  hStatus = CreateWindowA("STATIC","Waiting for net",WS_CHILD|WS_VISIBLE,1000,530,220,26,hwnd,0,0,0);
 INITCOMMONCONTROLSEX icce;
 icce.dwSize=sizeof(icce);
 icce.dwICC=ICC_LISTVIEW_CLASSES;
 if(!InitCommonControlsEx(&icce)){
  printf("InitCommonControlsEx failed\n");
  return GetLastError();
 }
 HWND hLabel1=CreateWindow("STATIC","Trajectory",WS_CHILD|WS_VISIBLE,820,5,480,30,hwnd,0,0,0);
  
// SetFocus(hTest);
 hPointVelLabel=CreateWindow("STATIC","G-code:",WS_CHILD|WS_VISIBLE,1120,15,70,26,hwnd,0,0,0);

// hPointVelBG=CreateWindow("STATIC",0,WS_CHILD|WS_VISIBLE,1188,13, 104,504,hwnd,0,0,0);
 hPointVel=CreateWindow("EDIT",0,WS_CHILD|WS_VISIBLE|ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL/*|ES_NUMBER*/,1190,15, 100,500,hwnd,0,0,0);
// editProc=(WNDPROC)SetWindowLongPtr(hPointVel,GWLP_WNDPROC,(LONG_PTR)&WinProcEdit);
 hHelpLabel=CreateWindow("STATIC","Control:\r\nAxis 1 +/- : Q/A\r\nAxis 2 +/- : W/S\r\nAxis 3 +/- : E/D\r\nNumpad \"+\" - zoom in\r\nNumpad \"-\" - zoom out\r\nRight mouse button - rotate\r\nNumpad \"7\" - Top view\r\nNumpad \".\" - reset view",WS_CHILD|WS_VISIBLE,600,650,220,130,hwnd,0,0,0);

// SendMessage(hPointVel,WM_SETTEXT,0,(LPARAM)lll);
/*
 hA1OffsetLabel=CreateWindow("STATIC","Axis 1 BS pivot offset, mm:",WS_CHILD|WS_VISIBLE,340,635,210,40,hwnd,0,0,0);
 hA2OffsetLabel=CreateWindow("STATIC","Axis 2 BS pivot offset, mm:",WS_CHILD|WS_VISIBLE,340,675,210,40,hwnd,0,0,0);
 hA1LeverLabel=CreateWindow("STATIC","Axis 1 BS lever length, mm:",WS_CHILD|WS_VISIBLE,340,715,210,25,hwnd,0,0,0);
 hA2LeverLabel=CreateWindow("STATIC","Axis 2 BS lever length, mm:",WS_CHILD|WS_VISIBLE,340,740,210,25,hwnd,0,0,0);

 hA1OffsetInput=CreateWindow("EDIT","400",WS_CHILD|WS_VISIBLE|WS_BORDER,552,650,50,25,hwnd,0,0,0);
 hA2OffsetInput=CreateWindow("EDIT","400",WS_CHILD|WS_VISIBLE|WS_BORDER,552,690,50,25,hwnd,0,0,0);
 hA1LeverInput=CreateWindow("EDIT","400",WS_CHILD|WS_VISIBLE|WS_BORDER,552,715,50,25,hwnd,0,0,0);
 hA2LeverInput=CreateWindow("EDIT","400",WS_CHILD|WS_VISIBLE|WS_BORDER,552,740,50,25,hwnd,0,0,0);
*/
 hButtonErase=CreateWindow("BUTTON","Erase",WS_CHILD|WS_VISIBLE,670,625,120,25,hwnd,0,0,0);

 hLinetoButton=CreateWindow("BUTTON","Move to:",WS_CHILD|WS_VISIBLE,340,620,90,25,hwnd,0,0,0);
 hArctoButton=CreateWindow("BUTTON","Arc to:",WS_CHILD|WS_VISIBLE,430,620,90,25,hwnd,0,0,0);

 hXLinLabel=CreateWindow("STATIC","X:",WS_CHILD|WS_VISIBLE,340,645,35,25,hwnd,0,0,0);
 hYLinLabel=CreateWindow("STATIC","Y:",WS_CHILD|WS_VISIBLE,340,670,35,25,hwnd,0,0,0);
 hCLinLabel=CreateWindow("STATIC","C:",WS_CHILD|WS_VISIBLE,340,695,35,25,hwnd,0,0,0);
 hLinVelLabel=CreateWindow("STATIC","Vel:",WS_CHILD|WS_VISIBLE,340,720,35,25,hwnd,0,0,0);

 
 hXCircEndLabel=CreateWindow("STATIC","end X:",WS_CHILD|WS_VISIBLE,430,645,45,25,hwnd,0,0,0);
 hYCircEndLabel=CreateWindow("STATIC","end Y:",WS_CHILD|WS_VISIBLE,430,670,45,25,hwnd,0,0,0);
 hCCircEndLabel=CreateWindow("STATIC","end C:",WS_CHILD|WS_VISIBLE,430,695,45,25,hwnd,0,0,0);
 hXCircAuxLabel=CreateWindow("STATIC","aux X:",WS_CHILD|WS_VISIBLE,430,720,45,25,hwnd,0,0,0);
 hYCircAuxLabel=CreateWindow("STATIC","aux Y:",WS_CHILD|WS_VISIBLE,430,745,45,25,hwnd,0,0,0);
 hCCircAuxLabel=CreateWindow("STATIC","aux C:",WS_CHILD|WS_VISIBLE,430,770,45,25,hwnd,0,0,0);
 hCircVelLabel=CreateWindow("STATIC","Vel:",WS_CHILD|WS_VISIBLE,430,795,45,25,hwnd,0,0,0);

 hXLin=CreateWindow("EDIT","0",WS_CHILD|WS_VISIBLE|WS_BORDER,375,645,50,25,hwnd,0,0,0);
 hYLin=CreateWindow("EDIT","0",WS_CHILD|WS_VISIBLE|WS_BORDER,375,670,50,25,hwnd,0,0,0);
 hCLin=CreateWindow("EDIT","0",WS_CHILD|WS_VISIBLE|WS_BORDER,375,695,50,25,hwnd,0,0,0);
 hLinVel=CreateWindow("EDIT","0",WS_CHILD|WS_VISIBLE|WS_BORDER,375,720,50,25,hwnd,0,0,0);

 hXCircEnd=CreateWindow("EDIT","0",WS_CHILD|WS_VISIBLE|WS_BORDER,475,645,50,25,hwnd,0,0,0);
 hYCircEnd=CreateWindow("EDIT","0",WS_CHILD|WS_VISIBLE|WS_BORDER,475,670,50,25,hwnd,0,0,0);
 hCCircEnd=CreateWindow("EDIT","0",WS_CHILD|WS_VISIBLE|WS_BORDER,475,695,50,25,hwnd,0,0,0);
 hXCircAux=CreateWindow("EDIT","0",WS_CHILD|WS_VISIBLE|WS_BORDER,475,720,50,25,hwnd,0,0,0);
 hYCircAux=CreateWindow("EDIT","0",WS_CHILD|WS_VISIBLE|WS_BORDER,475,745,50,25,hwnd,0,0,0);
 hCCircAux=CreateWindow("EDIT","0",WS_CHILD|WS_VISIBLE|WS_BORDER,475,770,50,25,hwnd,0,0,0);
 hCircVel=CreateWindow("EDIT","0",WS_CHILD|WS_VISIBLE|WS_BORDER,475,795,50,25,hwnd,0,0,0);

 hPosLabel=CreateWindow("STATIC","Actual position",WS_CHILD|WS_VISIBLE,10,620,100,25,hwnd,0,0,0);
 hXactual=CreateWindow("STATIC","(0)",WS_CHILD|WS_VISIBLE,15,645,95,25,hwnd,0,0,0);
 hYactual=CreateWindow("STATIC","(0)",WS_CHILD|WS_VISIBLE,15,670,95,25,hwnd,0,0,0);
 hZactual=CreateWindow("STATIC","(0)",WS_CHILD|WS_VISIBLE,15,695,95,25,hwnd,0,0,0);
 hVelactual=CreateWindow("STATIC","(0)",WS_CHILD|WS_VISIBLE,15,720,95,25,hwnd,0,0,0);

 hAxesLabel=CreateWindow("STATIC","Axis (deg)/ Ball Screw (mm)",WS_CHILD|WS_VISIBLE,120,620,200,25,hwnd,0,0,0);
 hA1=CreateWindow("STATIC","0 / 0",WS_CHILD|WS_VISIBLE,120,645,140,25,hwnd,0,0,0);
 hA2=CreateWindow("STATIC","0 / 0",WS_CHILD|WS_VISIBLE,120,670,140,25,hwnd,0,0,0);
 hA3=CreateWindow("STATIC","0",WS_CHILD|WS_VISIBLE,120,695,140,25,hwnd,0,0,0);

 hIncLabel=CreateWindow("STATIC","Increment Step(mm):",WS_CHILD|WS_VISIBLE,120,720,140,25,hwnd,0,0,0);
 hIncValue=CreateWindow("EDIT","10",WS_CHILD|WS_VISIBLE|WS_BORDER,260,720,50,25,hwnd,0,0,0);
 hXPlusButton=CreateWindow("BUTTON","X+",WS_CHILD|WS_VISIBLE,120,745,40,25,hwnd,0,0,0);
 hXMinusButton=CreateWindow("BUTTON","X-",WS_CHILD|WS_VISIBLE,160,745,40,25,hwnd,0,0,0);
 hYPlusButton=CreateWindow("BUTTON","Y+",WS_CHILD|WS_VISIBLE,200,745,40,25,hwnd,0,0,0);
 hYMinusButton=CreateWindow("BUTTON","Y-",WS_CHILD|WS_VISIBLE,240,745,40,25,hwnd,0,0,0);


 hResetErrorButton=CreateWindow("BUTTON","Reset Error",WS_CHILD|WS_VISIBLE,550,620,90,25,hwnd,0,0,0);

 char lbuf[256];
 sprintf(lbuf,"%.1f",A1offset);
 SetWindowText(hA1OffsetInput,lbuf);
 sprintf(lbuf,"%.1f",A1lever);
 SetWindowText(hA1LeverInput,lbuf);
 sprintf(lbuf,"%.1f",A2offset);
 SetWindowText(hA2OffsetInput,lbuf);
 sprintf(lbuf,"%.1f",A2lever);
 SetWindowText(hA2LeverInput,lbuf);

 hTest= CreateWindowA(WC_LISTVIEW,"",WS_CHILD|WS_VISIBLE|LVS_REPORT,810,30,320,500,hwnd,0,0,0);
 origTestProc=(WNDPROC)SetWindowLongPtr(hTest,GWLP_WNDPROC,(LONG_PTR)&WinProcLV);

 LVCOLUMN col;
 col.mask=LVCF_TEXT|LVCF_WIDTH|LVCF_FMT|LVCF_SUBITEM;
 col.pszText=malloc(257);

 sprintf(col.pszText,"Point");
 col.cx=80;
 SendMessage(hTest,LVM_INSERTCOLUMN,0,(LPARAM)&col);

 sprintf(col.pszText,"Current, kA");
 col.cx=120;
 SendMessage(hTest,LVM_INSERTCOLUMN,1,(LPARAM)&col);

 sprintf(col.pszText,"Pressure, MPa");
 col.cx=120;
 SendMessage(hTest,LVM_INSERTCOLUMN,2,(LPARAM)&col);

 li.mask=LVIF_TEXT;

/*
  ShowWindow(hwnd,1);
*/

  if(!CreateThread(0,0,thread,0,0,0)){
    printf("create thread failed\n");
    return -1;
  }
  if(!CreateThread(0,0,threadSend,0,0,0)){
    printf("create thread failed\n");
    return -1;
  }
  UpdateWindow(hwnd);

  while(GetMessage(&msg,0,0,0)){
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  return 0;
}

