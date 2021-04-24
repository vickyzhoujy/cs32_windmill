#ifndef WINDMILL_H
#define WINDMILL_H

#include "polygon.h"
#include "rect.h"
#include "ellipse.h"
#include "color.h"
#include "vec2.h"

class Windmill{
    private:
        std::vector<shared_ptr<Polygon>> polys;
        std::vector<Rect> rects;
        std::vector<ellipse> ellipses;

        vec2 center;
        int width;
        int height;
        int depth;

    public:
        Windmill(int width, int height, vec2 center, int depth);

        void setCenter(vec2 &cen)
        {
            this->center.setX(cen.x());
            this->center.setY(cen.y());
        };
        void setHeight(int height) { this->height = height;};
        void setWidth(int width) { this->width = width;};

        void setDepth(int depth);

        vec2 getCenter() const {return center;};
        int getHeight() const {return height;};
        int getWidth() const {return width;};

        std::vector<shared_ptr<Polygon>> getPolys() {return polys;};
        std::vector<Rect> getRects();
        std::vector<ellipse> getEllipses();

};

#endif