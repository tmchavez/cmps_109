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

GLubyte RED[] = {0xFF, 0x00, 0x00};
GLubyte GREEN[] = {0x00, 0xFF, 0x00};
GLubyte BLUE[] = {0x00, 0x00, 0xFF};
cycle_iterator<GLubyte*> colors {RED, GREEN, BLUE};

cycle_iterator<void*> glut_fonts {
   GLUT_BITMAP_TIMES_ROMAN_24,
   GLUT_BITMAP_HELVETICA_18,
   GLUT_BITMAP_9_BY_15,
};


void draw_ellipse (const GLubyte* color, double diam) {
   glBegin (GL_POLYGON);
   glColor3ubv (color);
   const GLfloat delta = 2 * M_PI / 64;
   const GLfloat width = window.width / diam;
   const GLfloat height = window.height / diam;
   const GLfloat xoffset = window.width / 2.0;
   const GLfloat yoffset = window.height / 2.0;
   for (GLfloat theta = 0; theta < 2 * M_PI; theta += delta) {
      GLfloat xpos = width * cos (theta) + xoffset;
      GLfloat ypos = height * sin (theta) + yoffset;
      glVertex2f (xpos, ypos);
   }
   glEnd();
}

void display() {
   glClearColor (211,211,211,0);
   glClear (GL_COLOR_BUFFER_BIT);

   //draw_rectangle (colors[0]);
   draw_ellipse (colors[2], 2.222);
   draw_ellipse (colors[1], 3.003);
   draw_ellipse (colors[0], 6.006);
   //draw_greeting (*greetings, colors[2]);

   glutSwapBuffers();
}


void invert_colors() {
   for (size_t color = 0; color < 3; ++color) {
      for (size_t rgb = 0; rgb < 3; ++rgb) {
         colors[color][rgb] ^= 0xFF;
      }
   }
}

void keyboard (GLubyte key, int, int) {
   enum {BS = 8, LF = 10, CR = 13, ESC = 27, DEL = 127};
   switch (key) {
      case 's': case 'S':
         invert_colors();
         break;
      case 'c': case 'C':
         ++colors;
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


int main (int argc, char** argv) {
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

