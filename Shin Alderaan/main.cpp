//  Shin Alderaan
//  Created by Matheus N on 27/05/19.
//-------------------------------------------------------------------------------------------

#include <stdlib.h>
#include <GL/GLUT.h>
#include <fstream>
#include <cmath>
#include <iostream>
#include "Texturas.h"

//GLfloat  fAspect;
int increment = 40;
float theta = 0.0;
int size = 1;
float angle = 45.0, angley = 0.0,
lx = 0.0f, lz = -1.0f, ly = 1.0,
x = 180.0f, y = 170, z = 120.0f,
velocidade = 600.0f,
sol, ter, diater;
/*
 X, Y e Z       - Posição camera
 lx, ly e lz    - Variaveis que armazenam o sen ou cos para incremento no olho da camera
 angle e angley - Angulo para usar o seno  e cos
 velocidade     - Define a velocidade da camera
 sol, merc..    - Variáveis para controlar a rotação dos planetas
 */

GLuint LoadBitmapTexture( const char * filename )
{
  GLuint texture;
  int width, height;
  unsigned char * data;

  FILE * file;
  file = fopen( filename, "rb" );

  if ( file == NULL ) return 0;
  width = 1024;
  height = 512;
  data = (unsigned char *)malloc( width * height * 3 );
  //int size = fseek(file,);
  fread( data, width * height * 3, 1, file );
  fclose( file );

  for(int i = 0; i < width * height ; ++i)
  {
    int index = i*3;
    unsigned char B,R;
    B = data[index];
    R = data[index+2];

    data[index] = R;
    data[index+2] = B;
  }

    glBindTexture(GL_TEXTURE_2D, texture); // Bind the ID texture specified by the 2nd parameter
    // The next commands sets the texture parameters
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // If the u,v coordinates overflow the range 0,1 the image is repeated
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // The magnification function ("linear" produces better results)
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST); //The minifying function
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); // We don't combine the color with the original surface color, use only the texture map.
    // Finally we define the 2d texture
    //glTexImage2D(GL_TEXTURE_2D, 0, 3, infoheader.biWidth, infoheader.biHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    // And create 2d mipmaps for the minifying function
    //gluBuild2DMipmaps(GL_TEXTURE_2D, 3, infoheader.biWidth, infoheader.biHeight, GL_RGB, GL_UNSIGNED_BYTE, data);
    free(data); // Free the memory we used to load the texture
    return (texture); // Returns the current texture OpenGL ID
}
void Sol(void) {
    glPushMatrix();
    GLuint texture = LoadBitmapTexture("2k_sun.bmp", 1);
    GLUquadric *quad;
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    quad = gluNewQuadric();
    gluQuadricTexture(quad, 40);
    gluSphere(quad, 6950.0, 500, 500);
    glPopMatrix();
}
void Luz(void) {
    glColor3f(0.5, 0.5, 0.5);
    GLfloat ambient[] = { 1.0f, 1.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    GLfloat position[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    GLfloat specular[] = { 1.0f, 1.0f, 1.0f , 1.0f };
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
}
void Alderaan(void) {
    glPushMatrix();
    //    glColor3f(0.0, 0.0, 0.7);
    GLuint texture = LoadBitmapTexture("2k_alderaan.bmp", 1);
    GLUquadric *quad;
    //    glColor3f(0.5, 0.5, 0.5);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    quad = gluNewQuadric();
    gluQuadricTexture(quad, 40);
    gluSphere(quad, 63.78, 500, 500);
    glPopMatrix();
    //glutWireSphere(63.78, 500.0, 500.0);
}
void Lua(void) {
    glPushMatrix();
    GLuint texture = LoadBitmapTexture("2k_moon.bmp", 1);
    GLUquadric *quad;
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    quad = gluNewQuadric();
    gluQuadricTexture(quad, 40);
    gluSphere(quad, 17.38, 500, 500);
    glPopMatrix();
    //glutWireSphere(17.38, 500.0, 500.0);
}
void Desenha(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //desenha o fundo (limpa a janela)
    glPushMatrix();
    glRotatef(sol, 0.0, 1.0, 0.0);
    //glColor3f(1.0, 1.0, 1.0);
    Sol();
    glPopMatrix();
    Luz();
    glPushMatrix();
    glRotatef(ter, 0.0, 1.0, 0.0);
    glTranslatef(149600.0, 0.0, 0.0);
    glRotatef(diater, 0.0, 1.0, 0.0);
    glRotatef(180, 1.0, 1.0, 1.0);
    Alderaan();
    glRotatef(ter, 0.0, 1.0, 0.0);
    glTranslatef(384.4, 0.0, 0.0);
    Lua();
    glPopMatrix();
    if(MojaveWorkAround)
    {
        glutReshapeWindow(2 * size,2 * size);//Necessary for Mojave. Has to be different dimensions than in glutInitWindowSize();
        MojaveWorkAround = false;
    }
    glutSwapBuffers();
    glutPostRedisplay();//Necessary for Mojave.
    glFlush();
}
// Inicializa parâmetros de rendering
void Inicializa(void)
{
    // Habilita a definição da cor do material a partir da cor corrente
    glEnable(GL_COLOR_MATERIAL);
    ///Habilita o uso de iluminação
    glEnable(GL_LIGHTING);
    // Habilita a luz de número 0
    glEnable(GL_LIGHT0);
    // Habilita o depth-buffering
    glEnable(GL_DEPTH_TEST);
}
// Função usada para especificar o volume de visualização
void EspecificaParametrosVisualizacao(void)
{
    // Especifica sistema de coordenadas de projeção
    glMatrixMode(GL_PROJECTION);
    // Inicializa sistema de coordenadas de projeção
    glLoadIdentity();
    // Especifica a projeção perspectiva
    gluPerspective(angle, fAspect, 0.1, 10000000000000);
    // Especifica sistema de coordenadas do modelo
    glMatrixMode(GL_MODELVIEW);
    // Inicializa sistema de coordenadas do modelo
    glLoadIdentity();
    gluLookAt(
              x, y, z,
              x + lx, y + ly, z + lz,
              0.0f, 1.0f, 0.0f
              );
}
// Função callback chamada quando o tamanho da janela é alterado
void AlteraTamanhoJanela(GLsizei w, GLsizei h)
{
    // Para previnir uma divisão por zero
    if (h == 0) h = 1;
    // Especifica o tamanho da viewport
    glViewport(0, 0, w, h);
    // Calcula a correção de aspecto
    fAspect = (GLfloat)w / (GLfloat)h;
    EspecificaParametrosVisualizacao();
}
void Timer(int value) {
    sol += 0.1;
    ter += 0.01;
    diater += 30.0;
    EspecificaParametrosVisualizacao();
    glutPostRedisplay();
    glutTimerFunc(33, Timer, 1);
}
void MenuPlanetas(int op)
{
    switch (op) {
        case 0:
            x = 149600.0;
            z = 0.0;
            y = 50;
            break;
        case 1:
            x = 384.4;
            z = 0.0;
            y = 50;
            break;
    }
    EspecificaParametrosVisualizacao();
    glutPostRedisplay();
}
void MenuVelocidades(int op)
{
    switch (op) {
        case 0:
            velocidade = 10.0f;
            break;
        case 1:
            velocidade = 30.0f;
            break;
        case 2:
            velocidade = 100.0f;
            break;
        case 3:
            velocidade = 365.0f;
            break;
        case 4:
            velocidade = 600.0f;
            break;
    }
    glutPostRedisplay();
}
void MenuPrincipal(int op) {
}
void CriaMenu()
{
    int menu, submenu1, submenu2;
    submenu1 = glutCreateMenu(MenuPlanetas);
    glutAddMenuEntry("Alderaan", 0);
    glutAddMenuEntry("Lua", 1);
    submenu2 = glutCreateMenu(MenuVelocidades);
    glutAddMenuEntry("Baixa", 0);
    glutAddMenuEntry("Normal", 1);
    glutAddMenuEntry("Alta", 3);
    glutAddMenuEntry("Velocidade da Luz", 4);
    glutAddMenuEntry("Acima da Velocidade da Luz", 5);
    menu = glutCreateMenu(MenuPrincipal);
    glutAddSubMenu("Planetas", submenu1);
    glutAddSubMenu("Velocidade", submenu2);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}
// Função callback chamada para gerenciar eventos do mouse
void GerenciaMouse(int button, int state, int a, int y)
{
    if (button == GLUT_LEFT_BUTTON)
        if (state == GLUT_UP) {
            glutTimerFunc(10, Timer, 1);
        }
    if (button == GLUT_RIGHT_BUTTON)
        if (state == GLUT_UP) {
            CriaMenu();
        }
    EspecificaParametrosVisualizacao();
    glutPostRedisplay();
}
void GerenciaTeclado(unsigned char key, int x, int y) {
    switch (key) {
        case 'W':
        case 'w':// muda a cor corrente para vermelho
            //    glColor3f(1.0f, 0.0f, 0.0f);
            angley += 0.05f;
            ly = sin(angley);
            break;
        case 'S':
        case 's':// muda a cor corrente para azul
            //    glColor3f(0.0f, 0.0f, 1.0f);
            angley -= 0.05f;
            ly = sin(angley);
            break;
        case 'A':
        case 'a':// muda a cor corrente para verde
            angle -= 0.05f;
            lx = sin(angle);
            lz = -cos(angle);
            break;
            break;
        case 'D':
        case 'd':// muda a cor corrente para azul
            angle += 0.05f;
            lx = sin(angle);
            lz = -cos(angle);
            break;
        case 'Z':
        case 'z':// muda a cor corrente para azul
            break;
        case 'C':
        case 'c':// muda a cor corrente para azul
            break;
    }
    EspecificaParametrosVisualizacao();
    glutPostRedisplay();
}
void GerenciaMovimento(int key, int a, int g) {
    
    switch (key) {
        case GLUT_KEY_LEFT:
            
        case GLUT_KEY_RIGHT:
            
        case GLUT_KEY_UP:
            x += lx * velocidade;
            z += lz * velocidade;
            y += ly * velocidade;
            break;
        case GLUT_KEY_DOWN:
            x -= lx * velocidade;
            z -= lz * velocidade;
            y -= ly * velocidade;
            break;
    }
    EspecificaParametrosVisualizacao();
    glutPostRedisplay();
}
// Programa Principal
int main(int argc, char **argv)
{
    size = 10 * increment;
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(size,size);
    glutCreateWindow("Sistema Solar");
    glutDisplayFunc(Desenha);
    glEnable(GL_DEPTH_TEST | GL_POINT_SMOOTH);
    glutReshapeFunc(AlteraTamanhoJanela);
    glutMouseFunc(GerenciaMouse);
    glutKeyboardFunc(GerenciaTeclado);
    glutSpecialFunc(GerenciaMovimento);
    glPointSize(20.0);
    //glColor4f(1.0,0.0,0.0,1.0);
    Inicializa();
    glutMainLoop();
    return 0;
}




