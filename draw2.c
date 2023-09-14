
void doCalc2(){
 Axis1=180-acos((A1lever*A1lever+A1offset*A1offset-L1*L1)/(2*A1lever*A1offset))*180/pi;

 A1motor=asin(A1lever*sin((180-Axis1)*pi/180)/L1)*180/pi; //направление штанги первой оси

 Axis2=90-acos((A2lever*A2lever+A2offset*A2offset-L2*L2)/(2*A2lever*A2offset))*180/pi-Axis1;
 A2motor=asin(A2lever*sin((90-Axis2-Axis1)*pi/180)/L2)*180/pi; //направление штанги второй оси

 float g=sqrt(toolpos[0]*toolpos[0]+toolpos[1]*toolpos[1]);
 float psi=atan(toolpos[1]/toolpos[0]);

 x=g*cos((Axis1+Axis2+Axis3)*pi/180+psi)-Arm1*cos((Axis3+Axis2)*pi/180)-Arm2*cos(Axis3*pi/180);
 x=-g*cos((Axis1+Axis2+Axis3)*pi/180-pi-psi)-Arm1*cos((Axis3+Axis2)*pi/180)-Arm2*cos(Axis3*pi/180);
 xc=x;
 y=g*sin((Axis1+Axis2+Axis3)*pi/180+psi)+Arm1*sin((Axis3+Axis2)*pi/180)+Arm2*sin(Axis3*pi/180);
 y=g*sin((Axis1+Axis2+Axis3)*pi/180-pi-psi)+Arm1*sin((Axis3+Axis2)*pi/180)+Arm2*sin(Axis3*pi/180);
 yc=y;
}

void doInverse2(){
 float g=sqrt(toolpos[0]*toolpos[0]+toolpos[1]*toolpos[1]);
 float psi=atan(toolpos[1]/toolpos[0]);
 float lr2=x*x+y*y;
 float lAxis2=acos((-lr2+Arm1*Arm1+Arm2*Arm2)/(2*Arm1*Arm2));
 float lAxis3=atan(y/x)-lAxis2;
 float lAxis1=0;

 lAxis2*=180/pi;
 lAxis3*=180/pi;

 Axis3=lAxis3;
 Axis2=lAxis2;
 L1=sqrt(A1offset*A1offset+A1lever*A1lever-2*A1offset*A1lever*cos((180-lAxis1)*pi/180));
 L2=sqrt(A2offset*A2offset+A2lever*A2lever-2*A2offset*A2lever*cos((90-lAxis2-lAxis1)*pi/180));
// curpt++;
}


void draw2(){

 L1+=dL1*1;
 L2+=dL2*1;
 Axis3+=dA3*0.15;

 doCalc2();

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
 sprintf(lbuf,"%i",curpt);
 SetWindowText(hZactual,lbuf);

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

//glRotatef(Axis1+Axis2+Axis3,0,0,1);

 glDisable(GL_LIGHTING);
 glColor3f(1,1,0);
 glBegin(GL_LINES);
 for(int i=1;i<curpt;i++){
  glVertex3f(trajc[i-1][0],trajc[i-1][1],1300);
  glVertex3f(trajc[i][0],trajc[i][1],1300);
 }
 glEnd();



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