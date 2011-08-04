//
// kmt-library
//
// Copyright (c) 2011 
// osyo-manga : http://d.hatena.ne.jp/osyo-manga/
//
// License:
// Boost Software License - Version 1.0
// <http://www.boost.org/LICENSE_1_0.txt>
//
#ifndef BOOST_GEOMETRY_RENDER_GL_DETAIL_C_FUNCTION_H
#define BOOST_GEOMETRY_RENDER_GL_DETAIL_C_FUNCTION_H

#include <boost/function_types/result_type.hpp>
#include <boost/function_types/parameter_types.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/at.hpp>
#include <boost/preprocessor/iteration/local.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/enum.hpp>
#include <boost/preprocessor/enum_shifted.hpp>


namespace gl{

namespace detail{

template<typename UUID, typename F, typename R, typename Args, int Argc>
struct c_function;

#ifndef C_FUNCTIONA_ARGS_MAX_ARITY
#define C_FUNCTIONA_ARGS_MAX_ARITY 10
#endif

#define ARGS_AT_C(z, n, key) \
	typename boost::mpl::at_c<Args, n>::type BOOST_PP_CAT(a, n)

#define BOOST_PP_LOCAL_MACRO(n)                                   \
template<typename UUID, typename F, typename R, typename Args>    \
struct c_function<UUID, F, R, Args, n>{                           \
	static R apply(                                               \
		BOOST_PP_ENUM(n, ARGS_AT_C, ~)                    \
	){                                                            \
		return func_( BOOST_PP_ENUM_PARAMS(n, a) );               \
	}                                                             \
	static F func_;                                               \
};                                                                \
template<typename UUID, typename F, typename R, typename Args>    \
F c_function<UUID, F, R, Args, n>::func_;

#define BOOST_PP_LOCAL_LIMITS (0, C_FUNCTIONA_ARGS_MAX_ARITY)
#include BOOST_PP_LOCAL_ITERATE()
#undef ARGS_AT_C

template<typename UUID, typename func_type, typename F>
func_type
make_c_function(F f){
	typedef c_function<UUID, F, 
		typename boost::function_types::result_type<func_type>::type,
		typename boost::function_types::parameter_types<func_type>::type,
		boost::mpl::size<boost::function_types::parameter_types<func_type>::type>::value
	> c_function_type;
	c_function_type::func_ = f;
	return &c_function_type::apply;
}

}  // namespace detail

} // namespace gl

#endif /* BOOST_GEOMETRY_RENDER_GL_DETAIL_C_FUNCTION_H */
