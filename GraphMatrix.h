// Filename:	GraphMatrix.h
// Author:	EVGENI C. DOBRANOV
// Date:	4.19.2016

#ifndef GraphMatrix_h
#define GraphMatrix_h

#include <string>
#include <iostream>

using namespace std;

class GraphMatrix
{
	public:
		GraphMatrix();
		~GraphMatrix();
		string &operator()(int a, int b) const;
		
		int getheight() { return height; }
		int getwidth() { return width; }
		
		void add(string vertex, string edge);
		
		string * lv();
		string * le();
		void rowmajor(string vertex);
		void colmajor(string edge);
		
		bool exists(string vertex, string edge);
	
	private:
		static const int INITIAL_SIZE = 4;
		
		int width;
		int height;
		
		int widthcap;
		int heightcap;
		
		string **matrix;
		void expandwidth();
		void expandheight();
};

#endif