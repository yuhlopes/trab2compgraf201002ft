#ifndef VERTEX_H
#define VERTEX_H

#include <QPoint>
#include <QObject>

class HalfEdge;

class Vertex : public QObject
{
    Q_OBJECT
public:
    Vertex();
    Vertex(QPoint p);

bool operator< (Vertex *v);
void setEdge(HalfEdge *hEdge);
void setPoint(QPoint p);
QPoint getPoint(void) const;


private:
    QPoint p;
    HalfEdge *hEdge;
};

#endif // VERTEX_H
