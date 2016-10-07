#ifndef CLIENT_H
#define CLIENT_H
#include "drawable.h"
#include "pageturner.h"
#include <vector>
#include "ShapeHelper.h"
class Client : public PageTurner
{
public:
    Client(Drawable *drawable);
    void nextPage();

private:
    Drawable *drawable;
    void draw_rect(int x1, int y1, int x2, int y2, unsigned int color);

	void draw_page1(std::vector<Pane> panes);
};

#endif // CLIENT_H
