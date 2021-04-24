/* ZJW simple C++ code to write out a PPM file representing an ellipse(s) */

#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <map>
#include "ppmR.h"
#include "windmill.h"

using namespace std;


/*write out PPM data, using the defined implicit equation 
  interior points write a differnt color then exterior points */

void writeOut(ostream& out, ppmR& theWriter,  
				vector<shared_ptr<ellipse>> IEs, 
				vector<shared_ptr<Rect> > Rs,
				vector<shared_ptr<Polygon> > Ps) {

	float res;
	color inC;
	color drawC;
	color background(12, 34, 56);
	bool inTrue = false;
	double curDepth = -1.0;

	//for every point in the 2D space
	for (int y=0; y < theWriter.height; y++) {
		for (int x =0; x < theWriter.width; x++) {

			inTrue = false;
			curDepth = -1;
			//iterate through all possible equations (note 'front' determined by order in vector)

			for (auto eq : IEs) {
				res = eq->eval(x, y);
				if (res < 0 && eq->getDepth() > curDepth) {
					inC = eq->getInC();
					inTrue = true;
					curDepth = eq->getDepth();
				}
			}

			for (auto rect: Rs) {
				if (rect->evalIn(x, y) && rect->getDepth() > curDepth){
					inC = rect->getInC();
					inTrue = true;
					curDepth = rect->getDepth();
				}
			}

			for (auto poly:Ps) {
				if (poly->eval(x, y) && poly->getDepth() > curDepth){
					inC = poly->getInC();
					inTrue = true;
					curDepth = poly->getDepth();
				}
			}

			if (inTrue) {			
				theWriter.writePixel(out, x, y, inC);
			}
			else
				theWriter.writePixel(out, x, y, background);
		}
		theWriter.writeNewLine(out);
	}
}


/*read command line arguments and write out new ascii file of given size */
int main(int argc, char *argv[]) {

	ofstream outFile;
	int sizeX, sizeY;

	map<string , shared_ptr<color>> Colors;

	Colors["red"] = make_shared<color>(255,0,0);
	Colors["blue"] = make_shared<color>(0,255,0);
	Colors["green"] = make_shared<color>(0,0,255);
	Colors["orange"] = make_shared<color>(255,177,59);
	Colors["yellow"] = make_shared<color>(235,230,57);

	// create all the equations for a simple asci face
	// layer values larger is closer - ie on top
	/* 
	vector<shared_ptr<ellipse>> theEllipses;
	theEllipses.push_back(make_shared<ellipse>(150,50,75,20,*Colors["red"]));
	theEllipses.push_back(make_shared<ellipse>(150,100,90,15,2, *Colors["red"]));
	theEllipses.push_back(make_shared<ellipse>(300,300,50,50, *Colors["yellow"]));
	theEllipses.push_back(make_shared<ellipse>(0,0,50,50, *Colors["yellow"]));
	theEllipses.push_back(make_shared<ellipse>(300,0,50,50, *Colors["green"]));
	theEllipses.push_back(make_shared<ellipse>(0,300,50,50, *Colors["green"]));
	
	//collection of rectangles (empty in base)
	
	vector<shared_ptr<Rect>> theRects;
	theRects.push_back(make_shared<Rect>(150,150,200,200, *Colors["orange"], 1));
	theRects.push_back(make_shared<Rect>(150,200,150,50, *Colors["blue"], 2)); 
	*/

	vector<shared_ptr<ellipse>> theEllipses;

	vector<shared_ptr<Rect>> theRects;
	
	vector<shared_ptr<Polygon>> thePolys;
	vector<vec2> trapVerts;
	//trapVerts.push_back(vec2(sizeX/2 + 100));

	Windmill a = Windmill(300,300, vec2(150,150),2);
	//a.getPolys()[0]->print();
	for(int i=0; i < a.getPolys().size(); i++){
		thePolys.push_back(a.getPolys().at(i));
	}

	if (argc < 4) {
		cerr << "Error format: a.out sizeX sizeY outfileName" << endl;
	} else {
		sizeX = stoi(argv[1]);
		sizeY = stoi(argv[2]);
		ppmR theWriter(sizeX, sizeY);
		outFile.open(argv[3]);
		if (outFile) {
			cout << "writing an image of size: " << sizeX << " " << sizeY << " to: " << argv[3] << endl;
			theWriter.writeHeader(outFile);
			writeOut(outFile, theWriter, theEllipses, theRects, thePolys);
		} else {
			cout << "Error cannot open outfile" << endl;
			exit(0);
		}
	}

}
