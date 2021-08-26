// RGBpixmap.cpp - routines to read a BMP file

/*Texture mapping is a technique that applies an image onto an object's surface as if the image were a decal 
or cellophane shrink-wrap. The image is created in texture space, with an (s, t) coordinate system. A texture 
is a two-dimensional or cube-mapped image and a set of parameters that determine how samples are derived from 
the image.*/

#include "RGBpixmap.h"

typedef unsigned short ushort;
typedef unsigned long ulong;
fstream inf; // global in this file for convenience- input stream

//<<<<<<<<<<<<<<<<<<<<< getShort >>>>>>>>>>>>>>>>>>>>
ushort getShort() //helper function
{ //BMP format uses little-endian integer types get a 2-byte integer stored in little-endian form
	char ic;
	ushort ip;
	inf.get(ic); ip = ic;  //first byte is little one 
	inf.get(ic);  ip |= ((ushort)ic << 8); // or in high order byte
	return ip;
}
//<<<<<<<<<<<<<<<<<<<< getLong >>>>>>>>>>>>>>>>>>>
ulong getLong() //helper function
{  //BMP format uses little-endian integer types get a 4-byte integer stored in little-endian form
	ulong ip = 0;
	char ic = 0;
	unsigned char uc = ic;
	inf.get(ic); uc = ic; ip = uc;  //ip=uc=ic
	inf.get(ic); uc = ic; ip |= ((ulong)uc << 8);
	inf.get(ic); uc = ic; ip |= ((ulong)uc << 16);
	inf.get(ic); uc = ic; ip |= ((ulong)uc << 24);
	return ip;
}
//<<<<<<<<<<<<<<<<<< RGBPixmap:: readBmpFile>>>>>>>>>>>>>
int RGBpixmap::readBMPFile(string fname, bool hasAlpha)
{
	// Read into memory an mRGB image from an uncompressed BMP file.
	// Return 0 on failure, 1 on success
	inf.open(fname.c_str(), ios::in | ios::binary); //read binary char's - opens a file(fname) of type binary
	//c_str() ->returns a pointer to an array that contains a null-terminated sequence of characters
	//ios::binary -> This causes the file to be accessed as a binary file.
	//ios::in -> This indicates that the stream will be used for input

	if (!inf) { cout << " can't open file: " << fname << endl; return 0; }
	int k, row, col, numPadBytes, nBytesInRow;

	// read the file header information
	char ch1, ch2;
	inf.get(ch1);
	inf.get(ch2);						//type: always 'BMP'
	ulong fileSize = getLong();
	ushort reserved1 = getShort();	// always 0
	ushort reserved2 = getShort();	// always 0 
	ulong offBits = getLong();	// offset to image - unreliable
	ulong headerSize = getLong();	// always 40
	ulong numCols = getLong();	// number of columns in image
	ulong numRows = getLong();	// number of rows in image
	ushort planes = getShort();	// always 1 
	ushort bitsPerPixel = getShort();	//8 or 24; allow 24 here -> 24-bit(RGB=8*3) image(a true color image) => 16M colors
	ulong compression = getLong();	// must be 0 for uncompressed 
	ulong imageSize = getLong();	// total bytes in image 
	ulong xPels = getLong();	// always 0 
	ulong yPels = getLong();	// always 0 
	ulong numLUTentries = getLong();	// 256 for 8 bit, otherwise 0 
	ulong impColors = getLong();	// always 0 

	if (bitsPerPixel != 24)
	{
		// error - must be a 24 bit uncompressed image
		cout << "not a 24 bit/pixelimage, or is compressed!\n";
		inf.close();
		return 0;
	}

	// Add bytes at end of each row so total # is a multiple of 4; => 1 row= 4 bytes
	// round up 3*numCols to next mult. of 4

	nBytesInRow = ((3 * numCols + 3) / 4) * 4;
	numPadBytes = nBytesInRow - 3 * numCols; // number of padding bytes required

	/*Because 24 bits is an odd number of bytes (3) and for a variety of reasons all the image rows are 
	required to start at an address which is a multiple of 4 bytes.
	The bits representing the bitmap pixels are packed in rows. The size of each row is rounded up to a 
	multiple of 4 bytes (a 32-bit DWORD) by padding. Padding bytes (not necessarily 0) must be appended to
	the end of the rows in order to bring up the length of the rows to a multiple of four bytes.*/

	nRows = numRows; // set class's(RGBpixmap class) data members
	nCols = numCols;
	pixel = new mRGB[nRows * nCols]; //make space for array
	if (!pixel)
		return 0; // out of memory!

	long count = 0;
	char dum;
	for (row = 0; row < nRows; row++) // read pixel values
	{
		for (col = 0; col < nCols; col++)
		{
			char r, g, b;
			inf.get(b); //because of little-endian
			inf.get(g);
			inf.get(r); //read bytes
			pixel[count].r = r; //place them in colors
			pixel[count].g = g;
			pixel[count].b = b;
			if ((hasAlpha) && (r == -1) && (g == -1) && (b == -1)) //if rgb==255 then a=0 else a=255
				pixel[count++].a = 0;
			else
				pixel[count++].a = 255;
		}
		for (k = 0; k < numPadBytes; k++) //skip pad bytes at row's end
			inf >> dum;
	}
	inf.close();
	return 1; // success
}

void RGBpixmap::setTexture(GLuint textureName)
{
	glBindTexture(GL_TEXTURE_2D, textureName); 
	
	//glBindTexture - bind a named texture to a texturing target , textureName=texBufferID
	//void glBindTexture(GLenum target,GLuint texture);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	
	//void glTexParameteri(GLenum target,GLenum pname,GLint param);
	/*
	#target-Specifies the target texture of the active texture unit, which must be either GL_TEXTURE_2D or GL_TEXTURE_CUBE_MAP.
	#pname-Specifies the symbolic name of a single - valued texture parameter.pname can be one of the following : GL_TEXTURE_MIN_FILTER, GL_TEXTURE_MAG_FILTER, GL_TEXTURE_WRAP_S, or GL_TEXTURE_WRAP_T.
	#param-Specifies the value of pname. 
	*/
	/*GL_TEXTURE_MIN_FILTER- 
		The texture minifying function is used whenever the pixel being textured maps to 
		an area greater than one texture element. */
		/*params -supplies a function for minifying the texture as one of the following:
		GL_NEAREST - Returns the value of the texture element that is nearest to the center of the pixel being textured.
		GL_LINEAR - Returns the weighted average of the four texture elements that are closest to the center of the pixel being textured.*/
	
	/*GL_TEXTURE_MAG_FILTER-
		The texture magnification function is used when the pixel being textured maps to an area less than or 
		equal to one texture element.It sets the texture magnification function to either GL_NEAREST or 
		GL_LINEAR(see below).GL_NEAREST is generally faster than GL_LINEAR, but it can produce textured images 
		with sharper edges because the transition between texture elements is not as smooth.The initial value 
		of GL_TEXTURE_MAG_FILTER is GL_LINEAR. */
		/*GL_NEAREST- Returns the value of the texture element that is nearest(in Manhattan distance) to the 
		center of the pixel being textured.
		GL_LINEAR- Returns the weighted average of the four texture elements that are closest to the center of 
		the pixel being textured. */

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, nCols, nRows, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixel);

	/* glTexImage2D- specify a two - dimensional texture image */
	
	//void glTexImage2D(GLenum target,GLint level,GLint internalformat,GLsizei width,GLsizei height,GLint border,GLenum format,GLenum type,const void* data);
	
	/* Texturing maps a portion of a specified texture image onto each graphical primitive for which texturing 
	is active.
	Data is read from data(pixel) as a sequence of unsigned bytes, depending on type. When type is 
	GL_UNSIGNED_BYTE, each of the bytes is interpreted as one color component.(texture element-RGB) */
}

