LRESULT CALLBACK WinProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam){
  switch(msg){
    case WM_CREATE:

      hDC=GetDC(hwnd);

      if(DescribePixelFormat(hDC,1,sizeof(PIXELFORMATDESCRIPTOR),&pfd)){
        iPixelFormat = ChoosePixelFormat(hDC, &pfd);
      }else
        printf("failed to describe pixel format\n");

       SetPixelFormat(hDC,iPixelFormat,&pfd);

      hRC = wglCreateContext( hDC );

      wglMakeCurrent(hDC, hRC);

      init_opengl();

    break;
    case WM_PAINT:
      glClearColor(0,0,0,0);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      draw();
      glFinish();
      SwapBuffers(hDC);
    break;
    case WM_RBUTTONDOWN:
      dragstart[0]=LOWORD(lParam)-rot[0];
      dragstart[1]=HIWORD(lParam)-rot[1];
      drag=1;
    break;
    case WM_RBUTTONUP:
      drag=0;
    break;
    case WM_MOUSEMOVE:
      if(drag){
        rot[0]=LOWORD(lParam)-dragstart[0];
        rot[1]=HIWORD(lParam)-dragstart[1];
      }
    break;
    case WM_MOUSEWHEEL:
      if((int)wParam>0)
        zoom/=1.4142;
      else
        zoom*=1.4142;
    break;
    case WM_DESTROY:
      PostQuitMessage(0);
    break;
    case WM_KEYDOWN:
//printf("MAIN WINDOW KEYDOWN\n");
//printf("%04X\n",wParam);
     if(wParam==0x43){ //c
     }
    break;
    default:
      return DefWindowProc(hwnd,msg,wParam,lParam);
  }
  return 0;
}
LRESULT CALLBACK WinProcLV(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam){
 return CallWindowProc(origTestProc,hwnd,msg,wParam,lParam);
}
LRESULT CALLBACK WinProc2(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam){
  char lbuf[256];
  OPENFILENAME ofn;
  TCHAR szFile[260]={0};
  memset(&ofn,0,sizeof(ofn));
  ofn.lStructSize=sizeof(ofn);
  ofn.hwndOwner=hwnd;
  ofn.lpstrFile=szFile;
  ofn.nMaxFile=sizeof(szFile);
  ofn.lpstrFilter="Comma separated list\0*.csv\0Text\0*.TXT\0";
  ofn.nFilterIndex=1;
  ofn.lpstrFileTitle=NULL;
  ofn.nMaxFileTitle=0;
  ofn.lpstrInitialDir=NULL;
  ofn.Flags=OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;


  switch(msg){
/*
  case WM_KEYDOWN:
    break;
*/
    case WM_COMMAND:
      if(lParam==(LPARAM)hButtonErase){
        curpt=0;
      }
      if(lParam==(LPARAM)hLinetoButton){
       float lx,ly,lc,lvel;

       GetWindowText(hXLin,lbuf,255);
       printf("X=%.3f\n",atof(lbuf));
       lx=atof(lbuf);

       GetWindowText(hYLin,lbuf,255);
       printf("Y=%.3f\n",atof(lbuf));
       ly=atof(lbuf);

       GetWindowText(hCLin,lbuf,255);
       printf("C=%.3f\n",atof(lbuf));
       lc=atof(lbuf);

       GetWindowText(hLinVel,lbuf,255);
       printf("velocity=%.3f\n",atof(lbuf));
       lvel=atof(lbuf);

       xe=lx;
       ye=ly;

       float lAxis1=atan(ye/xe)+acos((Arm1*Arm1+xe*xe+ye*ye-Arm2*Arm2)/(2*Arm1*sqrt(xe*xe+ye*ye)));
       lAxis1*=180/pi;
       float lAxis2=acos((Arm1*Arm1+Arm2*Arm2-xe*xe-ye*ye)/(2*Arm1*Arm2));
       lAxis2*=180/pi;
       lAxis2=180-lAxis2;
       lAxis2=-lAxis2;
       a3e=Axis3+Axis1+Axis2-lAxis1-lAxis2;
//Axis3=a3e;
       oldx=x;
       oldy=y;
       float lr=sqrt((lx-oldx)*(lx-oldx)+(ly-oldy)*(ly-oldy));
       dx=lvel/lr*(lx-oldx);
       dy=lvel/lr*(ly-oldy);
    printf("lr=%.3f %f %f\n",lr,dx,dy);

//       task=1;

       char lbuf[32];
       memset(lbuf,0,32);
       memcpy(lbuf+4,&lx,4);
       memcpy(lbuf+8,&ly,4);
       memcpy(lbuf+12,&lc,4);
       memcpy(lbuf+16,&lvel,4);
       lbuf[0]=0;
       printf("%u.%u.%u.%u\n",(unsigned char)rsa.sa_data[2],(unsigned char)rsa.sa_data[3],(unsigned char)rsa.sa_data[4],(unsigned char)rsa.sa_data[5]);
       if(sendto(s,lbuf,32,0,&rsa,16)==-1)
         printf("send failed\n");
      }

      if(lParam==(LPARAM)hArctoButton){
       float xs=x;
       float ys=y;
       float cs=Axis3+Axis1+Axis2;

       GetWindowText(hXCircEnd,lbuf,255);
       printf("Xend=%.3f\n",atof(lbuf));
       float xe=atof(lbuf);

       GetWindowText(hYCircEnd,lbuf,255);
       printf("Yend=%.3f\n",atof(lbuf));
       float ye=atof(lbuf);

       GetWindowText(hCCircEnd,lbuf,255);
       printf("Cend=%.3f\n",atof(lbuf));
       float ce=atof(lbuf);

       GetWindowText(hXCircAux,lbuf,255);
       printf("Xaux=%.3f\n",atof(lbuf));
       float xa=atof(lbuf);

       GetWindowText(hYCircAux,lbuf,255);
       printf("Yaux=%.3f\n",atof(lbuf));
       float ya=atof(lbuf);

       GetWindowText(hCCircAux,lbuf,255);
       printf("Yaux=%.3f\n",atof(lbuf));
       float ca=atof(lbuf);

       GetWindowText(hCircVel,lbuf,255);
       printf("Yaux=%.3f\n",atof(lbuf));
       float lvel=atof(lbuf);

       float lAxis1=atan(ya/xa)+acos((Arm1*Arm1+xa*xa+ya*ya-Arm2*Arm2)/(2*Arm1*sqrt(xa*xa+ya*ya)));
       lAxis1*=180/pi;
       float lAxis2=acos((Arm1*Arm1+Arm2*Arm2-xa*xa-ya*ya)/(2*Arm1*Arm2));
       lAxis2*=180/pi;
       lAxis2=180-lAxis2;
       lAxis2=-lAxis2;
       a3a=Axis3+Axis1+Axis2-lAxis1-lAxis2;

       float lA=(ys-ya)/(xs-xa);
       float lB=(xs*xs-xa*xa+ys*ys-ya*ya)/(2*(xs-xa));
       float lC=(xs-xe)/(ys-ye);
       float lD=(ys*ys-ye*ye+xs*xs-xe*xe)/(2*(ys-ye));

printf("%.1f %.1f %.1f %.1f\n",lA,lB,lC,lD);
       cy=(lD-lB*lC)/(1-lA*lC);
       cx=lB-lA*cy;
       cr=sqrt((xs-cx)*(xs-cx)+(ys-cy)*(ys-cy));
cphi=atan((y-cy)/(x-cx))*180/pi;
       cd=atan((ye-cy)/(xe-cx))*180/pi;
printf("cphi=%.1f cd=%.1f\n",cphi,cd);
       t0=0;
      /*
//вывернутый круг:
       cr=y-rollerY;
       cphi=180+cphi;
       cd=cd+180;
*/
printf("x0=%.3f y0=%.3f r=%.3f (%.2f)\n",cx,cy,cr,cd-cphi);

       a3e=cd-cphi+Axis3;
//Axis3=a3e;
//       inCirc=1;

       memset(lbuf,0,32);
       lbuf[0]=1;
       memcpy(lbuf+4,&xe,4);
       memcpy(lbuf+8,&ye,4);
       memcpy(lbuf+12,&ce,4);
       memcpy(lbuf+16,&lvel,4);
       memcpy(lbuf+20,&xa,4);
       memcpy(lbuf+24,&ya,4);
       memcpy(lbuf+28,&ca,4);

       if(sendto(s,lbuf,32,0,&rsa,16)==-1)
         printf("send failed\n");
      }

      if(lParam==(LPARAM)hResetErrorButton){
       memset(lbuf,0,32);
       lbuf[0]=2;
       if(sendto(s,lbuf,32,0,&rsa,16)==-1)
         printf("send failed\n");
      }

      if(lParam==(LPARAM)hButtonOpen){
        if(GetOpenFileName(&ofn)){
          pnum=0;
          int v;
          SendMessage(hTest,LVM_DELETEALLITEMS,0,0);
#ifndef O_BINARY
#define O_BINARY 0
#endif
          int f=open(ofn.lpstrFile,O_RDONLY|O_BINARY);
          struct stat st;
          fstat(f,&st);

          char *fbuf=malloc(st.st_size+1);
          fbuf[st.st_size]=0;
          read(f,fbuf,st.st_size);
          close(f);
          char *offset=fbuf,*lineStart;
          char pointName[256];
          li.iItem=0;

//Формат файла: название точки, сварочный ток[кА], давление[бар], скорость [м/с], тип движения (L-линейное,C-круговое) , координата X[мм],координата Y[мм], вспомогательная X (для кругового движения), вспомогательная Y

          do{
            lineStart=offset;
            offset=strchr(offset,'\n')+1;
//FIXME. in OS Windows string ends with TWO bytes: cr+lf (\r\n)
            if(offset==lineStart+2){
              printf("empty string\n");
              continue;
            }
            char *end=strchr(lineStart,';');
            *end=0;
            memcpy(pointName,lineStart,end-lineStart+1);
            sprintf(li.pszText,pointName);

            li.iSubItem=0;
            SendMessage(hTest,LVM_INSERTITEM,0,(LPARAM)&li);

            float derCurrent=atof(end+1);
            sprintf(li.pszText,"%.3f\n",derCurrent);
            li.iSubItem=1;
            SendMessage(hTest,LVM_SETITEM,0,(LPARAM)&li);
            end=strchr(end+1,';');
            float derPressure=atof(end+1);
            sprintf(li.pszText,"%.3f\n",derPressure);
            li.iSubItem=2;
            SendMessage(hTest,LVM_SETITEM,0,(LPARAM)&li);
            end=strchr(end+1,';');

float derVel=atof(end+1);
printf("vel=%.3f\n",derVel);
end=strchr(end+1,';');
trajectory[pnum].vel=derVel;
char derType=end[1];
printf("%c\n",derType);
end=strchr(end+1,';');
if(derType=='L')
 trajectory[pnum].type=0;
else if(derType=='C')
 trajectory[pnum].type=1;
else
 printf("derType=%c\n",derType);
float derX=atof(end+1);
printf("%.3f\n",derX);
end=strchr(end+1,';');
trajectory[pnum].x=derX;

float derY=atof(end+1);
printf("%.3f\n",derY);
end=strchr(end+1,';');
trajectory[pnum].y=derY;

float derC=atof(end+1);
printf("%.3f\n",derC);
end=strchr(end+1,';');
trajectory[pnum].c=derC;

if(derType=='C'){
 float derXaux=atof(end+1);
 printf("%.3f\n",derXaux);
 end=strchr(end+1,';');
 trajectory[pnum].xa=derXaux;

 float derYaux=atof(end+1);
 printf("%.3f\n",derYaux);
 end=strchr(end+1,';');
 trajectory[pnum].ya=derYaux;

 float derCaux=atof(end+1);
 printf("%.3f\n",derCaux);
 end=strchr(end+1,';');
 trajectory[pnum].ca=derCaux;
}           

            li.iItem++;
            if(offset==(char*)1)
              break;
            pnum++;
          }while(offset<fbuf+st.st_size);
trajectoryLength=pnum;
printf("pnum=%i\n",pnum);
          SendMessage(hTest,WM_PAINT,0,0);
          offset=fbuf;
//printf("%i\n",pnum);
for(int i=0;i<st.st_size;i++)
 if(*(fbuf+i)==0)
    *(fbuf+i)=';';
          int idx=0;
          do{
            lineStart=offset;
            offset=strchr(offset,'\n')+1;

            if(offset==lineStart+2){
              printf("empty string\n");
              continue;
            }
            char *end=strchr(lineStart,';');
            *end=0;
            memcpy(pointName,lineStart,end-lineStart+1);
printf("%llX %s\n",lineStart,pointName);
            float derX=atof(end+1);
            end=strchr(end+1,';');
            float derY=atof(end+1);
            end=strchr(end+1,';');

            if(offset==(char*)1)
              break;
            idx++;
          }while(offset<fbuf+st.st_size);
/*
for(int i=0;i<pnum;i++)
printf("%i: %.3f %.3f %.3f\n",i,points[i*3],points[i*3+1],points[i*3+2]);
*/
        }
      }else{
        if(lParam==(LPARAM)hPointVel){
          DefWindowProc((HWND)lParam,msg,wParam,0);
//        printf("hi:%i lo:%i\n",HIWORD(wParam),LOWORD(wParam));
         // printf("command %llX %llX(%llX) %llX\n",wParam,lParam,hPointVel,hwnd);
        }
      }
      if(lParam==(LPARAM)hButtonRunTrajectory){
        if(currentPoint==-1){
         currentPoint=0;
         EnableWindow(hButtonRunTrajectory,0);
        }
      }
      if(lParam==(LPARAM)hXPlusButton){
       GetWindowText(hIncValue,lbuf,255);
       x+=atof(lbuf);
//       doInverse();
       char lbuf[32];
/*
       memset(lbuf,0,32);
       memcpy(lbuf+4,&x,4);
       memcpy(lbuf+8,&y,4);
       memcpy(lbuf+12,&c,4);
       memcpy(lbuf+16,&lvel,4);
       lbuf[0]=0;
       printf("%u.%u.%u.%u\n",(unsigned char)rsa.sa_data[2],(unsigned char)rsa.sa_data[3],(unsigned char)rsa.sa_data[4],(unsigned char)rsa.sa_data[5]);
       if(sendto(s,lbuf,32,0,&rsa,16)==-1)
         printf("send failed\n");
*/
      }
      if(lParam==(LPARAM)hXMinusButton){
       GetWindowText(hIncValue,lbuf,255);
       x-=atof(lbuf);
       doInverse();
      }
      if(lParam==(LPARAM)hYPlusButton){
       GetWindowText(hIncValue,lbuf,255);
       y+=atof(lbuf);
       doInverse();
      }
      if(lParam==(LPARAM)hYMinusButton){
       GetWindowText(hIncValue,lbuf,255);
       y-=atof(lbuf);
       doInverse();
      }
//SetFocus(hTest);
    break;
    case WM_NOTIFY:
      if(((NMHDR*)lParam)->hwndFrom==hTest){
//       printf("notify code=%i\n",((NMLISTVIEW*)lParam)->hdr.code/*((NMHDR*)lParam)->code*/);
       //printf("LVN_ITEMACTIVATE %llX\n",LVN_ITEMACTIVATE);
//       printf("LVN_ITEMCHANGED (%i)%i\n",((NMLISTVIEW*)lParam)->hdr.code,LVN_ITEMCHANGED);
//printf("WM %llX\n",
switch(((NMHDR*)lParam)->code){
 case LVN_ITEMACTIVATE:
//  printf("item activate %i %i\n",((NMITEMACTIVATE*)lParam)->iItem,((NMITEMACTIVATE*)lParam)->iSubItem);
 break;

 case LVN_ITEMCHANGED:
//  printf("item changed %i %i\n",((NMLISTVIEW*)lParam)->iItem,((NMLISTVIEW*)lParam)->iSubItem); break;
 break;

 case NM_CUSTOMDRAW:
  return DefWindowProc(hwnd,msg,wParam,lParam);
 break;
 case NM_CLICK:
  //printf("click %i %i\n",((NMITEMACTIVATE*)lParam)->iItem,((NMITEMACTIVATE*)lParam)->iSubItem);
  SendMessage(hTest,WM_PAINT,0,0);
 break;
 default:
;
//  printf("some code:%i (%i)\n",((NMHDR*)lParam)->code,NM_FIRST);
}
      }
    break;
    case WM_DESTROY:
      PostQuitMessage(0);
    break;
    default:
//     printf("%llX\n",msg);
      return DefWindowProc(hwnd,msg,wParam,lParam);
  }
  return 0;
}

LRESULT CALLBACK WinProcOpenFile(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam){
  switch(msg){
   case WM_LBUTTONUP:
printf("LBUTTONUP procopen\n");
//     SendMessage(hListPoints,WM_PAINT,0,0);
   break;
   default:
     DefWindowProc(hwnd,msg,wParam,lParam);
  }
  return 0;
}

LRESULT CALLBACK WinProcEdit(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam){
 switch(msg){
  default:
   case WM_COMMAND:
    switch(wParam){
     default:
      return DefWindowProc(hwnd,msg,wParam,lParam);
    }
   break;
   return DefWindowProc(hwnd,msg,wParam,lParam);
 }
 return 0;
}

LRESULT CALLBACK WinProcOpenGL(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam){
 switch(msg){
   case WM_PAINT:
     glClearColor(0,0,0,0);
     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     draw();
     glFinish();
     SwapBuffers(hDC);
   break;
   case WM_SETFOCUS:
    SetFocus(opengl);
   break;
   case WM_KEYDOWN:
    switch(wParam){
    case 0x51:
      dL1=1;
    break;
    case 0x57:
      dL2=1;
    break;
    case 0x45:
      dA3=1;
    break;
    case 0x41:
      dL1=-1;
    break;
    case 0x53:
      dL2=-1;
    break;
    case 0x44:
      dA3=-1;
    break;
    case 0x5A:
     task=1;
     t0=0;
    // doLine();
    break;
    case 0x25:
      tx_d=-0.02;
    break;
    case 0x26:
      ty_d=0.02;
    break;
    case 0x27:
      tx_d=0.02;
    break;
    case 0x28:
      ty_d=-0.02;
    break;
    case 0x6B: //-
      zoom_d=1/1.01;
    break;
    case 0x6D: //+
      zoom_d=1.01;
    break;
    case 0X6E:
//     resetview();
     tx=500;
     ty=1000;
     tz=0;

     rot[0]=-50;
     rot[1]=10;

     zoom=0.1;
    break;
    case 0x67: //top view
     rot[0]=0;
     rot[1]=0;
    break;
    default:
printf("%llX\n",wParam);
    }
  break;
  case WM_KEYUP:
    switch(wParam){
    case 0x51:
      dL1=0;
    break;
    case 0x57:
      dL2=0;
    break;
    case 0x45:
      dA3=0;
    break;
    case 0x41:
      dL1=0;
    break;
    case 0x53:
      dL2=0;
    break;
    case 0x44:
      dA3=0;
    break;
    case 0x25:
    case 0x27:
     tx_d=0;
    break;
    case 0x26:
    case 0x28:
     ty_d=0;
    break;
    case 0x6B: //-
    case 0x6D: //+
      zoom_d=1;
    break;
    }
  break;
  case WM_LBUTTONDOWN:
    SetFocus(opengl);
  break;
  default:
//    printf("[opengl window]msg %08X\n",msg);
    return CallWindowProc(openglProc,hwnd,msg,wParam,lParam);
  }
  return CallWindowProc(openglProc,hwnd,msg,wParam,lParam);
}
