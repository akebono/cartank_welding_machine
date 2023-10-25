
void doCalc2(){
 Axis1=180-acos((A1lever*A1lever+A1offset*A1offset-L1*L1)/(2*A1lever*A1offset))*180/pi;

 A1motor=asin(A1lever*sin((180-Axis1)*pi/180)/L1)*180/pi; //направление штанги первой оси

 Axis2=90-acos((A2lever*A2lever+A2offset*A2offset-L2*L2)/(2*A2lever*A2offset))*180/pi-Axis1;
 A2motor=asin(A2lever*sin((90-Axis2-Axis1)*pi/180)/L2)*180/pi; //направление штанги второй оси


 x=-g*cos((Axis1+Axis2+Axis3)*pi/180-pi-psi)-Arm1*cos((Axis3+Axis2)*pi/180)-Arm2*cos(Axis3*pi/180);
 xc=x;
 y=g*sin((Axis1+Axis2+Axis3)*pi/180-pi-psi)+Arm1*sin((Axis3+Axis2)*pi/180)+Arm2*sin(Axis3*pi/180);
 yc=y;
 c=Axis1+Axis2+Axis3;
}

void doInverse2(){
 float lAxis1,lAxis2,lAxis3;
 float lr2=x*x+y*y;
 curpt++;
 lAxis3=(c-Axis2-Axis1)*pi/180;
// float theta=pi/2-lAxis3;
 float theta=c;
 if(x<0 && y<0)
  theta+=atan(y/x);
 if(x>=0 && y<0)
  theta+=pi+atan(y/x);
 if(x>=0 && y>=0)
  theta+=pi-atan(y/x);
 if(x<0 && y>=0)
  theta-=atan(y/x);

 if(x==0)
  theta=pi/2+c;

 float e2=Arm2*Arm2+lr2-2*sqrt(lr2)*Arm2*cos(theta);
 lAxis1=psi+acos((Arm1*Arm1+g*g-e2)/(2*Arm1*g));
 float part1=acos((Arm1*Arm1+e2-g*g)/(2*Arm1*sqrt(e2)));
 float part2=acos((Arm2*Arm2+e2-lr2)/(2*Arm2*sqrt(e2)));
 if(y>0)
  lAxis2=part1-part2-pi;
 else
  lAxis2=part1+part2-pi;
printf("atan=%.1f theta=%.1f e=%.1f\n",atan(y/x)*180/pi,cos(theta),sqrt(e2));
 lAxis2*=180/pi;
 lAxis3*=180/pi;
 lAxis1*=180/pi;

 printf("inverse: %.1f %.1f %.1f\n",lAxis1,lAxis2,lAxis3);

 Axis3=lAxis3;
 Axis2=lAxis2;
 Axis1=lAxis1;
 L1=sqrt(A1offset*A1offset+A1lever*A1lever-2*A1offset*A1lever*cos((180-lAxis1)*pi/180));
 L2=sqrt(A2offset*A2offset+A2lever*A2lever-2*A2offset*A2lever*cos((90-lAxis2-lAxis1)*pi/180));
}

void doInverse3(){
 float lAxis1,lAxis2,lAxis3;
 float lr2=x*x+y*y;
 curpt++;
 lAxis3=(c-Axis2-Axis1)*pi/180;

 float gamma=c;
 if(x>=0 && y>=0){
  gamma+=pi/2-atan(y/x)-psi;
 }
 if(x>=0 && y<0){
  gamma+=pi/2+atan(x/y)+psi;
 }
 if(x<0 && y>=0)
  gamma+=pi/2-atan(y/x)-psi;
 if(x<0 && y<0)
  gamma=pi-psi+atan(y/x);

 if(x==0)
  gamma+=pi/2;

 float f2=g*g+lr2-2*sqrt(lr2)*Arm2*cos(gamma);
 lAxis2=acos((Arm1*Arm1+Arm2*Arm2-f2)/(2*Arm1*Arm2));
 float ga1=acos((g*g+f2-sqrt(lr2))/(2*g*sqrt(f2)));
 float ga1a=acos((Arm1*Arm1+f2-Arm1*Arm1)/(2*Arm1*sqrt(f2)));

 //todo variants
 lAxis1=ga1a-ga1+psi;

 lAxis2*=180/pi;
 lAxis3*=180/pi;
 lAxis1*=180/pi;

 printf("inverse: %.1f %.1f %.1f\n",lAxis1,lAxis2,lAxis3);

 Axis3=lAxis3;
 Axis2=lAxis2;
 Axis1=lAxis1;
 L1=sqrt(A1offset*A1offset+A1lever*A1lever-2*A1offset*A1lever*cos((180-lAxis1)*pi/180));
 L2=sqrt(A2offset*A2offset+A2lever*A2lever-2*A2offset*A2lever*cos((90-lAxis2-lAxis1)*pi/180));
}


void draw2(){

 L1+=dL1*1;
 L2+=dL2*1;
 Axis3+=dA3*0.15;

 doCalc2();

  zoom*=zoom_d;
  tx+=tx_d*2000*sqrt(zoom);
  ty+=ty_d*2000*sqrt(zoom);

 if(((status&7==1) ||(status&0x38==8)) && !doTrajectory){//for when "Arc To" or "Lin To" done
  EnableWindow(hButtonResetTrajectory,1);
  EnableWindow(hButtonRunTrajectory,1);
 }

 if(!trajectoryDone){
  if(sernumback-sernumstart+1==trajectoryLength){
   SetWindowText(hButtonRunTrajectory,"Run");
   sernumstart=sernumback;
   EnableWindow(hButtonRunTrajectory,1);
   trajectoryDone=1;
  }
 }
 if(doTrajectory){
  if(currentPoint==trajectoryLength){
   doTrajectory=0;
   currentPoint=-1;
   SetWindowText(hButtonRunTrajectory,"Run");
   EnableWindow(hRadioStepTrajectory,1);
   EnableWindow(hButtonOpen,1);
  }else{
   if(runmode==0){
    if(((sernumback<=(sernumstart+currentPoint)) && (sernumback>=(sernumstart+currentPoint)-2))&& (!(status&36) || status&9)){
     char lbuf[40];
     memset(lbuf,0,40);
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
    }
   }
  }
 }

 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();

 if(GetFocus()==opengl){
  glDisable(GL_LIGHTING);
  glColor3f(1,0,0);
  glBegin(GL_QUADS);
  glVertex3f(-1,0.95,-1);
  glVertex3f(1,0.95,-1);
  glVertex3f(1,1,-1);
  glVertex3f(-1,1,-1);
  glEnd();
 }

 if(dL1 || dL2 ||dA3)
  curpt++;
 char lbuf[256];
 if(oldx!=x){
  sprintf(lbuf,"X:%.2f mm",x);
  SetWindowText(hXactual,lbuf);
  oldx=x;
 }
 if(oldy!=y){
  sprintf(lbuf,"Y:%.2f mm",y);
  SetWindowText(hYactual,lbuf);
  oldy=y;
 }
  if(oldc!=c){
   sprintf(lbuf,"C: %.2f deg",c);
   SetWindowText(hZactual,lbuf);
   oldc=c;
  }

  if(oldAxis1!=Axis1 || oldL1!=L1){
   sprintf(lbuf,"%.1f / %.3f",Axis1,L1);
   SetWindowText(hA1,lbuf);
   oldAxis1=Axis1;
   oldL1=L1;
  }
  if(oldAxis2!=Axis2 ||oldL2!=L2){
   sprintf(lbuf,"%.1f / %.3f",Axis2,L2);
   SetWindowText(hA2,lbuf);
   oldAxis2=Axis2;
   oldL2=L2;
  }
  if(oldAxis3!=Axis3){
   sprintf(lbuf,"%.1f",Axis3);
   SetWindowText(hA3,lbuf);
   oldAxis3=Axis3;
  }


 traj[curpt][0]=x;
 traj[curpt][1]=y;
 trajc[curpt][0]=xc;
 trajc[curpt][1]=yc;

 glOrtho(-w*40*zoom,w*40*zoom,-h*40*zoom,h*40*zoom,-100000,100000);

 glPushMatrix();
 glRotatef(rot[1]/1.0,1,0,0);
 glRotatef(rot[0]/1.0,0,1,0);
 glTranslatef(-tx+trans[0],-ty-trans[1],tz);

 glTranslatef(g,0,0);
 glRotatef(Axis1,0,0,1);
 glTranslatef(Arm1,0,0);
 glRotatef(Axis2,0,0,1);
 glTranslatef(Arm2,0,0);
 glRotatef(Axis3,0,0,1);


 if(show_trajectory){
  glDisable(GL_LIGHTING);
  glColor3f(1,1,0);
  glBegin(GL_LINES);
  for(int i=1;i<curpt;i++){
   glVertex3f(trajc[i-1][0],trajc[i-1][1],1300);
   glVertex3f(trajc[i][0],trajc[i][1],1300);
  }
  glEnd();
 }


 glColor3f(1,1,1);
 glEnable(GL_LIGHTING);
 
 glPushMatrix();
 int j=4;
 if(status&9)
  mat[0]=1;
 else
  mat[0]=0.2;
 if(status&18)
  mat[1]=1;
 else
  mat[1]=0.2;
 if(status&36)
  mat[2]=1;
 else
  mat[2]=0.2;
 mat[3]=1;
 glMaterialfv(GL_FRONT,GL_DIFFUSE,mat);

 glBegin(GL_TRIANGLES);
 for(int i=0;i<*(int*)blobs[j];i++){
  glNormal3f(*(float*)(blobs[j]+4+i*50),*(float*)(blobs[j]+8+i*50),*(float*)(blobs[j]+12+i*50));
  glVertex3f(*(float*)(blobs[j]+16+i*50),*(float*)(blobs[j]+20+i*50),*(float*)(blobs[j]+24+i*50));
  glVertex3f(*(float*)(blobs[j]+28+i*50),*(float*)(blobs[j]+32+i*50),*(float*)(blobs[j]+36+i*50));
  glVertex3f(*(float*)(blobs[j]+40+i*50),*(float*)(blobs[j]+44+i*50),*(float*)(blobs[j]+48+i*50));
 }
 glEnd();

 glPushMatrix();
 glRotatef(-Axis3,0,0,1);
 glTranslatef(-Arm2,0,0);
 j=1;
 mat[0]=0.2;
 mat[1]=1;
 mat[2]=0.1;
 mat[3]=1;
 glMaterialfv(GL_FRONT,GL_DIFFUSE,mat);
 glBegin(GL_TRIANGLES);
 for(int i=0;i<*(int*)blobs[j];i++){
  glNormal3f(*(float*)(blobs[j]+4+i*50),*(float*)(blobs[j]+8+i*50),*(float*)(blobs[j]+12+i*50));
  glVertex3f(*(float*)(blobs[j]+16+i*50),*(float*)(blobs[j]+20+i*50),*(float*)(blobs[j]+24+i*50));
  glVertex3f(*(float*)(blobs[j]+28+i*50),*(float*)(blobs[j]+32+i*50),*(float*)(blobs[j]+36+i*50));
  glVertex3f(*(float*)(blobs[j]+40+i*50),*(float*)(blobs[j]+44+i*50),*(float*)(blobs[j]+48+i*50));
 }
 glEnd();

 glPushMatrix();
 glRotatef(-Axis2,0,0,1);
 glTranslatef(-Arm1,0,0);
 j=2;
 mat[0]=1;
 mat[1]=0.1;
 mat[2]=0.1;
 mat[3]=1;
 glMaterialfv(GL_FRONT,GL_DIFFUSE,mat);
 glBegin(GL_TRIANGLES);
 for(int i=0;i<*(int*)blobs[j];i++){
  glNormal3f(*(float*)(blobs[j]+4+i*50),*(float*)(blobs[j]+8+i*50),*(float*)(blobs[j]+12+i*50));
  glVertex3f(*(float*)(blobs[j]+16+i*50),*(float*)(blobs[j]+20+i*50),*(float*)(blobs[j]+24+i*50));
  glVertex3f(*(float*)(blobs[j]+28+i*50),*(float*)(blobs[j]+32+i*50),*(float*)(blobs[j]+36+i*50));
  glVertex3f(*(float*)(blobs[j]+40+i*50),*(float*)(blobs[j]+44+i*50),*(float*)(blobs[j]+48+i*50));
 }
 glEnd();
 glPushMatrix();
 glRotatef(-Axis1,0,0,1);
 glTranslatef(toolpos[0],toolpos[1],0);
 j=6;
 mat[0]=1;
 mat[1]=0.9;
 mat[2]=1;
 mat[3]=1;
 glMaterialfv(GL_FRONT,GL_DIFFUSE,mat);
 glBegin(GL_TRIANGLES);
 for(int i=0;i<*(int*)blobs[j];i++){
  glNormal3f(*(float*)(blobs[j]+4+i*50),*(float*)(blobs[j]+8+i*50),*(float*)(blobs[j]+12+i*50));
  glVertex3f(*(float*)(blobs[j]+16+i*50),*(float*)(blobs[j]+20+i*50),*(float*)(blobs[j]+24+i*50));
  glVertex3f(*(float*)(blobs[j]+28+i*50),*(float*)(blobs[j]+32+i*50),*(float*)(blobs[j]+36+i*50));
  glVertex3f(*(float*)(blobs[j]+40+i*50),*(float*)(blobs[j]+44+i*50),*(float*)(blobs[j]+48+i*50));
 }
 glEnd();
 glDisable(GL_LIGHTING);

 glColor3f(0.5,0.5,0.5);

 glBegin(GL_LINES);

 for(int i=0;i<41;i++){
  glVertex3f(-20000,-20000+1000*i,0);
  glVertex3f(20000,-20000+1000*i,0);
  glVertex3f(-20000+1000*i,-20000,0);
  glVertex3f(-20000+1000*i,20000,0);
 }

 glColor3f(0.1,0.1,0.3);
 if(zoom<0.01){
  for(int i=0;i<1001;i++){
   glVertex3f(tx-500,ty-500+1*i,0);
   glVertex3f(tx+500,ty-500+1*i,0);
   glVertex3f(tx-500+1*i,ty-500,0);
   glVertex3f(tx-500+1*i,ty+500,0);
  }
 }
 glEnd();

 glPopMatrix();
 glPopMatrix();
 glPopMatrix();
 glPopMatrix();
}