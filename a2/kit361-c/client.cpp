#include <iostream>
#include "client.h"
#include "filereader.h"
#include "drawers.h"
#include "shapeHelper.h"

Client::Client(Drawable *_drawable)
{
    this->_drawable = _drawable;
}


void Client::nextPage() {
    static int pageNumber = 0;
    pageNumber++;
    std::cout << "PageNumber " << pageNumber << std::endl;
	Point2D topLeft(50, 50);
	Point2D botRight(700, 700);
	Pane drawPane(topLeft, botRight);
	
    switch(pageNumber % 8) {
    case 1:
        draw_rect(0, 0, 750, 750, Color::WHITE);
        draw_rect( 50,  50, 700, 700, Color::BLACK);
		draw_page1(drawPane);
        _drawable->updateScreen();   // you must call this to make the display change.
        break;
    case 2:
		draw_rect(0, 0, 750, 750, Color::WHITE);
		draw_rect(50, 50, 700, 700, Color::BLACK);
		draw_page2(drawPane);
		_drawable->updateScreen();  
        break;
    case 3:
		draw_rect(0, 0, 750, 750, Color::WHITE);
		draw_rect(50, 50, 700, 700, Color::BLACK);
		draw_page3(drawPane);
		_drawable->updateScreen();   // you must call this to make the display change.
        break;
    case 4:
		draw_rect(0, 0, 750, 750, Color::WHITE);
		draw_rect(50, 50, 700, 700, Color::BLACK);
		draw_page4(drawPane);
		_drawable->updateScreen();  
		break;
	case 5:
		draw_rect(0, 0, 750, 750, Color::WHITE);
		draw_rect(50, 50, 700, 700, Color::BLACK);
		draw_page5(drawPane);
		_drawable->updateScreen();
		break;
	case 6:
		draw_rect(0, 0, 750, 750, Color::WHITE);
		draw_rect(50, 50, 700, 700, Color::BLACK);
		draw_page6(drawPane);
		_drawable->updateScreen();
		break;
	case 7:
		draw_rect(0, 0, 750, 750, Color::WHITE);
		draw_rect(50, 50, 700, 700, Color::BLACK);
		draw_page7(drawPane);
		_drawable->updateScreen();
		break;
	case 8:
		draw_rect(0, 0, 750, 750, Color::WHITE);
		draw_rect(50, 50, 700, 700, Color::BLACK);
		draw_page8(drawPane);
		_drawable->updateScreen();
		break;
    default:
        draw_rect(0, 0, 750, 750, 0xffffffff);
        draw_rect(400, 400, 700, 700, 0xff00ff40);
        _drawable->updateScreen();
    }
}

void Client::draw_rect(int x1, int y1, int x2, int y2, unsigned int color) {
    for(int x = x1; x<x2; x++) {
        for(int y=y1; y<y2; y++) {
            _drawable->setPixel(x, y, color);
        }
    }
}

void Client::draw_page1(Pane pane)
{
	DDA_Drawer drawer(_drawable);
	ShapeHelper::draw_squaredShiftedTriangleLines(pane, &drawer);
}

void Client::draw_page2(Pane pane)
{
	TriangleDrawer drawer(_drawable);

	ShapeHelper::draw_squaredShiftedTriangle(pane, &drawer);
}

void Client::draw_page3(Pane pane) {
	std::ifstream simpFileStream;
	simpFileStream.open("p3_4.txt");
	SimpReader reader(&simpFileStream);
	reader.Run(pane, _drawable, ColorMode::DepthCue_BW);
}

void Client::draw_page4(Pane pane)
{
	std::ifstream simpFileStream;
	simpFileStream.open("p3_4.txt");
	SimpReader reader(&simpFileStream);
	reader.Run(pane, _drawable, ColorMode::DepthCue_Fix);
}

void Client::draw_page5(Pane pane)
{
	std::ifstream simpFileStream;
	simpFileStream.open("p5.txt");
	SimpReader reader(&simpFileStream);
	reader.Run(pane, _drawable, ColorMode::DepthCue_Fix);
}

void Client::draw_page6(Pane pane)
{
	std::ifstream simpFileStream;
	simpFileStream.open("test1.simp");
	SimpReader reader(&simpFileStream);
	reader.Run(pane, _drawable);
}

void Client::draw_page7(Pane pane)
{
	std::ifstream simpFileStream;
	simpFileStream.open("test2.simp");
	SimpReader reader(&simpFileStream);
	reader.Run(pane, _drawable);
}

void Client::draw_page8(Pane pane)
{
	std::ifstream simpFileStream;
	simpFileStream.open("test3.simp");
	SimpReader reader(&simpFileStream);
	reader.Run(pane, _drawable);
}
