#ifndef INTERFACE_H
#define INTERFACE_H

#include <QMap>
#include <QPair>
#include "Vertex.h"
#include "HalfEdge.h"
#include "Face.h"

#define EXTERNA NULL
#define INF 0x3F3F3F3F
#define MIN(a,b) ((a < b)?(a):(b))
#define MAX(a,b) ((a > b)?(a):(b))

class Interface : public QObject
{
    Q_OBJECT
public:
    Interface();

HalfEdge* findTwin(QPoint u,QPoint v);
void addFace(QVector<QPoint> in);
void addExtEdges(void);
int getMaxX(void){ return maxX; }
int getMaxY(void){ return maxY; }
int getMinX(void){ return minX; }
int getMinY(void){ return minY; }
QList<QPair<QPoint, QPoint> > getTodasArestas();
QList<QPair<QPoint, QPoint> > getArestasNearAresta(HalfEdge* h);
QList<QPair<QPoint, QPoint> > getArestasNearFace(Face* f);
QList<QPair<QPoint, QPoint> > getArestasNearVertice(Vertex* v);
QList<QPoint> getVerticesNearAresta(HalfEdge* h);
QList<QPoint> getVerticesNearFace(Face* f);
QList<QPoint> getVerticesNearVertice(Vertex* v);
QList<QList<QPoint> > getFacesNearAresta(HalfEdge* h);
QList<QList<QPoint> > getFacesNearFace(Face* f);
QList<QList<QPoint> > getFacesNearVertice(Vertex* v);
HalfEdge* getArestaNear(QPoint p);
Face* getFaceNear(QPoint p);
Vertex* getVerticeNear(QPoint p);


Face *faceExterna;

private:
    QMap<QPoint,Vertex*> vertices;
    QVector<Face*> faces;
    QMap<QPair<QPoint,QPoint>, HalfEdge *> map;
    int minX, minY, maxX, maxY;

    Vertex* addVertex(QPoint p);
    void adicionaface(HalfEdge* e, Face* f);
};

inline bool operator< (const QPoint& p1, const QPoint& p2);

#endif // INTERFACE_H
