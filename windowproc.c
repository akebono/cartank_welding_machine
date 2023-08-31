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
       wglUseFontBitmaps (hDC, 0, 255, 1000);
glListBase (1000); 
      init_opengl();

    break;
    case WM_PAINT:

/*

 HBRUSH brush=CreateSolidBrush(RGB(255,128,128));
// HDC hdcmem = CreateCompatibleDC(((LPNMCUSTOMDRAW)hwnd)->hdc);
 HDC hdcmem=GetDC(hwnd);
 RECT temp;
 temp.left=0;
 temp.top=0;
 temp.right=500;
 temp.bottom=h+325;
 FillRect(hdcmem, &temp, brush);
*/    break;
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
      if(strafedrag){
        trans[0]=LOWORD(lParam)-dragstart[0];
        trans[1]=HIWORD(lParam)-dragstart[1];
      }
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
      if(lParam==(LPARAM)hButtonDraw){
       if(show_trajectory){
        SendMessage(hButtonDraw,BM_SETCHECK,BST_UNCHECKED,0);
        show_trajectory=0;
       }else{
        SendMessage(hButtonDraw,BM_SETCHECK,BST_CHECKED,0);
        show_trajectory=1;
       }
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

       memset(lbuf,0,40);
       lbuf[0]=1;
       memcpy(lbuf+4,&lx,4);
       memcpy(lbuf+8,&ly,4);
       memcpy(lbuf+12,&lc,4);
       memcpy(lbuf+16,&lvel,4);
       memcpy(lbuf+32,&sernumback,4);
       printf("%u.%u.%u.%u\n",(unsigned char)rsa.sa_data[2],(unsigned char)rsa.sa_data[3],(unsigned char)rsa.sa_data[4],(unsigned char)rsa.sa_data[5]);
       if(sendto(s,lbuf,40,0,&rsa,16)==-1)
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

       memset(lbuf,0,40);
       lbuf[0]=2;
       memcpy(lbuf+4,&xe,4);
       memcpy(lbuf+8,&ye,4);
       memcpy(lbuf+12,&ce,4);
       memcpy(lbuf+16,&lvel,4);
       memcpy(lbuf+20,&xa,4);
       memcpy(lbuf+24,&ya,4);
       memcpy(lbuf+28,&ca,4);
       memcpy(lbuf+32,&sernumback,4);
       if(sendto(s,lbuf,40,0,&rsa,16)==-1)
         printf("send failed\n");
      }

      if(lParam==(LPARAM)hResetErrorButton){
       memset(lbuf,0,40);
       lbuf[0]=4;
       if(sendto(s,lbuf,40,0,&rsa,16)==-1)
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
            trajectory[pnum].name=malloc(strlen(pointName)+1);
            sprintf(trajectory[pnum].name,pointName);
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
            trajectory[pnum].vel=derVel;

            end=strchr(end+1,';');
            char derType=end[1];
            if(derType=='L')
             trajectory[pnum].type=1;
            else if(derType=='C')
             trajectory[pnum].type=2;
            else
             printf("derType=%c\n",derType);

            end=strchr(end+1,';');
            float derX=atof(end+1);
            trajectory[pnum].x=derX;

            end=strchr(end+1,';');
            float derY=atof(end+1);
            trajectory[pnum].y=derY;

            end=strchr(end+1,';');
            float derC=atof(end+1);
            trajectory[pnum].c=derC;

            if(derType=='C'){
             end=strchr(end+1,';');
             float derXaux=atof(end+1);
             trajectory[pnum].xa=derXaux;

             end=strchr(end+1,';');
             float derYaux=atof(end+1);
             trajectory[pnum].ya=derYaux;

             end=strchr(end+1,';');
             float derCaux=atof(end+1);
             trajectory[pnum].ca=derCaux;
            }           

            end=strchr(end+1,';');
            li.iItem++;
            if(offset==(char*)1)
              break;
            pnum++;
          }while(offset<fbuf+st.st_size);
          trajectoryLength=pnum;
          doTrajectory=0;
          trajectoryDone=0;
          currentPoint=-1;
          SetWindowText(hButtonRunTrajectory,"Run");
          sernumstart=sernumback;
          EnableWindow(hButtonRunTrajectory,1);
          EnableWindow(hButtonResetTrajectory,0);
          EnableWindow(hRadioStepTrajectory,1);
          memset(lbuf,0,36);
          lbuf[0]=32;
          if(sendto(s,lbuf,36,0,&rsa,16)==-1)
           printf("send failed\n");
          SendMessage(hTest,WM_PAINT,0,0);
          if(pnum>0){
           EnableWindow(hButtonRunTrajectory,1);
           EnableWindow(hRadioStepTrajectory,1);
          }
          offset=fbuf;

          for(int i=0;i<st.st_size;i++)
           if(*(fbuf+i)==0)
            *(fbuf+i)=';';

          int idx=0;
/*
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
            float derX=atof(end+1);
            end=strchr(end+1,';');
            float derY=atof(end+1);
            end=strchr(end+1,';');

            if(offset==(char*)1)
              break;
            idx++;
          }while(offset<fbuf+st.st_size);
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
printf("before. doTrajectory=%i currentPoint=%i\n",doTrajectory,currentPoint);
        if(doTrajectory==1){
         doTrajectory=0;
         SetWindowText(hButtonRunTrajectory,"Cont.");
         EnableWindow(hRadioStepTrajectory,1);
         EnableWindow(hButtonOpen,1);
         memset(lbuf,0,40);
         lbuf[0]=8;
//         sernumsent=sernumstart+currentPoint;
         memcpy(lbuf+32,&sernumback,4);
         if(sendto(s,lbuf,40,0,&rsa,16)==-1)
          printf("send failed\n");
        }else{
         if(currentPoint>-1){
          memset(lbuf,0,40);
          lbuf[0]=16;
          sernumsent=sernumstart+currentPoint;
          memcpy(lbuf+32,&sernumsent,4);
          if(sendto(s,lbuf,40,0,&rsa,16)==-1)
           printf("send failed\n");
          SetWindowText(hButtonRunTrajectory,"Pause");
          doTrajectory=1;
          EnableWindow(hRadioStepTrajectory,0);
          EnableWindow(hButtonOpen,0);
         }else{
          trajectoryDone=0;
          currentPoint=0;
          sernumstart=sernumback;
          doTrajectory=1;
          SetWindowText(hButtonRunTrajectory,"Pause");
          EnableWindow(hButtonResetTrajectory,1);
          EnableWindow(hRadioStepTrajectory,0);
          EnableWindow(hButtonOpen,0);
         }
        }
        printf("after. doTrajectory=%i currentPoint=%i sernumback=%i sernumsent=%i\n",doTrajectory,currentPoint,sernumback,sernumsent);
      }

      if(lParam==(LPARAM)hRadioStepTrajectory){
/*
       char lbuf[36];
       memset(lbuf,0,36);
       memcpy(lbuf,&trajectory[currentPoint].type,4);
       memcpy(lbuf+4,&trajectory[currentPoint].x,4);
       memcpy(lbuf+8,&trajectory[currentPoint].y,4);
       memcpy(lbuf+12,&trajectory[currentPoint].c,4);
       memcpy(lbuf+16,&trajectory[currentPoint].vel,4);
       memcpy(lbuf+20,&trajectory[currentPoint].xa,4);
       memcpy(lbuf+24,&trajectory[currentPoint].ya,4);
       memcpy(lbuf+28,&trajectory[currentPoint].ca,4);
       sernumsent=sernumstart+currentPoint;
       memcpy(lbuf+32,&sernumsent,2);
       sendPacket(lbuf);
       printf("current point:%i(%i) %s\n",currentPoint,trajectoryLength,trajectory[currentPoint].name);
       currentPoint++;
*/
       SendMessage(hRadioStepTrajectory,BM_SETCHECK,BST_CHECKED,0);
       SendMessage(hRadioContTrajectory,BM_SETCHECK,BST_UNCHECKED,0);
      }

      if(lParam==(LPARAM)hButtonResetTrajectory){
       doTrajectory=0;
       trajectoryDone=0;
       currentPoint=-1;
       SetWindowText(hButtonRunTrajectory,"Run");
       sernumstart=sernumback;
       EnableWindow(hButtonRunTrajectory,1);
       EnableWindow(hButtonResetTrajectory,0);
       EnableWindow(hRadioStepTrajectory,1);
       memset(lbuf,0,40);
       lbuf[0]=32;
       if(sendto(s,lbuf,40,0,&rsa,16)==-1)
        printf("send failed\n");
      }

      if(lParam==(LPARAM)hXPlusButton){
       GetWindowText(hIncValue,lbuf,255);
       float newx=x+atof(lbuf);
       GetWindowText(hIncVelValue,lbuf,255);
       float vel=atof(lbuf);

       memset(lbuf,0,40);
       lbuf[0]=1;
       memcpy(lbuf+4,&newx,4);
       memcpy(lbuf+8,&y,4);
       memcpy(lbuf+12,&c,4);
       memcpy(lbuf+16,&vel,4);
       memcpy(lbuf+32,&sernumstart,4);

       sendPacket(lbuf);
      }
      if(lParam==(LPARAM)hXMinusButton){
       GetWindowText(hIncValue,lbuf,255);
       float newx=x-atof(lbuf);
       GetWindowText(hIncVelValue,lbuf,255);
       float vel=atof(lbuf);
       memset(lbuf,0,40);
       lbuf[0]=1;
       memcpy(lbuf+4,&newx,4);
       memcpy(lbuf+8,&y,4);
       memcpy(lbuf+12,&c,4);
       memcpy(lbuf+16,&vel,4);
       memcpy(lbuf+32,&sernumstart,4);

       sendPacket(lbuf);
      }
      if(lParam==(LPARAM)hYPlusButton){
       GetWindowText(hIncValue,lbuf,255);
       float newy=y+atof(lbuf);
       GetWindowText(hIncVelValue,lbuf,255);
       float vel=atof(lbuf);

       memset(lbuf,0,40);
       lbuf[0]=1;
       memcpy(lbuf+4,&x,4);
       memcpy(lbuf+8,&newy,4);
       memcpy(lbuf+12,&c,4);
       memcpy(lbuf+16,&vel,4);

       memcpy(lbuf+32,&sernumstart,4);

       sendPacket(lbuf);
      }
      if(lParam==(LPARAM)hYMinusButton){
       GetWindowText(hIncValue,lbuf,255);
       float newy=y-atof(lbuf);
       GetWindowText(hIncVelValue,lbuf,255);
       float vel=atof(lbuf);


       memset(lbuf,0,40);
       lbuf[0]=1;
       memcpy(lbuf+4,&x,4);
       memcpy(lbuf+8,&newy,4);
       memcpy(lbuf+12,&c,4);
       memcpy(lbuf+16,&vel,4);
       memcpy(lbuf+32,&sernumstart,4);

       sendPacket(lbuf);
      }
      if(lParam==(LPARAM)hCPlusButton){
       GetWindowText(hIncValue,lbuf,255);
       float newc=c+atof(lbuf);
       GetWindowText(hIncVelValue,lbuf,255);
       float vel=atof(lbuf);


       memset(lbuf,0,40);
       lbuf[0]=1;
       memcpy(lbuf+4,&x,4);
       memcpy(lbuf+8,&y,4);
       memcpy(lbuf+12,&newc,4);
       memcpy(lbuf+16,&vel,4);
       memcpy(lbuf+32,&sernumstart,4);

       sendPacket(lbuf);
      }
      if(lParam==(LPARAM)hCMinusButton){
       GetWindowText(hIncValue,lbuf,255);
       float newc=c-atof(lbuf);
       GetWindowText(hIncVelValue,lbuf,255);
       float vel=atof(lbuf);

       memset(lbuf,0,40);
       lbuf[0]=1;
       memcpy(lbuf+4,&x,4);
       memcpy(lbuf+8,&y,4);
       memcpy(lbuf+12,&newc,4);
       memcpy(lbuf+16,&vel,4);
       memcpy(lbuf+32,&sernumstart,4);

       sendPacket(lbuf);
      }
      if(lParam==(LPARAM)hCopyLinButton){
       sprintf(lbuf,"%.1f",x);
       SetWindowText(hXLin,lbuf);
       sprintf(lbuf,"%.1f",y);
       SetWindowText(hYLin,lbuf);
       sprintf(lbuf,"%.1f",c);
       SetWindowText(hCLin,lbuf);
       SendMessage(hXLin,WM_PAINT,0,0);
       SendMessage(hYLin,WM_PAINT,0,0);
       SendMessage(hCLin,WM_PAINT,0,0);
      }
      if(lParam==(LPARAM)hCopyCircButton){
       sprintf(lbuf,"%.1f",x);
       SetWindowText(hXCircEnd,lbuf);
       sprintf(lbuf,"%.1f",y);
       SetWindowText(hYCircEnd,lbuf);
       sprintf(lbuf,"%.1f",c);
       SetWindowText(hCCircEnd,lbuf);
       SendMessage(hXCircEnd,WM_PAINT,0,0);
       SendMessage(hYCircEnd,WM_PAINT,0,0);
       SendMessage(hCCircEnd,WM_PAINT,0,0);
      }
      if(lParam==(LPARAM)hCopyCircAuxButton){
       sprintf(lbuf,"%.1f",x);
       SetWindowText(hXCircAux,lbuf);
       sprintf(lbuf,"%.1f",y);
       SetWindowText(hYCircAux,lbuf);
       sprintf(lbuf,"%.1f",c);
       SetWindowText(hCCircAux,lbuf);
       SendMessage(hXCircAux,WM_PAINT,0,0);
       SendMessage(hYCircAux,WM_PAINT,0,0);
       SendMessage(hCCircAux,WM_PAINT,0,0);
      }
      if(lParam==(LPARAM)hBit00){
       testword^=1;
       sprintf(lbuf,"%04X",testword);
       SetWindowText(hSendTestWordInput,lbuf);
       SendMessage(hSendTestWordInput,WM_PAINT,0,0);
       if(testword&1) SendMessage(hBit00,BM_SETCHECK,BST_CHECKED,0);
       else SendMessage(hBit00,BM_SETCHECK,BST_UNCHECKED,0);
      }
      if(lParam==(LPARAM)hBit01){
       testword^=2;
       sprintf(lbuf,"%04X",testword);
       SetWindowText(hSendTestWordInput,lbuf);
       SendMessage(hSendTestWordInput,WM_PAINT,0,0);
       if(testword&2) SendMessage(hBit01,BM_SETCHECK,BST_CHECKED,0);
       else SendMessage(hBit01,BM_SETCHECK,BST_UNCHECKED,0);
      }
      if(lParam==(LPARAM)hBit02){
       testword^=4;
       sprintf(lbuf,"%04X",testword);
       SetWindowText(hSendTestWordInput,lbuf);
       SendMessage(hSendTestWordInput,WM_PAINT,0,0);
       if(testword&4) SendMessage(hBit02,BM_SETCHECK,BST_CHECKED,0);
       else SendMessage(hBit02,BM_SETCHECK,BST_UNCHECKED,0);
      }
      if(lParam==(LPARAM)hBit03){
       testword^=8;
       sprintf(lbuf,"%04X",testword);
       SetWindowText(hSendTestWordInput,lbuf);
       SendMessage(hSendTestWordInput,WM_PAINT,0,0);
       if(testword&8) SendMessage(hBit03,BM_SETCHECK,BST_CHECKED,0);
       else SendMessage(hBit03,BM_SETCHECK,BST_UNCHECKED,0);
      }
      if(lParam==(LPARAM)hBit04){
       testword^=16;
       sprintf(lbuf,"%04X",testword);
       SetWindowText(hSendTestWordInput,lbuf);
       SendMessage(hSendTestWordInput,WM_PAINT,0,0);
       if(testword&0x10) SendMessage(hBit04,BM_SETCHECK,BST_CHECKED,0);
       else SendMessage(hBit04,BM_SETCHECK,BST_UNCHECKED,0);
      }
      if(lParam==(LPARAM)hBit05){
       testword^=32;
       sprintf(lbuf,"%04X",testword);
       SetWindowText(hSendTestWordInput,lbuf);
       SendMessage(hSendTestWordInput,WM_PAINT,0,0);
       if(testword&0x20) SendMessage(hBit05,BM_SETCHECK,BST_CHECKED,0);
       else SendMessage(hBit05,BM_SETCHECK,BST_UNCHECKED,0);
      }
      if(lParam==(LPARAM)hBit06){
       testword^=64;
       sprintf(lbuf,"%04X",testword);
       SetWindowText(hSendTestWordInput,lbuf);
       SendMessage(hSendTestWordInput,WM_PAINT,0,0);
       if(testword&0x40) SendMessage(hBit06,BM_SETCHECK,BST_CHECKED,0);
       else SendMessage(hBit06,BM_SETCHECK,BST_UNCHECKED,0);
      }
      if(lParam==(LPARAM)hBit07){
       testword^=128;
       sprintf(lbuf,"%04X",testword);
       SetWindowText(hSendTestWordInput,lbuf);
       SendMessage(hSendTestWordInput,WM_PAINT,0,0);
       if(testword&0x80) SendMessage(hBit07,BM_SETCHECK,BST_CHECKED,0);
       else SendMessage(hBit07,BM_SETCHECK,BST_UNCHECKED,0);
      }
      if(lParam==(LPARAM)hBit08){
       testword^=256;
       sprintf(lbuf,"%04X",testword);
       SetWindowText(hSendTestWordInput,lbuf);
       SendMessage(hSendTestWordInput,WM_PAINT,0,0);
       if(testword&0x100) SendMessage(hBit08,BM_SETCHECK,BST_CHECKED,0);
       else SendMessage(hBit08,BM_SETCHECK,BST_UNCHECKED,0);
      }
      if(lParam==(LPARAM)hBit09){
       testword^=512;
       sprintf(lbuf,"%04X",testword);
       SetWindowText(hSendTestWordInput,lbuf);
       SendMessage(hSendTestWordInput,WM_PAINT,0,0);
       if(testword&0x200) SendMessage(hBit09,BM_SETCHECK,BST_CHECKED,0);
       else SendMessage(hBit09,BM_SETCHECK,BST_UNCHECKED,0);
      }
      if(lParam==(LPARAM)hBit10){
       testword^=1024;
       sprintf(lbuf,"%04X",testword);
       SetWindowText(hSendTestWordInput,lbuf);
       SendMessage(hSendTestWordInput,WM_PAINT,0,0);
       if(testword&0x400) SendMessage(hBit10,BM_SETCHECK,BST_CHECKED,0);
       else SendMessage(hBit10,BM_SETCHECK,BST_UNCHECKED,0);
      }
      if(lParam==(LPARAM)hBit11){
       testword^=2048;
       sprintf(lbuf,"%04X",testword);
       SetWindowText(hSendTestWordInput,lbuf);
       SendMessage(hSendTestWordInput,WM_PAINT,0,0);
       if(testword&0x800) SendMessage(hBit11,BM_SETCHECK,BST_CHECKED,0);
       else SendMessage(hBit11,BM_SETCHECK,BST_UNCHECKED,0);
      }
      if(lParam==(LPARAM)hBit12){
       testword^=4096;
       sprintf(lbuf,"%04X",testword);
       SetWindowText(hSendTestWordInput,lbuf);
       SendMessage(hSendTestWordInput,WM_PAINT,0,0);
       if(testword&0x1000) SendMessage(hBit12,BM_SETCHECK,BST_CHECKED,0);
       else SendMessage(hBit12,BM_SETCHECK,BST_UNCHECKED,0);
      }
      if(lParam==(LPARAM)hBit13){
       testword^=8192;
       sprintf(lbuf,"%04X",testword);
       SetWindowText(hSendTestWordInput,lbuf);
       SendMessage(hSendTestWordInput,WM_PAINT,0,0);
       if(testword&0x2000) SendMessage(hBit13,BM_SETCHECK,BST_CHECKED,0);
       else SendMessage(hBit13,BM_SETCHECK,BST_UNCHECKED,0);
      }
      if(lParam==(LPARAM)hBit14){
       testword^=16384;
       sprintf(lbuf,"%04X",testword);
       SetWindowText(hSendTestWordInput,lbuf);
       SendMessage(hSendTestWordInput,WM_PAINT,0,0);
       if(testword&0x4000) SendMessage(hBit14,BM_SETCHECK,BST_CHECKED,0);
       else SendMessage(hBit14,BM_SETCHECK,BST_UNCHECKED,0);
      }
      if(lParam==(LPARAM)hBit15){
       testword^=32768;
       sprintf(lbuf,"%04X",testword);
       SetWindowText(hSendTestWordInput,lbuf);
       SendMessage(hSendTestWordInput,WM_PAINT,0,0);
       if(testword&0x8000) SendMessage(hBit15,BM_SETCHECK,BST_CHECKED,0);
       else SendMessage(hBit15,BM_SETCHECK,BST_UNCHECKED,0);
      }
      if(lParam==(LPARAM)hSendTestWordButton){
       char lbuf[40];
       memset(lbuf,0,40);
       lbuf[0]=64;

       memcpy(lbuf+36,&testword,2);

       sendPacket(lbuf);
      }
//SetFocus(hTest);
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
     if(status&0x80000000)
      glClearColor(1,0,0,1);
     else
      glClearColor(0,0,0,1);
     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     draw();
     glFinish();
     SwapBuffers(hDC);
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
      tx_d=-t_vel;
    break;
    case 0x26:
      ty_d=t_vel;
    break;
    case 0x27:
      tx_d=t_vel;
    break;
    case 0x28:
      ty_d=-t_vel;
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
  case WM_MOUSEWHEEL:
    float m[16]={1.4142,0,0,0,0,1.4142,0,0,0,0,1.4142,0,0,0,0,1};
    float d[16]={1/1.4142,0,0,0,0,1/1.4142,0,0,0,0,1/1.4142,0,0,0,0,1};
   if((int)wParam>0){

//    glMultMatrixf(d);
    zoom/=1.4142;
   }else{
    zoom*=1.4142;
//    glMultMatrixf(m);
   }
  break;

  case WM_LBUTTONDOWN:
   //glPushMatrix();
   //glMatrixMode(GL_PROJECTION);
//   gluPickMatrix(LOWORD(lParam),HIWORD(lParam),1,1,viewport);
   //glPopMatrix();
   SetFocus(opengl);
   dragstart[0]=LOWORD(lParam)-trans[0];
   dragstart[1]=HIWORD(lParam)-trans[1];
   strafedrag=1;
  break;
  case WM_LBUTTONUP:
    strafedrag=0;
  break;
  default:
//    printf("[opengl window]msg %08X\n",msg);
    return CallWindowProc(openglProc,hwnd,msg,wParam,lParam);
  }
  return CallWindowProc(openglProc,hwnd,msg,wParam,lParam);
}
