/*
Allocore Example: 2D drawing

Description:
This demonstrates how to do 2D drawing by setting up an orthographic projection
matrix.

Author:
Lance Putnam, Feb. 2012
Keehong Youn, 2017
*/

#include "al/core.hpp"
using namespace al;

class MyApp : public App {
public:

	Mesh verts {Mesh::LINE_STRIP};

	void onCreate ()
	{
		// Create a sine wave
		const int N = 128;
		for(int i=0; i<N; ++i){
			float f = float(i)/(N-1);
			verts.vertex(2*f-1, 0.5*sin(f*M_PI*2));
		}
	}

	void onDraw ()
	{
		g.clear(0);
		g.shader(color_shader);
		g.camera(Viewpoint::IDENTITY);
		g.shader().uniform("col0", 1, 1, 1, 1);
		g.draw(verts);
	}
};

int main () {
	MyApp().start();
}