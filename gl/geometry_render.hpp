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
#ifndef BOOST_GEOMETRY_RENDER_GL_GEOMETRY_RENDER_H
#define BOOST_GEOMETRY_RENDER_GL_GEOMETRY_RENDER_H

#include <boost/geometry/geometry.hpp>
#include <boost/mpl/size_t.hpp>
#include <boost/mpl/map.hpp>
#include <boost/mpl/at.hpp>
#include <boost/geometry/algorithms/convert.hpp>
#include <boost/geometry/geometries/polygon.hpp>

namespace gl{

template<typename T>
void
render(T const& geometry);

namespace detail{

namespace mpl = boost::mpl;
namespace bg  = boost::geometry;

typedef mpl::map<
	mpl::pair<short,  mpl::size_t<GL_SHORT> >,
	mpl::pair<int,    mpl::size_t<GL_INT> >,
	mpl::pair<float,  mpl::size_t<GL_FLOAT> >,
	mpl::pair<double, mpl::size_t<GL_DOUBLE> >
> glVertexPointer_value_types;

template<typename Geometry>
struct glVertexPointer_value_type :
	mpl::at<
		glVertexPointer_value_types,
		typename boost::geometry::coordinate_type<Geometry>::type
	>::type
{};

template<typename Tag>
struct geometry_render;

template<>
struct geometry_render<boost::geometry::polygon_tag>{
	template<typename T>
	static void apply(T const& geometry){
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(bg::dimension<T>::value,
			glVertexPointer_value_type<T>::value, 0, &geometry.outer()[0]);
		glDrawArrays(GL_POLYGON, 0, geometry.outer().size());
		glDisableClientState(GL_VERTEX_ARRAY);
	}
};

template<>
struct geometry_render<boost::geometry::linestring_tag>{
	template<typename T>
	static void apply(T const& geometry){
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(bg::dimension<T>::value,
			glVertexPointer_value_type<T>::value, 0, &geometry[0]);
		glDrawArrays(GL_LINE_LOOP, 0, geometry.size());
		glDisableClientState(GL_VERTEX_ARRAY);
	}
};

template<>
struct geometry_render<boost::geometry::point_tag>{
	template<typename T>
	static void apply(T const& geometry){
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(bg::dimension<T>::value,
			glVertexPointer_value_type<T>::value, 0, &geometry.get<0>());
		glDrawArrays(GL_POINTS, 0, 1);
		glDisableClientState(GL_VERTEX_ARRAY);
	}
};

template<>
struct geometry_render<boost::geometry::multi_point_tag>{
	template<typename T>
	static void apply(T const& geometry){
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(bg::dimension<T>::value,
			glVertexPointer_value_type<T>::value, 0, &geometry[0]);
		glDrawArrays(GL_POINTS, 0, geometry.size());
		glDisableClientState(GL_VERTEX_ARRAY);
	}
};

template<>
struct geometry_render<boost::geometry::box_tag>{
	template<typename T>
	static void apply(T const& geometry){
		bg::model::polygon<typename bg::point_type<T>::type> polygon;
		bg::convert(geometry, polygon);
		return render(polygon);
	}
};

}  // namespace detail

template<typename T>
void
render(T const& geometry){
	if(boost::geometry::num_points(geometry) > 0){
		detail::geometry_render<
			typename boost::geometry::tag<T>::type
		>::apply(geometry);
	}
}

} // namespace gl

#endif /* BOOST_GEOMETRY_RENDER_GL_GEOMETRY_RENDER_H */
