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
#include <boost/geometry/geometries/linestring.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/algorithms/simplify.hpp>
#include <boost/thread.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <boost/phoenix.hpp>
#include <boost/assign.hpp>

int
main(int argc, char* argv[]){
	namespace bg = boost::geometry;
	namespace phx = boost::phoenix;
	
	typedef bg::model::d2::point_xy<float> point_type;
	typedef bg::model::linestring<point_type> linestring_type;
	linestring_type linestring = boost::assign::list_of<point_type>
		(-0.86, -0.87)
		(-0.85, -0.48)
		(-0.49, -0.04)
		(-0.44,  0.41)
		( 0.06,  0.82)
		( 0.51,  0.35)
		( 0.72, -0.11);

	gl::graphics g(argc, argv, 500, 500);
	gl::displayfunc(g,(
		phx::bind(boost::bind(&glColor3f, 1.0f, 0.0f, 0.0f)),
		phx::bind(boost::bind(&glPointSize, 3.0f)),
		phx::bind(gl::render<linestring_type>, phx::ref(linestring))
	));
	
	linestring_type result;
	bg::simplify(linestring, result, 0.05f);
	gl::displayfunc(g,(
		phx::bind(boost::bind(&glColor3f, 1.0f, 1.0f, 0.0f)),
		phx::bind(boost::bind(&glPointSize, 3.0f)),
		phx::bind(gl::render<linestring_type>, phx::ref(result))
	));
	
	g.run();

	return 0;
}
