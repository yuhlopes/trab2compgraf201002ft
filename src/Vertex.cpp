#include "Vertex.h"

Vertex::Vertex()
{
}

Vertex::Vertex(QPoint p)
{
    this->p = p;
}

bool operator== (const Vertex &v1, const Vertex &v2)
{
    return (v1.getPoint() == v2.getPoint());
}

bool Vertex::operator <(Vertex *v)
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
