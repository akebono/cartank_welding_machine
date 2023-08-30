void draw2(){
  glPushMatrix();
  glRotatef(rot[1]/1.0,1,0,0);
  glRotatef(rot[0]/1.0,0,1,0);
  glTranslatef(-tx+trans[0],-ty-trans[1],tz);
  glDisable(GL_LIGHTING);

  glColor3f(1,1,0);
  glBegin(GL_LINES);
  for(int i=1;i<curpt;i++){
   glVertex3f(trajc[i-1][0],trajc[i-1][1],1300);
   glVertex3f(trajc[i][0],trajc[i][1],1300);
  }
  glEnd();

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
  glColor3f(1,1,1);
  glEnable(GL_LIGHTING);
  
  glPopMatrix();
}