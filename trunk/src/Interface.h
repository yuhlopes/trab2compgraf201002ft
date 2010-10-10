#ifndef INTERFACE_H
#define INTERFACE_H

#include <QMap>
#include <QPair>
#include "Vertex.h"
#include "HalfEdge.h"
#include "Face.h"

class Interface : public QObject
{
    Q_OBJECT
public:
    Interface();

HalfEdge* findTwin(QPoint u,QPoint v);
void addFace(QVector<QPoint> in);
void addExtEdges(void);
int getMaxX(void);
int getMaxY(void);
int getMinX(void);
int getMinY(void);

private:
    QMap<QPoint,Vertex*> vertices;
    QVector<Face*> faces;
    QMap<QPair<QPoint,QPoint>, HalfEdge *> map;
    int minX, minY, maxX, maxY;

    Vertex* addVertex(QPoint p);
};


#endif // INTERFACE_H
