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
#define BOOST_BIND_ENABLE_STDCALL

#include <cstdlib>
#include <gl/glut.h>
#include <gl/graphics.hpp>
#include <gl/geometry_render.hpp>
#include <boost/geometry/multi/geometries/multi_point.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/thread.hpp>
#include <boost/phoenix.hpp>
#include <boost/bind.hpp>

int
main(int argc, char* argv[]){
	namespace bg = boost::geometry;
	namespace phx = boost::phoenix;
	using phx::arg_names::arg1;
	using phx::arg_names::arg2;
	using phx::arg_names::arg3;
	using phx::arg_names::arg4;
	using phx::local_names::_x;
	using phx::local_names::_y;
	using phx::local_names::_g;
	using phx::local_names::_d;

	typedef bg::model::d2::point_xy<float> point_type;
	typedef bg::model::multi_point<point_type> multi_point_type;
	multi_point_type multi_point;
	
	gl::graphics g(argc, argv, 500, 500);

	gl::displayfunc(g, (
			phx::bind(boost::bind(&glColor3f, 1.0f, 0.0f, 0.0f)),
			phx::bind(boost::bind(&glPointSize, 3.0f)),
			phx::bind(gl::render<multi_point_type>, phx::ref(multi_point))
	));
	gl::idlefunc(g, (
		phx::bind(boost::this_thread::sleep<boost::posix_time::milliseconds>,
			boost::posix_time::milliseconds(16)),
		phx::bind(boost::bind(&glutPostRedisplay))
	));
	gl::mousefunc(g, phx::if_(arg1 == GLUT_LEFT_BUTTON && arg2 == GLUT_DOWN)[
		phx::let(
			_x = arg3 /  250.0f - 1.0f,
			_y = arg4 / -250.0f + 1.0f,
			_g = phx::ref(multi_point)
		)[
			phx::push_back(_g, phx::construct<point_type>(_x, _y))
		]
	]);
	
	g.run();
	return 0;
}
