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
#include <boost/geometry/multi/geometries/multi_point.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/thread.hpp>
#include <boost/phoenix.hpp>


namespace bg = boost::geometry;

template<typename Geometry>
struct geometry_renderer{
	geometry_renderer(Geometry& geometry) : geometry_(geometry){};
	void operator ()(){
		glColor3d(1.0f, 0.0f, 0.0f);
		glPointSize(5.0f);
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

template<typename Geometry>
struct point_adder{
	point_adder(Geometry& geometry) : geometry_(geometry){};
	void operator ()(int button, int state, int x, int y){
		bg::point_type<Geometry>::type point(x / 250.0f - 1.0f, y / -250.0f + 1.0f);
		geometry_.push_back(point);
	}
private:
	Geometry& geometry_;
};

template<typename Geometry>
point_adder<Geometry>
make_point_adder(Geometry& geometry){
	return point_adder<Geometry>(geometry);
}

void
idle(){
	boost::this_thread::sleep(boost::posix_time::milliseconds(16));
	glutPostRedisplay();
}

int
main(int argc, char* argv[]){

	typedef bg::model::d2::point_xy<float> point_type;
	typedef bg::model::multi_point<point_type> multi_point_type;
	multi_point_type multi_point;
	
	gl::graphics g(argc, argv, 500, 500);
	gl::displayfunc(g, make_geometry_renderer(multi_point));
	gl::idlefunc(g, idle);
	gl::mousefunc(g, make_point_adder(multi_point));
	g.run();

	return 0;
}
