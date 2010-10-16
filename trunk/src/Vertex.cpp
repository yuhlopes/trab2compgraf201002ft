#include "Vertex.h"

Vertex::Vertex()
{
}

Vertex::Vertex(QPoint p)
{
    this->p = p;
}

bool Vertex::operator== ( const Vertex &v2)const
{
    return (this->getPoint() == v2.getPoint());
}

bool Vertex::operator <(Vertex *v)const
{
    if (this->getPoint().x() == v->getPoint().x())
        return (this->getPoint().y() - v->getPoint().y());

    return (this->getPoint().x() - v->getPoint().x());
}

void Vertex::setPoint(QPoint p)
{
    this->p = p;
}

void Vertex::setEdge(HalfEdge *hEdge)
{
    this->hEdge = hEdge;
}

QPoint Vertex::getPoint(void) const
{
    return p;
}

HalfEdge* Vertex::getEdge(void)
{
    return hEdge;
}
