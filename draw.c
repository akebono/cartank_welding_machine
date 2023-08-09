//#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <math.h>

#define POINTS 10000000

int dragstart[2],drag=0,dragt=0,rott=0,dragtstart;
float rot[2]={0,0};
float zoom=0.25;
float zoom_d=1;
float shift[3];
float tx=10,ty=6,tz=0;
float tx_d=0,ty_d=0;

float lipos[4]={-1000,2000,4000,1};
float lipar[4]={1,1,1,1};
float mat[4]={1,1,1,1};

unsigned int t=0;
float t0=0;
float delta=0.01;
char task=0;
float pi=3.141592654;
float angle=0;

float Axis1=90,Axis2=-90,Axis3=0;
float dL1=0,dL2=0,dA3=0;

float x,y,z=0,c,xc,yc,zc=0,oldx,oldy,oldz;
float velReceived;
unsigned int status;
unsigned int sernum=1;
unsigned int sernumback=0;

double dx=0,dy=0,d3=0;
float xe,ye,ce,a3e,a3a;
float traj[POINTS][3];
float trajc[POINTS][3];
int curpt=0;

struct point{
 unsigned char type;
 float x;
 float y;
 float c;
 float xa;
 float ya;
 float ca;
 float vel;
};

struct point trajectory[256];
int trajectoryLength=0;
int currentPoint=-1;
float A1offset=600;
float A2offset=600;

float A1lever=600;
float A2lever=853;

float Arm1=2000;
float Arm2=2000;

float L1=830,L2=1030;
float A1motor,A2motor;

char focus=0;

int pnum;

int selected_point=-1;
unsigned char **blobs;
int curblob=0;

char inCirc=0;
float cr,cx,cy,cphi,cd;



float rollerY=6179.4,rollerX=10100;
//
void doCalc(){
  Axis1=180-acos((A1lever*A1lever+A1offset*A1offset-L1*L1)/(2*A1lever*A1offset))*180/pi;

  A1motor=asin(A1lever*sin((180-Axis1)*pi/180)/L1)*180/pi; //направление штанги первой оси

  Axis2=90-acos((A2lever*A2lever+A2offset*A2offset-L2*L2)/(2*A2lever*A2offset))*180/pi-Axis1;
  A2motor=asin(A2lever*sin((90-Axis2-Axis1)*pi/180)/L2)*180/pi; //направление штанги второй оси

  xc=Arm2*cos(Axis2*pi/180+Axis1*pi/180)+Arm1*cos(Axis1*pi/180);
  x=xc;
  yc=Arm2*sin(Axis2*pi/180+Axis1*pi/180)+Arm1*sin(Axis1*pi/180);
  y=yc;
  c=Axis1+Axis2+Axis3;
//  y=yc-tank_edge;
}


void doInverse(){
// float ly=y+tank_edge;
 float ly=y;
 float lAxis1=atan(ly/x)+acos((Arm1*Arm1+x*x+ly*ly-Arm2*Arm2)/(2*Arm1*sqrt(x*x+ly*ly)));

 lAxis1*=180/pi;
Axis1=lAxis1;
 float lAxis2=acos((Arm1*Arm1+Arm2*Arm2-x*x-ly*ly)/(2*Arm1*Arm2));
 lAxis2*=180/pi;
 lAxis2=180-lAxis2;
 lAxis2=-lAxis2;
Axis2=lAxis2;
 L1=sqrt(A1offset*A1offset+A1lever*A1lever-2*A1offset*A1lever*cos((180-lAxis1)*pi/180));
 L2=sqrt(A2offset*A2offset+A2lever*A2lever-2*A2offset*A2lever*cos((90-lAxis2-lAxis1)*pi/180));
 curpt++;
}
void doLine(){

 x+=dx;
 y+=dy;
if(dx>0 && x>xe
 ||dx<0 && x<xe
 ||dy>0 && y>ye
 ||dy<0 && y<ye){
  task=0;
//  x=xe;
//  y=ye;
}

//printf("x=%.4f(%f) y=%.4f(%f)\n",x,dx,y,dy);
 doInverse();
// doCalc();

}

void doCirc(){
 x=cr*cos((t0+cphi)*pi/180)+cx;
 y=cr*sin((t0+cphi)*pi/180)+cy;

 doInverse();
 if((t0+cphi)>cd)
  inCirc=0;
}

void draw(){
  L1+=dL1*1;
  L2+=dL2*1;

 if(dL1 || dL2 ||dA3)
  curpt++;


/*
  if(Axis2>-30)
   Axis2=-30;
  if(Axis2<-145)
   Axis2=-145;
*/
  Axis3+=dA3*0.15;
  if(task){
   t0+=delta;
   doLine();
  }

  if(inCirc){
   doCirc();
   t0+=delta;
  }

  doCalc();

  char lbuf[256];
  sprintf(lbuf,"X:%.2f mm",x);
  SetWindowText(hXactual,lbuf);
  sprintf(lbuf,"Y:%.2f mm",y);
  SetWindowText(hYactual,lbuf);
  sprintf(lbuf,"C: %.2f deg",c);
  SetWindowText(hZactual,lbuf);
  sprintf(lbuf,"Vel: %.2f",velReceived);
  SetWindowText(hVelactual,lbuf);


  sprintf(lbuf,"%.1f / %.3f",Axis1,L1);
  SetWindowText(hA1,lbuf);
  sprintf(lbuf,"%.1f / %.3f",Axis2,L2);
  SetWindowText(hA2,lbuf);
  sprintf(lbuf,"%.1f",Axis3);
  SetWindowText(hA3,lbuf);

  zoom*=zoom_d;
  tx+=tx_d*300;
  ty+=ty_d*300;
//  glOrtho(-w/2*zoom,w/2*zoom,-h/2*zoom,h/2*zoom,-10000*zoom,10000*zoom);
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
traj[curpt][0]=x;
traj[curpt][1]=y;
trajc[curpt][0]=xc;
trajc[curpt][1]=yc;
  glOrtho(-w*40*zoom,w*40*zoom,-h*40*zoom,h*40*zoom,-100000,100000);
/*
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
*/

  glPushMatrix();
  glTranslatef(-tx,-ty,tz);
  glRotatef(rot[1]/1.0,1,0,0);
  glRotatef(rot[0]/1.0,0,1,0);
  glDisable(GL_LIGHTING);

  glColor3f(1,1,0);
  glBegin(GL_LINES);
  for(int i=1;i<curpt;i++){
   glVertex3f(trajc[i-1][0],trajc[i-1][1],1300);
   glVertex3f(trajc[i][0],trajc[i][1],1300);
  }
  glEnd();

  glColor3f(1,1,0);
  glBegin(GL_LINES);
  for(int i=1;i<pnum;i++){
   glVertex3f(trajectory[i-1].x,trajectory[i-1].y,1300);
   glVertex3f(trajectory[i].x,trajectory[i].y,1300);
  }
  glEnd();

  glEnable(GL_LIGHTING);


  glColor3f(0.5,0.5,0.5);
  glBegin(GL_LINES);
  for(int i=0;i<21;i++){
    glVertex3f(-20000,-20000+2000*i,0);
    glVertex3f(20000,-20000+2000*i,0);
    glVertex3f(-20000+2000*i,-20000,0);
    glVertex3f(-20000+2000*i,20000,0);
  }
  glEnd();
  glColor3f(1,0,0);
  glEnable(GL_LIGHTING);

  lipos[0]=3000;
  lipos[1]=3000;
  glLightfv(GL_LIGHT0,GL_POSITION,lipos);

  t++;
  glColor3f(1,1,1);
  mat[0]=0.3;
  mat[1]=0.3;
  mat[2]=0.3;
  glMaterialfv(GL_FRONT,GL_DIFFUSE,mat);
/*
  glBegin(7);
  for(int i=0;i<20;i++){
    for(int j=0;j<20;j++){
      glVertex3f(-5000+i*500,-5000+j*500,0);
      glVertex3f(-4500+i*500,-5000+j*500,0);
      glVertex3f(-4500+i*500,-4500+j*500,0);
      glVertex3f(-5000+i*500,-4500+j*500,0);
    }
  }
  glEnd();
*/
  mat[0]=1;
  mat[1]=1;
  mat[2]=0.3;
  glMaterialfv(GL_FRONT,GL_DIFFUSE,mat);
  glBegin(7);
  glNormal3f(0,-1,0);
  glVertex3f(-20000,-20000,0);
  glVertex3f(20000,-20000,0);
  glVertex3f(20000,-20000,300);
  glVertex3f(-20000,-20000,300);

  glNormal3f(0,0,1);
  glVertex3f(-20000,-20000,300);
  glVertex3f(20000,-20000,300);
  glVertex3f(19500,-19500,300);
  glVertex3f(-19500,-19500,300);

  glNormal3f(0,1,0);
  glVertex3f(19500,-19500,0);
  glVertex3f(-19500,-19500,0);
  glVertex3f(-19500,-19500,300);
  glVertex3f(19500,-19500,300);

  glNormal3f(1,0,0);
  glVertex3f(20000,-20000,0);
  glVertex3f(20000,20000,0);
  glVertex3f(20000,20000,300);
  glVertex3f(20000,-20000,300);

  glNormal3f(0,0,1);
  glVertex3f(20000,-20000,300);
  glVertex3f(20000,20000,300);
  glVertex3f(19500,19500,300);
  glVertex3f(19500,-19500,300);

  glNormal3f(-1,0,0);
  glVertex3f(19500,19500,0);
  glVertex3f(19500,-19500,0);
  glVertex3f(19500,-19500,300);
  glVertex3f(19500,19500,300);

  glNormal3f(0,1,0);
  glVertex3f(20000,20000,0);
  glVertex3f(-20000,20000,0);
  glVertex3f(-20000,20000,300);
  glVertex3f(20000,20000,300);

  glNormal3f(0,0,1);
  glVertex3f(20000,20000,300);
  glVertex3f(-20000,20000,300);
  glVertex3f(-19500,19500,300);
  glVertex3f(19500,19500,300);

  glNormal3f(0,-1,0);
  glVertex3f(-19500,19500,0);
  glVertex3f(19500,19500,0);
  glVertex3f(19500,19500,300);
  glVertex3f(-19500,19500,300);

  glNormal3f(-1,0,0);
  glVertex3f(-20000,20000,0);
  glVertex3f(-20000,-20000,0);
  glVertex3f(-20000,-20000,300);
  glVertex3f(-20000,20000,300);

  glNormal3f(0,0,1);
  glVertex3f(-20000,20000,300);
  glVertex3f(-20000,-20000,300);
  glVertex3f(-19500,-19500,300);
  glVertex3f(-19500,19500,300);

  glNormal3f(1,0,0);
  glVertex3f(-19500,-19500,0);
  glVertex3f(-19500,19500,0);
  glVertex3f(-19500,19500,300);
  glVertex3f(-19500,-19500,300);
  glEnd();

  int j;
  j=8;
  glPushMatrix();
  glTranslatef(0,-A2offset,1000);
  glRotatef(A2motor,0,0,1);
  glBegin(GL_TRIANGLES);
  mat[0]=0.35;
  mat[1]=1;
  mat[2]=0.1;
  mat[3]=1;
  glMaterialfv(GL_FRONT,GL_DIFFUSE,mat);
  for(int i=0;i<*(int*)blobs[j];i++){
    glNormal3f(*(float*)(blobs[j]+4+i*50),*(float*)(blobs[j]+8+i*50),*(float*)(blobs[j]+12+i*50));
    glVertex3f(*(float*)(blobs[j]+16+i*50),*(float*)(blobs[j]+20+i*50),*(float*)(blobs[j]+24+i*50));
    glVertex3f(*(float*)(blobs[j]+28+i*50),*(float*)(blobs[j]+32+i*50),*(float*)(blobs[j]+36+i*50));
    glVertex3f(*(float*)(blobs[j]+40+i*50),*(float*)(blobs[j]+44+i*50),*(float*)(blobs[j]+48+i*50));
  }
  glEnd();
  j=9;

  glPushMatrix();
  glTranslatef(0,76.37,0);
  glScalef(1,L2/sqrt(A2offset*A2offset+A2lever*A2lever),1);
  glBegin(GL_TRIANGLES);
  mat[0]=0.35;
  mat[1]=0.25;
  mat[2]=0.1;
  mat[3]=1;
  glMaterialfv(GL_FRONT,GL_DIFFUSE,mat);
  for(int i=0;i<*(int*)blobs[j];i++){
    glNormal3f(*(float*)(blobs[j]+4+i*50),*(float*)(blobs[j]+8+i*50),*(float*)(blobs[j]+12+i*50));
    glVertex3f(*(float*)(blobs[j]+16+i*50),*(float*)(blobs[j]+20+i*50),*(float*)(blobs[j]+24+i*50));
    glVertex3f(*(float*)(blobs[j]+28+i*50),*(float*)(blobs[j]+32+i*50),*(float*)(blobs[j]+36+i*50));
    glVertex3f(*(float*)(blobs[j]+40+i*50),*(float*)(blobs[j]+44+i*50),*(float*)(blobs[j]+48+i*50));
  }
  glEnd();
  glPopMatrix();

  j=10;
/*
  glPushMatrix();
  glTranslatef(0,1650,0);
  glScalef(1,(1650-L2)/1.6,1);
  glBegin(GL_TRIANGLES);
  mat[0]=0.35;
  mat[1]=0.25;
  mat[2]=0.1;
  mat[3]=1;
  glMaterialfv(GL_FRONT,GL_DIFFUSE,mat);
  for(int i=0;i<*(int*)blobs[j];i++){
    glNormal3f(*(float*)(blobs[j]+4+i*50),*(float*)(blobs[j]+8+i*50),*(float*)(blobs[j]+12+i*50));
    glVertex3f(*(float*)(blobs[j]+16+i*50),*(float*)(blobs[j]+20+i*50),*(float*)(blobs[j]+24+i*50));
    glVertex3f(*(float*)(blobs[j]+28+i*50),*(float*)(blobs[j]+32+i*50),*(float*)(blobs[j]+36+i*50));
    glVertex3f(*(float*)(blobs[j]+40+i*50),*(float*)(blobs[j]+44+i*50),*(float*)(blobs[j]+48+i*50));
  }
  glEnd();
  glPopMatrix();
*/
  glPopMatrix();

  j=7;

  glPushMatrix();
  glTranslatef(0,-A1offset,600);
  glRotatef(A1motor,0,0,1);
  glBegin(GL_TRIANGLES);
  mat[0]=0.35;
  mat[1]=1;
  mat[2]=0.1;
  mat[3]=1;
  glMaterialfv(GL_FRONT,GL_DIFFUSE,mat);
  for(int i=0;i<*(int*)blobs[j];i++){
    glNormal3f(*(float*)(blobs[j]+4+i*50),*(float*)(blobs[j]+8+i*50),*(float*)(blobs[j]+12+i*50));
    glVertex3f(*(float*)(blobs[j]+16+i*50),*(float*)(blobs[j]+20+i*50),*(float*)(blobs[j]+24+i*50));
    glVertex3f(*(float*)(blobs[j]+28+i*50),*(float*)(blobs[j]+32+i*50),*(float*)(blobs[j]+36+i*50));
    glVertex3f(*(float*)(blobs[j]+40+i*50),*(float*)(blobs[j]+44+i*50),*(float*)(blobs[j]+48+i*50));
  }
  glEnd();

  j=9;

  glPushMatrix();
  glScalef(1,L1/sqrt(A1offset*A1offset+A1lever*A1lever),1);
  glBegin(GL_TRIANGLES);
  mat[0]=0.35;
  mat[1]=0.25;
  mat[2]=0.1;
  mat[3]=1;
  glMaterialfv(GL_FRONT,GL_DIFFUSE,mat);
  for(int i=0;i<*(int*)blobs[j];i++){
    glNormal3f(*(float*)(blobs[j]+4+i*50),*(float*)(blobs[j]+8+i*50),*(float*)(blobs[j]+12+i*50));
    glVertex3f(*(float*)(blobs[j]+16+i*50),*(float*)(blobs[j]+20+i*50),*(float*)(blobs[j]+24+i*50));
    glVertex3f(*(float*)(blobs[j]+28+i*50),*(float*)(blobs[j]+32+i*50),*(float*)(blobs[j]+36+i*50));
    glVertex3f(*(float*)(blobs[j]+40+i*50),*(float*)(blobs[j]+44+i*50),*(float*)(blobs[j]+48+i*50));
  }
  glEnd();
  glPopMatrix();

  j=10;
/*
  glPushMatrix();
  glTranslatef(0,1650,0);
  glScalef(1,(1650-L1)/1.6,1);
  glBegin(GL_TRIANGLES);
  mat[0]=0.35;
  mat[1]=0.25;
  mat[2]=0.1;
  mat[3]=1;
  glMaterialfv(GL_FRONT,GL_DIFFUSE,mat);
  for(int i=0;i<*(int*)blobs[j];i++){
    glNormal3f(*(float*)(blobs[j]+4+i*50),*(float*)(blobs[j]+8+i*50),*(float*)(blobs[j]+12+i*50));
    glVertex3f(*(float*)(blobs[j]+16+i*50),*(float*)(blobs[j]+20+i*50),*(float*)(blobs[j]+24+i*50));
    glVertex3f(*(float*)(blobs[j]+28+i*50),*(float*)(blobs[j]+32+i*50),*(float*)(blobs[j]+36+i*50));
    glVertex3f(*(float*)(blobs[j]+40+i*50),*(float*)(blobs[j]+44+i*50),*(float*)(blobs[j]+48+i*50));
  }
  glEnd();
  glPopMatrix();
*/
  glPopMatrix();

  glPushMatrix();
  j=5;
/*
  glBegin(GL_TRIANGLES);
  mat[0]=1;
  mat[1]=1;
  mat[2]=0.1;
  mat[3]=1;
  glMaterialfv(GL_FRONT,GL_DIFFUSE,mat);
  for(int i=0;i<*(int*)blobs[j];i++){
    glNormal3f(*(float*)(blobs[j]+4+i*50),*(float*)(blobs[j]+8+i*50),*(float*)(blobs[j]+12+i*50));
    glVertex3f(*(float*)(blobs[j]+16+i*50),*(float*)(blobs[j]+20+i*50),*(float*)(blobs[j]+24+i*50));
    glVertex3f(*(float*)(blobs[j]+28+i*50),*(float*)(blobs[j]+32+i*50),*(float*)(blobs[j]+36+i*50));
    glVertex3f(*(float*)(blobs[j]+40+i*50),*(float*)(blobs[j]+44+i*50),*(float*)(blobs[j]+48+i*50));
  }
  glEnd();
*/
  j=6;

  glBegin(GL_TRIANGLES);
  mat[0]=0.4;
  mat[1]=0.4;
  mat[2]=0.3;
  mat[3]=1;
  glMaterialfv(GL_FRONT,GL_DIFFUSE,mat);
  for(int i=0;i<*(int*)blobs[j];i++){
    glNormal3f(*(float*)(blobs[j]+4+i*50),*(float*)(blobs[j]+8+i*50),*(float*)(blobs[j]+12+i*50));
    glVertex3f(*(float*)(blobs[j]+16+i*50),*(float*)(blobs[j]+20+i*50),*(float*)(blobs[j]+24+i*50));
    glVertex3f(*(float*)(blobs[j]+28+i*50),*(float*)(blobs[j]+32+i*50),*(float*)(blobs[j]+36+i*50));
    glVertex3f(*(float*)(blobs[j]+40+i*50),*(float*)(blobs[j]+44+i*50),*(float*)(blobs[j]+48+i*50));
  }
  glEnd();

  glTranslatef(0,0,600);
  glPushMatrix();
  glRotatef(Axis2+Axis1,0,0,1);
  j=0;
  glBegin(GL_TRIANGLES);
  mat[0]=0.5;
  mat[1]=0.5;
  mat[2]=1;
  mat[3]=1;
  glMaterialfv(GL_FRONT,GL_DIFFUSE,mat);
  for(int i=0;i<*(int*)blobs[j];i++){
    glNormal3f(*(float*)(blobs[j]+4+i*50),*(float*)(blobs[j]+8+i*50),*(float*)(blobs[j]+12+i*50));
    glVertex3f(*(float*)(blobs[j]+16+i*50),*(float*)(blobs[j]+20+i*50),*(float*)(blobs[j]+24+i*50));
    glVertex3f(*(float*)(blobs[j]+28+i*50),*(float*)(blobs[j]+32+i*50),*(float*)(blobs[j]+36+i*50));
    glVertex3f(*(float*)(blobs[j]+40+i*50),*(float*)(blobs[j]+44+i*50),*(float*)(blobs[j]+48+i*50));
  }
  glEnd();
  glPopMatrix();
  glPushMatrix();
  glRotatef(Axis1,0,0,1);
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
  glTranslatef(Arm1,0,0);
  glRotatef(Axis2,0,0,1);

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


  glTranslatef(Arm2,0,300);
  glRotatef(Axis3,0,0,1);
glDisable(GL_LIGHTING);

 glLineWidth(2);
 glBegin(GL_LINES);
  glVertex3f(0,0,0);
  glVertex3f(0,0,400);
 glEnd();
 glLineWidth(1);

  glBegin(GL_LINES);
  glVertex3f(0,0,400);
  glVertex3f(500,0,500);
  glVertex3f(0,0,400);
  glVertex3f(500,500,500);
 glEnd();

glEnable(GL_LIGHTING);
  j=4;
  mat[0]=0.5;
  mat[1]=0.5;
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


  glPopMatrix();
  glPopMatrix();
  glPopMatrix();
  glPopMatrix();
  glPopMatrix();
}

int load_model(char *modelname){
  int f=open(modelname,O_RDONLY|O_BINARY);
  if(f<0){
    printf("%08X\n",GetLastError());
    return -1;
  }
  struct stat st;
  fstat(f,&st);

  blobs[curblob]=malloc(st.st_size-80);
  int ret=lseek(f,80,0);
  read(f,blobs[curblob],st.st_size-80);
  close(f);
  curblob++;
  return 0;
}
int viewport[4];
void init_opengl(){
  glEnable(GL_DEPTH_TEST);
/*
  glGetIntegerv(GL_VIEWPORT,viewport);
printf("%i %i %i %i\n",viewport[0],viewport[1],viewport[2],viewport[3]);
*/
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-w,w,-h,h,-10000,10000);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);
  glLightfv(GL_LIGHT0,GL_POSITION,lipos);
  lipos[0]=5000;
  lipos[1]=0;
  glLightfv(GL_LIGHT1,GL_POSITION,lipos);
  glLightfv(GL_LIGHT1,GL_DIFFUSE,lipar);

  glMaterialfv(GL_FRONT,GL_DIFFUSE,mat);
  glClearColor(0.9,0.9,0.5,1);
  blobs=malloc(10*sizeof(void*));

  load_model("part1.stl");
  load_model("part2.stl");
  load_model("part3.stl");
  load_model("shtanga.stl");
  load_model("bak.stl");
  load_model("walls.stl");
  load_model("apparatus.stl");
  load_model("motor1.stl");
  load_model("motor1.stl");
  load_model("gofra1A.stl");
  load_model("gofra1B.stl");
}
