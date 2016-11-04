#include <iostream>
#include "client.h"
#include "filereader.h"
#include "drawers.h"
#include "shapeHelper.h"

Client::Client(Drawable *drawable)
{
    this->drawable = drawable;
}


void Client::nextPage() {
    static int pageNumber = 0;
    pageNumber++;
    std::cout << "PageNumber " << pageNumber << std::endl;
	Point2D topLeft(50, 50);
	Point2D botRight(700, 700);
	Pane drawPane(topLeft, botRight);
    switch(pageNumber % 4) {
    case 1:

        draw_rect(0, 0, 750, 750, Color::WHITE);
        draw_rect( 50,  50, 700, 700, Color::BLACK);
		draw_page1(drawPane);
        drawable->updateScreen();   // you must call this to make the display change.
        break;
    case 2:
        break;
    case 3:
		draw_rect(0, 0, 750, 750, Color::WHITE);
		draw_rect(50, 50, 700, 700, Color::BLACK);
		draw_page3(drawPane);
		drawable->updateScreen();   // you must call this to make the display change.
        break;
    case 4:
        // fall through...
    default:
        draw_rect(0, 0, 750, 750, 0xffffffff);
        draw_rect(400, 400, 700, 700, 0xff00ff40);
        drawable->updateScreen();
    }
}

void Client::draw_rect(int x1, int y1, int x2, int y2, unsigned int color) {
    for(int x = x1; x<x2; x++) {
        for(int y=y1; y<y2; y++) {
            drawable->setPixel(x, y, color);
        }
    }
}

void Client::draw_page1(Pane pane)
{
	DDA_Drawer drawer(drawable);
	ShapeHelper::draw_squaredShiftedTriangleLines(pane, &drawer);
}

void Client::draw_page2(Pane pane)
{
}

void Client::draw_page3(Pane pane) {
	std::ifstream simpFileStream;
	simpFileStream.open("simp.txt");
	SimpReader reader(&simpFileStream);
	reader.Run();
}

void Client::draw_page4(Pane pane)
{
}

void Client::draw_page5(Pane pane)
{
}

void Client::draw_page6(Pane pane)
{
}
