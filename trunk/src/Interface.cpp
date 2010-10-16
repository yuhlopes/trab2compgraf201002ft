#include "Interface.h"
#include <QDebug>
#include <math.h>

Interface::Interface()
{
    minX = INF;
    minY = INF;
    maxX = -INF;
    maxY = -INF;
    faceExterna = NULL;
}

inline bool operator< (const QPoint& p1, const QPoint& p2)
{
 if (p1.x() == p2.x())
        return (p1.y() < p2.y());

    return (p1.x() < p2.x());

}

bool Interface::isExterna(Face* f)
{
    return (f == faceExterna);
}

QList<QPair<QPoint, QPoint> > Interface::getTodasArestas()
{
    return map.keys();
}

void Interface::addFace(QVector<QPoint> in)
{
    Face *f = new Face();
    HalfEdge *ant = NULL;
    HalfEdge *first, *twin;

    for(int i = 0; i < in.size(); i++)
    {
        HalfEdge *e = new HalfEdge();
        if (i == 0)     first = e;

        map[qMakePair(in[i], in[(i+1)%in.size()])] = e;
        twin = findTwin(in[(i+1)%in.size()],in[i]);

        minX = MIN(minX, in[i].x());
        maxX = MAX(maxX, in[i].x());
        minY = MIN(minY, in[i].y());
        maxY = MAX(maxY, in[i].y());

        Vertex *v = addVertex(in[i]);
        v->setEdge(e);
        e->setOrigem(v);

        e->setFace(f);

        e->setAnt(ant);
        if (ant!= NULL)
        {
            ant->setProx(e);
            qDebug() << ant << "->" << e;
        }

        e->setTwin(twin);
        if (twin!= NULL)
        {
            twin->setTwin(e);
            qDebug() << "    Twin:" << twin;
        }

        ant = e;
    }
    //qDebug() << "Saiu do for";

    first->setAnt(ant);
    ant->setProx(first);

    qDebug() << ant << "->" << first;

    f->setOuterComp(first);

    faces.push_back(f);
}

Vertex* Interface::addVertex(QPoint p)
{
    QMap<QPoint,Vertex*>::iterator it;

    it = vertices.find(p);
    if (it == vertices.end())
    {
        Vertex *v = new Vertex(p);
        vertices[p] = v;

        return v;
    }

    return it.value();
}

HalfEdge* Interface::findTwin(QPoint u, QPoint v)
{
    QMap<QPair<QPoint, QPoint>, HalfEdge*>::iterator it;

    it = map.find(qMakePair(u,v));
    if(it != map.end())
    {
        return it.value();
    }

    return NULL;
}

void Interface::adicionaface(HalfEdge* e, Face* f)
{
    HalfEdge *ori = e;
    HalfEdge *ant = NULL;
    HalfEdge *nova = NULL;

    nova = new HalfEdge();

    nova->setFace(f);
    nova->setTwin(ori);
    ori->setTwin(nova);
    nova->setOrigem(ori->getDestino());
    f->setOuterComp(nova);
    ant = nova;

    ori = ori->getProx();
    while(ori != e)
    {
        while(ori->getTwin() != NULL)
            ori = ori->getTwin()->getProx();

        nova = new HalfEdge();

        nova->setFace(f);
        nova->setTwin(ori);
        ori->setTwin(nova);
        nova->setProx(ant);
        ant->setAnt(nova);
        nova->setOrigem(ori->getDestino());

        ori = ori->getProx();

        ant = nova;
    }

    ant->setAnt(ori->getTwin());
    ori->getTwin()->setProx(ant);
}

void Interface::addExtEdges(void)
{
    if(faceExterna == NULL)
        faceExterna = new Face();

    QList<HalfEdge *> lista = map.values();

    for(int i = 0; i < lista.size(); ++i)
    {
        if(lista[i]->getTwin() == NULL)
        {
            adicionaface(lista[i], faceExterna);
        }
    }
}

HalfEdge* Interface::getArestaNear(QPoint p)
{
    return NULL;
}
Face* Interface::getFaceNear(QPoint p)
{
    return NULL;
}
Vertex* Interface::getVerticeNear(QPoint p)
{
    QMap<QPoint,Vertex*>::const_iterator it;
    Vertex* menor = NULL;
    double distMenor = INF;
    double dist;
    QPoint ponto;

    for(it = vertices.begin(); it != vertices.end(); ++it)
    {
        ponto = it.value()->getPoint();
        dist = sqrt((p.x()-ponto.x())*(p.x()-ponto.x()) + (p.y()-ponto.y())*(p.y()-ponto.y()));
        if(dist < distMenor)
        {
            distMenor = dist;
            menor = it.value();
        }
    }

    return menor;
}
