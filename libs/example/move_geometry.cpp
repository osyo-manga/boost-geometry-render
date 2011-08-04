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
#include <iostream>
#include <boost/geometry/algorithms/intersects.hpp>

namespace bg = boost::geometry;

template<typename Geometry, typename Transformer>
void
transform(Geometry& geometry, Transformer const& transfomer){
	Geometry tmp;
	bg::transform(geometry, tmp, transfomer);
	bg::assign(geometry, tmp);
}

template<typename Geometry>
struct move_geometry{
	typedef typename bg::point_type<Geometry>::type point_type;
	typedef bg::strategy::transform::translate_transformer<point_type, point_type> transformer;
	move_geometry(Geometry& geometry) : geometry_(geometry){}
	
	void operator ()(unsigned char key, int x, int y){
		switch(key){
			case 'q': return exit(0);
			case 'h': return transform(geometry_, transformer(-0.01f, 0.0f));
			case 'j': return transform(geometry_, transformer(0.0f, -0.01f));
			case 'k': return transform(geometry_, transformer(0.0f, 0.01f));
			case 'l': return transform(geometry_, transformer(0.01f, 0.0f));
		}
	}
private:
	Geometry& geometry_;
};

template<typename Geometry>
move_geometry<Geometry>
make_move_geometry(Geometry& geometry){
	return move_geometry<Geometry>(geometry);
}

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

	typedef bg::model::d2::point_xy<float> point_type;

	bg::model::box<point_type> b1(point_type(-0.5f, -0.1f), point_type(0.4f, 0.4f));
	
	gl::graphics g(argc, argv, 500, 500);
	
	gl::displayfunc(g, make_geometry_renderer(b1));
	gl::keyboardfunc(g, make_move_geometry(b1));
	gl::idlefunc(g, idle);
	
	g.run();
	return 0;
}
