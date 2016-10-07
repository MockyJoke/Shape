#include <iostream>
#include "client.h"
#include "LineDrawer.h"
#include "Color.h"
#include "ShapeHelper.h"

Client::Client(Drawable *drawable)
{
    this->drawable = drawable;
}


void Client::nextPage() {
    static int pageNumber = 0;
    pageNumber++;
    std::cout << "PageNumber " << pageNumber << std::endl;
	std::vector<Pane> panes = {
		Pane(Point(50,50),Point(350,350)),
		Pane(Point(400,50),Point(700,350)),
		Pane(Point(50,400),Point(350,700)),
		Pane(Point(400,400),Point(700,700)),
	};
    switch(pageNumber % 4) {
    case 1:
        draw_rect(0, 0, 750, 750, Color::WHITE);
		
        draw_rect( 50,  50, 350, 350, Color::BLACK);
        draw_rect(400,  50, 700, 350, Color::BLACK);
        draw_rect( 50, 400, 350, 700, Color::BLACK);
        draw_rect(400, 400, 700, 700, Color::BLACK);
		draw_page1(panes);
        drawable->updateScreen();   // you must call this to make the display change.
        break;
    case 2:
		draw_rect(0, 0, 750, 750, Color::WHITE);

		draw_rect(50, 50, 350, 350, Color::BLACK);
		draw_rect(400, 50, 700, 350, Color::BLACK);
		draw_rect(50, 400, 350, 700, Color::BLACK);
		draw_rect(400, 400, 700, 700, Color::BLACK);

		draw_page2(panes);
		drawable->updateScreen();
        break;
    case 3:
		draw_rect(0, 0, 750, 750, Color::WHITE);

		draw_rect(50, 50, 350, 350, Color::BLACK);
		draw_rect(400, 50, 700, 350, Color::BLACK);
		draw_rect(50, 400, 350, 700, Color::BLACK);
		draw_rect(400, 400, 700, 700, Color::BLACK);
		draw_page3(panes);
		drawable->updateScreen();
        break;
    case 4:
		draw_rect(0, 0, 750, 750, Color::WHITE);

		draw_rect(50, 50, 350, 350, Color::BLACK);
		draw_rect(400, 50, 700, 350, Color::BLACK);
		draw_rect(50, 400, 350, 700, Color::BLACK);
		draw_rect(400, 400, 700, 700, Color::BLACK);
		draw_page4(panes);
		drawable->updateScreen();
		break;
	case 5:
		draw_rect(0, 0, 750, 750, Color::WHITE);

		draw_rect(50, 50, 350, 350, Color::BLACK);
		draw_rect(400, 50, 700, 350, Color::BLACK);
		draw_rect(50, 400, 350, 700, Color::BLACK);
		draw_rect(400, 400, 700, 700, Color::BLACK);
		draw_page5(panes);
		drawable->updateScreen();
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

void Client::draw_page1(std::vector<Pane> panes) {
	
	//dda_drawer.draw_line(350, 50, 50, 350, 0xffff0000);

	//Pane 1
	DDA_Drawer dda_drawer(drawable);
	ShapeHelper::draw_starBurst(panes[0], &dda_drawer);

	//Pane 2
	Bresenham_Drawer bresenham_drawer(drawable);
	ShapeHelper::draw_starBurst(panes[1], &bresenham_drawer);

	//Pane 3
	std::vector<LineDrawer*> lineDrawers = { &dda_drawer,&bresenham_drawer };
	ShapeHelper::draw_starBurst_alt(panes[2], lineDrawers);

	//Pane 4

}

void Client::draw_page2(std::vector<Pane> panes)
{
	//Pane 1
	DDA_Drawer dda_drawer(drawable);
	ShapeHelper::draw_parallelogram(panes[0], &dda_drawer);


	//Pane 2
	Bresenham_Drawer bresenham_drawer(drawable);
	ShapeHelper::draw_parallelogram(panes[1], &bresenham_drawer);

	//Pane 3
	std::vector<LineDrawer*> lineDrawers = { &dda_drawer,&bresenham_drawer };
	ShapeHelper::draw_parallelogram_alt(panes[2], lineDrawers);

	//Pane 4
}

void Client::draw_page3(std::vector<Pane> panes)
{
}

void Client::draw_page4(std::vector<Pane> panes)
{
}

void Client::draw_page5(std::vector<Pane> panes)
{
}
