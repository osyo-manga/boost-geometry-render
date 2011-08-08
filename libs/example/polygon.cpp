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
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <iostream>
#include <boost/thread.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/geometry/geometries/linestring.hpp>

namespace bg = boost::geometry;

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

int
main(int argc, char* argv[]){
	typedef bg::model::d2::point_xy<float> point_type;
	bg::model::polygon<point_type> polygon;
	bg::exterior_ring(polygon) = boost::assign::list_of<point_type>
		( 0.0f,  0.5f)
		( 0.5f,  0.0f)
		( 0.1f, -0.1f)
		( 0.0f, -0.5f)
		(-0.1f, -0.1f)
		(-0.5f,  0.0f);
	
	gl::graphics g(argc, argv, 500, 500);
	gl::displayfunc(g, make_geometry_renderer(polygon));
	
	g.run();
	return 0;
}
