#include <cmath>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
using namespace std;

#include <GL/freeglut.h>
#include <libgen.h>

// Characteristics of the window.
struct window {
string name;
   int width {640};
   int height {480};
} window;

template <typename item_t>
struct cycle_iterator {
   const vector<item_t> items;
   size_t curr_item {0};
   cycle_iterator (initializer_list<item_t> list):
               items(vector<item_t> (list)) {}
   const item_t& operator*() { return items[curr_item]; }
   const item_t& operator[] (size_t index) {
      return items[(curr_item + index) % items.size()];
   }
   cycle_iterator& operator++() { 
      curr_item = (curr_item + 1) % items.size();
      return *this;
   }
};

cycle_iterator<string> greetings {
   "Hello, World!",
   "Hodie natus est radici frater.",
   "Goodbye, World!",
};

GLubyte BLACK[] = {255, 255, 255};
GLubyte WHITE[] = {0, 0, 0};
GLubyte BLUE[] = {0, 0, 255};
GLubyte CYAN[] = {0, 255, 255};
GLubyte GREEN[] = {0, 255, 0};
GLubyte RED[] = {255, 0, 0};


cycle_iterator<GLubyte*> colors {BLACK, WHITE};

cycle_iterator<void*> glut_fonts {
   GLUT_BITMAP_TIMES_ROMAN_24,
   GLUT_BITMAP_HELVETICA_18,
   GLUT_BITMAP_9_BY_15,
};


void draw_rectangle (const GLubyte* color, int xpos, int ypos) {
   int xincr = window.width/8;
   int yincr = window.height/8;
   glBegin (GL_POLYGON);
   glColor3ubv (color);
   glVertex2f (xpos, ypos);
   glVertex2f (xpos + xincr, ypos);
   glVertex2f (xpos+xincr, ypos+yincr);
   glVertex2f (xpos, ypos+yincr);
   glEnd();
}


void display() {
   glClearColor (0.15, 0.15, 0.15, 1.0);
   glClear (GL_COLOR_BUFFER_BIT);
   int xincr = window.width/8;
   int yincr = window.height/8;
   int xpos = 0;
   int ypos = 0;
   int rowset = 1;
   for (int j = 0; j<8; j++){
     for(int i =0; i<8; i++){
       draw_rectangle (colors[i+rowset],xpos,ypos);
       xpos = xpos + xincr;
     }
     ypos = ypos + yincr;
     xpos = 0;
     rowset+=1;
   }
   //draw_ellipse (colors[1]);
   //draw_greeting (*greetings, colors[2]);

   glutSwapBuffers();
}


void invert_colors(char fleg) {
  if(fleg == 'k'){
    colors[0][0] = 255;
    colors[0][1] = 255;
    colors[0][2] = 255;
    colors[1][0] = 0;
    colors[1][1] = 0;
    colors[1][2] = 0;
  }else if(fleg == 'b'){
    colors[0][0] = 0;
    colors[0][1] = 255;
    colors[0][2] = 255;
    colors[1][0] = 0;
    colors[1][1] = 0;
    colors[1][2] = 255;
  }else{
    colors[0][0] = 0;
    colors[0][1] = 255;
    colors[0][2] = 0;
    colors[1][0] = 255;
    colors[1][1] = 0;
    colors[1][2] = 0;
  }
}

void keyboard (GLubyte key, int, int) {
   enum {BS = 8, LF = 10, CR = 13, ESC = 27, DEL = 127};
   switch (key) {
      case 'k': case 'K':
         invert_colors('k');
         break;
      case 'b': case 'B':
         invert_colors('b');
         break;
      case 'g': case 'G':
         invert_colors('g');
         break;
      case 'q': case 'Q': 
         exit (EXIT_SUCCESS);
   }
   glutPostRedisplay();
}

void mouse (int button, int state, int, int) {
   if (state == GLUT_DOWN) {
      switch (button) {
         case GLUT_LEFT_BUTTON:
            ++glut_fonts;
            break;
         case GLUT_MIDDLE_BUTTON:
            ++greetings;
            break;
         case GLUT_RIGHT_BUTTON:
            ++colors;
            break;
      }
   }
   glutPostRedisplay();
}


void reshape (int width, int height) {
   window.width = width;
   window.height = height;
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity();
   gluOrtho2D (0, window.width, 0, window.height);
   glMatrixMode (GL_MODELVIEW);
   glViewport (0, 0, window.width, window.height);
   glutPostRedisplay();
}

void print_howto() {
cout << R"(
To cycle the colors: right mouse button or key 'c' or 'C'.
To cycle the fonts: left mouse button or key 'f' or 'F'.
To cycle the greetings: middle mouse button or key 'g' or 'G'.
To invert the colors: key Space, Backspace, Return, or Delete.
To quit: key 'q' or 'Q' or ESC.
)";
}

int main (int argc, char** argv) {
   print_howto();
   window.name = basename (argv[0]);
   glutInit (&argc, argv);
   glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE);
   glutInitWindowSize (window.width, window.height);
   glutCreateWindow (window.name.c_str());
   glutDisplayFunc (display);
   glutReshapeFunc (reshape);
   glutKeyboardFunc (keyboard);
   glutMouseFunc (mouse);
   glutMainLoop();
   return 0;
}

