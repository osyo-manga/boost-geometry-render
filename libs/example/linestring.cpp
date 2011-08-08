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
#include <boost/geometry/geometries/linestring.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <iostream>
#include <boost/thread.hpp>
#include <boost/assign/list_of.hpp>

namespace bg = boost::geometry;

template<typename Geometry>
struct geometry_renderer{
	geometry_renderer(Geometry& geometry) : geometry_(geometry){};
	void operator ()(){
		glColor3f(1.0f, 0.0f, 0.0f);
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

	typedef bg::model::d2::point_xy<float> point_type;

	bg::model::linestring<point_type> linestring;
	linestring = boost::assign::list_of<point_type>
		( 0.2f,  0.8f)
		( 0.7f,  0.6f)
		( 0.7f, -0.3f)
		( 0.4f, -0.5f)
		(-0.4f, -0.6f)
		(-0.8f, -0.3f)
		(-0.6f,  0.2f)
		(-0.4f,  0.4f)
		(-0.2f, -0.3f)
		( 0.1f, -0.1f)
		( 0.2f, 0.8f);

	gl::graphics g(argc, argv, 500, 500);
	gl::displayfunc(g, make_geometry_renderer(linestring));
	gl::idlefunc(g, &idle);
	
	g.run();
	return 0;
}
