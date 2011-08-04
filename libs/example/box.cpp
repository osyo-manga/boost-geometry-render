//
// boost-geometry-render
//
// Copyright (c) 2011 
// osyo-manga : http://d.hatena.ne.jp/osyo-manga/
//
// License:
// Boost Software License - Version 1.0
// <http://www.boost.org/LICENSE_1_0.txt>
//
#include <cstdlib>
#include <gl/glut.h>
#include <gl/graphics.hpp>
#include <gl/geometry_render.hpp>
#include <boost/geometry/geometries/box.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <iostream>
#include <boost/thread.hpp>

namespace bg = boost::geometry;

typedef bg::model::d2::point_xy<float> point_type;

template<typename Geometry>
struct geometry_renderer{
	geometry_renderer(Geometry& geometry) : geometry_(geometry){};
	void operator ()(){
		glColor3d(1.0f, 0.0f, 0.0f);
		gl::render(geometry_);
	}
private:
	Geometry& geometry_;
};

template<typename Geometry>
geometry_renderer<Geometry>
make_geometry_renderer(Geometry& geometry){
	return geometry_renderer<Geometry>(geometry);
}

void
idle(){
	boost::this_thread::sleep(boost::posix_time::milliseconds(16));
	glutPostRedisplay();
}

int
main(int argc, char* argv[]){
	bg::model::box<point_type> box(point_type(-0.3f, -0.3f), point_type(0.3f, 0.6f));
	
	gl::graphics g(argc, argv, 500, 500);
	
	gl::displayfunc(g, make_geometry_renderer(box));
	
	gl::idlefunc(g, &idle);
	
	g.run();
	return 0;
}
