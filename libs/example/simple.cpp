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
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/thread.hpp>

int
main(int argc, char* argv[]){
	namespace bg = boost::geometry;

	typedef bg::model::d2::point_xy<float> point_type;
	typedef bg::model::multi_point<point_type> multi_point_type;
	typedef bg::model::polygon<point_type> polygon_type;
	multi_point_type multi_point;
	
	// 適当な描画管理クラス
	// arc, argv, width, height
	gl::graphics g(argc, argv, 500, 500);
	
	// glutDisplayFunc にコールバック関数を登録
	// convex_hull の描画
	gl::displayfunc(g, [&](){
		namespace bg = boost::geometry;
		if(multi_point.empty())	return;
		
		// convex_hull の取得
		// multi_point からでは直接取得出来ないので、
		// 一旦 Polygon に変換してから取得を行う
		polygon_type tmp;
		bg::assign_points(tmp, multi_point);
		bg::model::polygon<point_type> hull;
		bg::convex_hull(tmp, hull);
		glColor3f(0.0f, 1.0f, 0.0f);
		// 取得した Polygon の描画
		gl::render(hull);
	});
	
	// multi_point の描画
	// 登録順に描画が行われる
	gl::displayfunc(g, [&](){
		// gl の描画設定
		glColor3f(1.0f, 0.0f, 0.0f);
		glPointSize(5.0f);
		// Geometry の描画
		gl::render(multi_point);
	});
	
	// マウスイベント
	gl::mousefunc(g, [&](int button, int state, int x, int y){
		// 左クリックが押された時のみ頂点を追加する
		if(button != GLUT_LEFT_BUTTON || state != GLUT_DOWN)	return;
		
		// クリックされたウィンドウ座標をスクリーン座標に変換して Geometry に追加
		multi_point.push_back(point_type(x / 250.0f - 1.0f, y / -250.0f + 1.0f));
	});
	
	// キー入力イベント
	gl::keyboardfunc(g, [](unsigned char key, int x, int y){
		if(key == 'q') return exit(0);
	});
	
	// idle
	gl::idlefunc(g, [&](){
		boost::this_thread::sleep(boost::posix_time::milliseconds(16));
		glutPostRedisplay();
	});
	
	// glut のメインループ
	g.run();
	
	return 0;
}
