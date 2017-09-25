// =============================================================================
// VIZA654/CSCE646 at Texas A&M University
// Project 1
// Created by Nicolas Bain based on Anton Agana's modifications
// of Ariel Chisholm's template
// 09.25.2017
//
// -------- COMPILE INSTRUCTIONS ----------------------------------------------
// This file is supplied with an associated makefile. Put both files in the same
// directory, navigate to that directory from the Linux/mac shell, and type 'make'.
// This will create a program called 'pr01' that you can run by entering
// './pr01' as a command in the shell.
//
// If you are new to programming in Linux, there is an
// excellent introduction to makefile structure and the gcc compiler here:
//
// http://www.cs.txstate.edu/labs/tutorials/tut_docs/Linux_Prog_Environment.pdf
//
// -------- USAGE INSTRUCTIONS ------------------------------------------------
// The program defaults to a light blue image. Below are all available keyboard
// commands:
//
// 'l' => load ppm image. You will be prompted for the name in the shell. 
// 's' => save current bitmap to output.ppm
// '1' => load procedurally generated image 1
// '2' => load procedurally generated image 2
// '3' => load procedurally generated image 3
// '4' => load procedurally generated image 4
// 'q' => Exit/Quit
// =============================================================================

#include <cstdlib>
#include <iostream>
#include <GLUT/glut.h>
#include <fstream>
#include <cassert>
#include <sstream>
#include <string>

using namespace std;

// =============================================================================
// These variables will store the input ppm image's width, height, and color
// =============================================================================
int width, height, max_color;
unsigned char *pixmap;
FILE *input_file, *output_file;
string infilename;



// =============================================================================
// loadPPM()
//
// This function reads and displays a given PPM file
// =============================================================================
void loadPPM(FILE *input_file){
  char c[80];

  //Skip comment lines
  fscanf(input_file, "%s", c);
  while(c[0] == '#'){
    char comment[100];
    fgets(comment, 100, input_file);
    fscanf(input_file, "%s", c);
  }

  //Check format
  if(c[0] == 'P' && c[1] == '6'){
    //File is P6 format. Good to go.
    cout<<"File is a ppm file! Proceeding. . ."<<endl;

    //Skip comment lines
    fscanf(input_file, "%s", c);
    while(c[0] == '#'){
      char comment[100];
      fgets(comment, 100, input_file);
      fscanf(input_file, "%s", c);
    }
    //Read width, height, and max_color values
    width = atoi(c);
    fscanf(input_file, "%d %d", &height, &max_color);
    char a = fgetc(input_file); //Skip newline character that preceeds pixel color values


    
    //Resize window and initialize pixmap
    glutReshapeWindow(width, height);
    pixmap = new unsigned char[width * height * 3];

    //Fill in RGB values
    for(int y = height - 1; y >= 0; y--){
      for(int x = 0; x < width; x++){
        int i = (y * width + x) * 3;
        pixmap[i++] = fgetc(input_file);
        pixmap[i++] = fgetc(input_file);
        pixmap[i] = fgetc(input_file);
      }
    }
  }
  cout<<"Input file has been read and displayed."<<endl;
  fclose(input_file);
}

// =============================================================================
// savePPM()
//
// This function writes the current image to a PPM file with the given name
// =============================================================================
void savePPM(FILE *output_file){
  //Write image properties to file
  fprintf(output_file, "P6\n");
  fprintf(output_file, "%d %d\n", width, height);
  fprintf(output_file, "255\n");

  //Write pixmap to file
  for(int y = height - 1; y >= 0; y--){
    for(int x = 0; x < width; x++){
      int i = (y * width + x) * 3;
      fputc(pixmap[i++], output_file);
      fputc(pixmap[i++], output_file);
      fputc(pixmap[i], output_file);
    }
  }
  cout<<"Successfully saved image to PPM file."<<endl;
  fclose(output_file);
}

// =============================================================================
// setPixels()
//
// This function stores the RGB values of each pixel to "pixmap."
// Then, "glutDisplayFunc" below will use pixmap to display the pixel colors.
// This has 4 options for rendering an image. They are explained below.
// =============================================================================
void setPixels(int v)
{

  height = 800;
  width = 800;

  glutReshapeWindow(width, height);
  pixmap = new unsigned char[width * height * 3];


  for(int y = 0; y < height ; y++) {
    for(int x = 0; x < width; x++) {
      int i = (y * width + x) * 3;

      //Version 0 -- all one color - initial image
      if(v == 0){
        pixmap[i++] = 0; //RED
        pixmap[i++] = 255; //GREEN
        pixmap[i] = 255; //BLUE
      }
      //Version 1 -- Ellipse equations
      if(v == 1){
        pixmap[i++] = (((x*x)/1) + ((y*y)/2) - 1) % 255; 
        pixmap[i++] = (((x*x)/4) + ((y*y)/9) - 1) % 255; 
        pixmap[i] = (((x*x)/16) + ((y*y)/25) - 1) % 255; 
      }
      //Version 2 -- Modified Ellipse equations
      if(v == 2){
        pixmap[i++] = (((x*x)/1) - 1) % 255; 
        pixmap[i++] = (((y*y)/9) - 1) % 255; 
        pixmap[i] = (((x*x)/16) + ((y*y)/25) - 1) % 255; 
      }
      //Version 3 -- Circle equations
      if(v == 3){
        pixmap[i++] = ((x*x) + (y*y) - 4) % 255; 
        pixmap[i++] = ((x*x) + (y*y) - 9) % 255; 
        pixmap[i] = ((x*x) + (y*y) - 16) % 255; 
      }
      //Version 4 -- Circle - Ellipse
      if(v == 4){
        // pixmap[i++] = ((x*x) - 4) % 255; 
        // pixmap[i++] = ((y*y) - 9) % 255; 
        // pixmap[i] = ((x*x) + (y*y) - 16) % 255;
        pixmap[i++] = (((x*x) + (y*y) - 4) - (((x*x)/1) + ((y*y)/2) - 2)) % 255; 
        pixmap[i++] = (((x*x) + (y*y) - 9) - (((x*x)/4) + ((y*y)/9) - 2))% 255; 
        pixmap[i] = (((x*x) + (y*y) - 16) - (((x*x)/16) + ((y*y)/25) - 2)) % 255; 
      }
    }
  }
}

// =============================================================================
// OpenGL Display and Mouse Processing Functions.
//
// You can read up on OpenGL and modify these functions, as well as the commands
// in main(), to perform more sophisticated display or GUI behavior. This code
// will service the bare minimum display needs for most assignments.
// =============================================================================
static void windowResize(int w, int h)
{   
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0,(w/2),0,(h/2),0,1); 
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity() ;
}
static void windowDisplay(void)
{
  glClear(GL_COLOR_BUFFER_BIT);
  glRasterPos2i(0,0);
  glPixelStorei(GL_UNPACK_ALIGNMENT,1);
  glDrawPixels(width, height, GL_RGB, GL_UNSIGNED_BYTE, pixmap);
  glFlush();
}
static void processMouse(int button, int state, int x, int y)
{
  //if(state == GLUT_UP)
  //exit(0);               // Exit on mouse click.
}
void keyboard(unsigned char key, int x, int y){
  switch(key){
    case 'l':
      //load input file
      cout<< "Please enter the file you would like to load:   "<< flush;
      
      while(true){
        getline(cin, infilename);
        cout<<infilename<<endl;
        input_file = fopen(infilename.c_str(), "r");
        if(input_file == NULL){
          perror("Unable to open file");
        }
        if(input_file) break;
        cout<<"Please enter a valid input file name: "<<flush;
      }
      
      loadPPM(input_file);
      glutPostRedisplay();
      break;
    case 's':
      //save current bitmap to ppm file
      output_file = fopen("output.ppm", "w");
      if(output_file == NULL){
        perror("Unable to open file");
      }
      savePPM(output_file);
      break;
    case '0':
      //Render starting image
      setPixels(0);
      glutPostRedisplay();
      break;
    case '1':
      //Render procedurally generated image 1
      setPixels(1);
      glutPostRedisplay();
      break;
    case '2':
      //Render procedurally generated image 2
      setPixels(2);
      glutPostRedisplay();
      break;
    case '3':
      //Render procedurally generated image 3
      setPixels(3);
      glutPostRedisplay();
      break;
    case '4':
      //Render procedurally generated image 4
      setPixels(4);
      glutPostRedisplay();
      break;
    case 'q':
      exit(0);
      break;
  }
}
static void init(void)
{
  glClearColor(1,1,1,1); // Set background color.
}

// =============================================================================
// main() Program Entry
// =============================================================================
int main(int argc, char *argv[])
{

  //initialize the global variables
  width = 800;
  height = 800;


  //Start with all one color
  setPixels(0);

  // OpenGL Commands:
  // Once "glutMainLoop" is executed, the program loops indefinitely to all
  // glut functions.  
  glutInit(&argc, argv);
  glutInitWindowPosition(100, 100); // Where the window will display on-screen.
  glutInitWindowSize(width, height);
  glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
  glutCreateWindow("NHB Project 1");
  init();
  glutReshapeFunc(windowResize);
  glutDisplayFunc(windowDisplay);
  glutMouseFunc(processMouse);
  glutKeyboardFunc(keyboard);
  glutMainLoop();

  return 0; //This line never gets reached. We use it because "main" is type int.
}

