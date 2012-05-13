#include "Face.h"

Face::Face()
{

}

Face::~Face()
{
}

void Face::setHalfEdge(HalfEdge *e)
{
    this->hEdge = e;
}

HalfEdge* Face::getHalfEdge(void)
{
    return this->hEdge;
}
