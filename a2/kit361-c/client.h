#ifndef CLIENT_H
#define CLIENT_H
#include "drawable.h"
#include "pageturner.h"
#include "utility.h"
class Client : public PageTurner
{
public:
    Client(Drawable *_drawable);
    void nextPage();

private:
    Drawable *_drawable;
    void draw_rect(int x1, int y1, int x2, int y2, unsigned int color);
	void draw_page1(Pane pane);
	void draw_page2(Pane pane);
	void draw_page3(Pane pane);
	void draw_page4(Pane pane);
	void draw_page5(Pane pane);
	void draw_page6(Pane pane);
	void draw_page7(Pane pane);
	void draw_page8(Pane pane);

	



};

#endif // CLIENT_H
