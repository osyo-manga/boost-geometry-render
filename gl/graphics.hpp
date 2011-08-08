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
#ifndef BOOST_GEOMETRY_RENDER_GL_GRAPHICS_H
#define BOOST_GEOMETRY_RENDER_GL_GRAPHICS_H

#include <boost/range/iterator_range_core.hpp>

#include "detail/c_function.hpp"

#include <boost/bind.hpp>
#include <boost/signals2/signal.hpp>
#include <boost/function.hpp>
#include <boost/ref.hpp>

namespace gl{

struct graphics : boost::noncopyable{
	explicit graphics(int argc, char* argv[], int width, int height){
		glutInit(&argc, argv);
		glutInitWindowSize(width, height);
		glutCreateWindow(argv[0]);
		glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	}
	void run(){
		glutMainLoop();
	}
	void render(){
		// 画面クリア処理
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		displayfunc_();
		glutSwapBuffers();
		glFlush();                      // 強制描画
	}
	boost::signals2::signal<void()> displayfunc_;
	boost::signals2::signal<void(int, int)> reshapefunc_;
	boost::signals2::signal<void()> idlefunc_;
	boost::signals2::signal<void(unsigned char, int, int)> keyboardfunc_;
	boost::signals2::signal<void(int, int, int, int)> mousefunc_;
};

void
window_size(int width, int height){
	glutInitWindowSize(width, height);
	glViewport(0, 0, width, height);
}

template<typename F>
void
displayfunc(F f){
	typedef void(*func_type)();
	glutDisplayFunc(
		detail::make_c_function<struct graphics_glutDisplayFunc, func_type>(f)
	);
}

template<typename F>
void
displayfunc(graphics& g, F f){
	displayfunc(boost::function<void()>(boost::bind(&graphics::render, &g)));
	g.displayfunc_.connect(boost::function<void()>(f));
}

template<typename F>
void
reshapefunc(F func){
	typedef void(*func_type)(int, int);
	glutReshapeFunc(
		detail::make_c_function<struct graphics_glutReshapeFunc, func_type>(func)
	);
}

template<typename F>
void
reshapefunc(graphics& g, F f){
	reshapefunc(boost::function<void(int, int)>(boost::ref(g.reshapefunc_)));
	g.reshapefunc_.connect(boost::function<void(int, int)>(f));
}

template<typename F>
void
idlefunc(F f){
	typedef void(*func_type)();
	glutIdleFunc(
		detail::make_c_function<struct graphics_glutIdleFunc, func_type>(f)
	);
}

template<typename F>
void
idlefunc(graphics& g, F f){
	idlefunc(boost::function<void()>(boost::ref(g.idlefunc_)));
	g.idlefunc_.connect(boost::function<void>(f));
}

template<typename F>
void
keyboardfunc(F f){
	typedef void(*func_type)(unsigned char, int, int);
	glutKeyboardFunc(
		detail::make_c_function<struct graphics_glutKeyboardFunc, func_type>(f)
	);
}

template<typename F>
void
keyboardfunc(graphics& g, F f){
	keyboardfunc(boost::function<void(unsigned char, int, int)>(boost::ref(g.keyboardfunc_)));
	g.keyboardfunc_.connect(boost::function<void(usigned char, int, int)>(f));
}

template<typename F>
void
mousefunc(F f){
	typedef void(*func_type)(int, int, int, int);
	glutMouseFunc(
		detail::make_c_function<struct graphics_glutMouseFunc, func_type>(f)
	);
}

template<typename F>
void
mousefunc(graphics& g, F f){
	mousefunc(boost::function<void(int, int, int, int)>(boost::ref(g.mousefunc_)));
	g.mousefunc_.connect(boost::function<void(int, int, int, int)>(f));
}


} // namespace gl

#endif /* BOOST_GEOMETRY_RENDER_GL_GRAPHICS_H */
