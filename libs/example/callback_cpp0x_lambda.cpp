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
#include <boost/thread.hpp>
#include <boost/geometry/algorithms/intersects.hpp>

template<typename Geometry, typename Transformer>
void
transform(Geometry& geometry, Transformer const& transfomer){
	Geometry tmp;
	boost::geometry::transform(geometry, tmp, transfomer);
	boost::geometry::assign(geometry, tmp);
}

int
main(int argc, char* argv[]){
	namespace bg = boost::geometry;

	typedef bg::model::d2::point_xy<float> point_type;
	
	bg::model::box<point_type> b1(point_type(-0.5f, -0.1f), point_type(0.4f, 0.4f));
	bg::model::box<point_type> b2(point_type(-0.6f, -0.1f), point_type(0.2f, 0.3f));
	
	gl::graphics g(argc, argv, 500, 500);
	
	gl::displayfunc(g, [&](){
		glColor3d(1.0f, 0.0f, 0.0f);
		gl::render(b1);
	});
	
	gl::displayfunc(g, [&](){
		if( boost::geometry::intersects(b1, b2) ){
			glColor3d(1.0f, 0.0f, 0.0f);
		}
		else{
			glColor3d(0.0f, 1.0f, 0.0f);
		}
		gl::render(b2);
	});
	
	gl::keyboardfunc(g, [&](unsigned char key, int x, int y){
		namespace bg = boost::geometry;
		typedef bg::model::d2::point_xy<float> point_type;
		typedef bg::strategy::transform::translate_transformer<point_type, point_type> transformer;
		switch(key){
			case 'q': return exit(0);
			case 'h': return transform(b2, transformer(-0.01f, 0.0f));
			case 'j': return transform(b2, transformer(0.0f, -0.01f));
			case 'k': return transform(b2, transformer(0.0f, 0.01f));
			case 'l': return transform(b2, transformer(0.01f, 0.0f));
		}
	});

	gl::idlefunc(g, [&](){
		boost::this_thread::sleep(boost::posix_time::milliseconds(16));
		glutPostRedisplay();
	});
	
	g.run();
	return 0;
}
