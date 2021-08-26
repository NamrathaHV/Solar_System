// RGBpixmap.h: a class to support working with RGB pixmaps.

#ifndef _RGBPIXMAP //#ifndef/#endif block is only defined once, regardless of how many times the header file is included
#define _RGBPIXMAP

#include<gl/glut.h>
#include <string>
#include <iostream>
#include <fstream>
#include <assert.h> 
using namespace std;

/*The <assert.h> header file of the c std library provides a macro called assert which can be used to verify 
assumptions made by the program and print a diagnostic message if this assumption is false */

/*If expression evaluates to TRUE, assert() does nothing. If exp evalustes to FALSE, assert displays an error 
message on stderr (standard error stream to display error messages and diagnostics ) and abort program execution

Assertion Fail: exp, filename,line number*/



//************************* IntPoint class *************************
class IntPoint
{
	// for 2D points with integer coordinates
public:
	int x, y;
	void set(int dx, int dy) { x = dx; y = dy; } //set function for data members assignment
	void set(IntPoint& p) { x = p.x; y = p.y; } //set function to assign data members using another object
	IntPoint(int xx, int yy) { x = xx; y = yy; } //constructor with 2 parameters
	IntPoint() { x = y = 0; } //constructor with 0 parameters
};

//********************** Point2 class **********************
class Point2
{
	// for 2D points with real coordinates
public:
	float x, y;
	void set(float dx, float dy) { x = dx; y = dy; }
	void set(Point2& p) { x = p.x; y = p.y; }
	Point2(float xx, float yy) { x = xx; y = yy; }
	Point2() { x = y = 0; }
};


//<<<<<<<<<<<<<<<<<<<<<< PolyLine class >>>>>>>>>>>>>>>>>>>>>>>>>
class PolyLine
{
	// a polyline is a num plus an array of points
public:
	int num;
	Point2 pt[80]; //may need larger arrays in some circumstances
	PolyLine() { num = 0; } //constructor
};

// ************************ IntRect class **********************
class IntRect
{
	// a rectangle with integer border values
public:
	int left, top, right, bott;
	IntRect() { left = top = right = bott = 0; } //zero parameter constructor
	IntRect(int l, int t, int r, int b)          //parameter constructor
	{
		left = l; top = t; right = r; bott = b;
	}
	void set(int l, int t, int r, int b)         //set function for data members assignment
	{
		left = l; top = t; right = r; bott = b;
	}
	void set(IntRect& r)                         //set function to assign data members using another object
	{
		left = r.left; top = r.top; right = r.right; bott = r.bott;
	}
};

typedef unsigned char uchar;

class mRGB    // the name RGB is already used by Windows
{
	
public: uchar r, g, b, a;
	  mRGB() { r = g = b = 0; a = 255; }
	  mRGB(mRGB& p) { r = p.r; g = p.g; b = p.b; a = p.a; }
	  mRGB(uchar rr, uchar gg, uchar bb) {
		  r = rr; g = gg; b = bb;
		  if ((r == 255) && (g == 255) && (b == 255)) a = 0; else a = 255;
	  }
	  mRGB(uchar rr, uchar gg, uchar bb, uchar aa) { r = rr; g = gg; b = bb; a = aa; }
	  void set(uchar rr, uchar gg, uchar bb) {
		  r = rr; g = gg; b = bb;
		  if ((r == 255) && (g == 255) && (b == 255)) a = 0; else a = 255;
	  }
	  void set(uchar rr, uchar gg, uchar bb, uchar aa) { r = rr; g = gg; b = bb; a = aa; }
};


//****************** RGBPixmap class *****************
class RGBpixmap
{
private:
	mRGB* pixel; // array of pixels

public:
	int nRows, nCols; // dimensions of the pixmap
	RGBpixmap() { nRows = nCols = 0; pixel = 0; } //default constructor
	RGBpixmap(int rows, int cols) //constructor
	{
		nRows = rows;
		nCols = cols;
		pixel = new mRGB[rows * cols];
	}

	int readBMPFile(string fname, bool hasAlpha); // read BMP file into this pixmap
	void setTexture(GLuint textureName);

	void freeIt() // give back memory for this pixmap
	{
		delete[]pixel; nRows = nCols = 0;
	}

	//<<<<<<<<<<<<<<<<<< copy >>>>>>>>>>>>>>>>>>>
	void copy(IntPoint from, IntPoint to, int x, int y, int width, int height)
	{
		// copy a region of the display back onto the display
		if (nRows == 0 || nCols == 0) return;
		glCopyPixels(x, y, width, height, GL_COLOR); //copy pixels in the frame buffer

		//void glCopyPixels(GLint x,GLint y,GLsizei width,GLsizei height,GLenum type);
		/* 
		   x, y- Specify the window coordinates of the lower left corner of the rectangular region of pixels to be copied.
		   width, height- Specify the dimensions of the rectangular region of pixels to be copied.Both must be nonnegative.
		   type- Specifies whether color values, depth values, or stencil values are to be copied.Symbolic constants GL_COLOR, GL_DEPTH, and GL_STENCIL are accepted.
		*/
		/*glCopyPixels copies a screen-aligned rectangle of pixels from the specified frame buffer location to 
		a region relative to the current raster position. Its operation is well defined only if the entire 
		pixel source region is within the exposed portion of the window. Results of copies from outside the 
		window, or from regions of the window that are not exposed, are hardware dependent and undefined.*/
	}

	//<<<<<<<<<<<<<<<<<<< draw >>>>>>>>>>>>>>>>>
	void draw()
	{
		// draw this pixmap at current raster position
		if (nRows == 0 || nCols == 0) return;

		//tell OpenGL: don’t align pixels to 4 byte boundaries in memory

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1); //set pixel storage modes
		/*
		  void glPixelStorei(GLenum pname,GLint param);
			#pname- Specifies the symbolic name of the parameter to be set.One value affects the packing of pixel data into memory : GL_PACK_ALIGNMENT.The other affects the unpacking of pixel data from memory : GL_UNPACK_ALIGNMENT.
			#param- Specifies the value that pname is set to.
		*/

		glDrawPixels(nCols, nRows, GL_RGBA, GL_UNSIGNED_BYTE, pixel); //write a block of pixels to the frame buffer

		//glDrawPixels- reads pixel data from memory and writes it into the frame buffer relative to the current raster position
		//void glDrawPixels(GLsizei width,GLsizei height,GLenum format,GLenum type,const void* data);
		//#data-Specifies a pointer to the pixel data.
	}

	//<<<<<<<<<<<<<<<<< read >>>>>>>>>>>>>>>>
	int read(int x, int y, int wid, int ht)
	{
		// read a rectangle of pixels into this pixmap
		nRows = ht;
		nCols = wid;
		pixel = new mRGB[nRows * nCols];
		if (!pixel) return -1;

		//tell OpenGL: don’t align pixels to 4 byte boundaries in memory

		glPixelStorei(GL_PACK_ALIGNMENT, 1);
		glReadPixels(x, y, nCols, nRows, GL_RGBA, GL_UNSIGNED_BYTE, pixel); //read a block of pixels from the frame buffer
		//void glReadPixels(GLint x,GLint y,GLsizei width,GLsizei height,GLenum format,GLenum type,void* data);
		return 0;
	}

	//<<<<<<<<<<<<<<<<< read from IntRect >>>>>>>>>>>>>>>>
	int read(IntRect r)
	{
		// read a rectangle of pixels into this pixmap
		nRows = r.top - r.bott;
		nCols = r.right - r.left;
		pixel = new mRGB[nRows * nCols];
		if (!pixel) return -1;

		//tell OpenGL: don’t align pixels to 4 byte boundaries in memory
		glPixelStorei(GL_PACK_ALIGNMENT, 1);
		glReadPixels(r.left, r.bott, nCols, nRows, GL_RGBA, GL_UNSIGNED_BYTE, pixel); //(x,y) => left-bootom point of the rectangle
		return 0;
	}

	//<<<<<<<<<<<<<< setPixel >>>>>>>>>>>>>
	void setPixel(int x, int y, mRGB color) //sets the color in pixel array
	{
		//The SetPixel function sets the pixel at a specified x- and y-coordinate to a particular color
		if (x >= 0 && x < nCols && y >= 0 && y < nRows)
			pixel[nCols * y + x] = color;
	}

	//<<<<<<<<<<<<<<<< getPixel >>>>>>>>>>>
	mRGB getPixel(int x, int y) //getpixel() function returns the color of pixel present at location (x, y)
	{
		mRGB bad(255, 255, 255); //object bad= white color
		assert(x >= 0 && x < nCols);   
		assert(y >= 0 && y < nRows);

		/*If expression evaluates to TRUE, assert() does nothing. If exp evalustes to FALSE, assert displays an error 
		message on stderr (standard error stream to display error messages and diagnostics ) and abort program execution

		Assertion Fail: exp, filename,line number*/
		
		return pixel[nCols * y + x];
	}
}; //end of class RGBpixmap


#endif


