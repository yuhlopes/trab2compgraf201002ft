#ifndef HALFEDGE_H
#define HALFEDGE_H

#include <QPoint>
#include <QObject>
#include "Vertex.h"

class Face;

class HalfEdge : public QObject
{
    Q_OBJECT
public:
    HalfEdge();

void setOrigem(Vertex *v);
void setOrigem(QPoint *p);
void setFace(Face *f);
void setTwin(HalfEdge *twin);
void setProx(HalfEdge *prox);
void setAnt(HalfEdge *ant);
Vertex*   getOrigem(void);
Face*     getFace(void);
HalfEdge* getTwin(void);
HalfEdge* getProx(void);
HalfEdge* getAnt(void);

private:
    Vertex *origem;
    Face *f;
    HalfEdge *twin, *prox, *ant;

};

#endif // HALFEDGE_H
