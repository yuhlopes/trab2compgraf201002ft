#include "Interface.h"

#define EXTERNA NULL
#define INF 0x3F3F3F3F
#define MIN(a,b) ((a < b)?(a):(b))
#define MAX(a,b) ((a > b)?(a):(b))

Interface::Interface()
{
    minX = INF;
    minY = INF;
    maxX = -INF;
    maxY = -INF;
}

bool operator<(const QPoint &p1, const QPoint &p2)
{
    if (p1.x() == p2.x())
        return (p1.y() < p2.y());

    return (p1.x() < p2.x());
}

void Interface::addFace(QVector<QPoint> in)
{
    Face *f = new Face();
    HalfEdge *ant = NULL;
    HalfEdge *first, *twin;

    for(int i = 0; i < in.size(); i++)
    {
        HalfEdge *e = new HalfEdge();

        minX = MIN(minX, in[i].x());
        maxX = MAX(maxX, in[i].x());
        minY = MIN(minY, in[i].y());
        maxY = MAX(maxY, in[i].y());

        Vertex *v = addVertex(in[i]);

        v->setEdge(e);

        e->setOrigem(v);
        e->setFace(f);
        e->setAnt(ant);

        if (i == 0)     first = e;
        if (ant!= NULL)
        {
            ant->setProx(e);

            map[qMakePair(in[i-1], in[i])] = ant;

            twin = findTwin(in[i],in[i-1]);
            ant->setTwin(twin);
            if (twin!= NULL)
                twin->setTwin(ant);
        }

        ant = e;
    }

    first->setAnt(ant);
    ant->setProx(first);

    int n = in.size();
/*
    map[qMakePair(in[n-1], in[n])] = ant;
    twin = findTwin(in[n],in[n-1]);
*/
    map[qMakePair(in[n-1],in[0])] = ant;
    twin = findTwin(in[0],in[n-1]);
    ant->setTwin(twin);
    if (twin!= NULL)
        twin->setTwin(ant);

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

void Interface::addExtEdges(void)
{
    Face *ext = new Face();

    for (int i = 0; i < faces.size(); i++)
    {
        HalfEdge *e1 = faces[i]->getOuterComp();

        if (e1->getTwin() == NULL)
        {
            HalfEdge *e2 = new HalfEdge();

            e1->setTwin(e2);

            e2->setTwin(e1);
            e2->setOrigem(e1->getProx()->getOrigem());

            HalfEdge *aux;

            aux = e1->getProx()->getTwin();
            while (aux->getFace()!= EXTERNA && aux!= NULL)
            {
                aux = aux->getProx()->getTwin();
            }
            e2->setAnt(aux);
            if (aux!= NULL) aux->setProx(e2);

            aux = e1->getAnt()->getTwin();
            while (aux->getFace()!= EXTERNA && aux!= NULL)
            {
                aux = aux->getAnt()->getTwin();
            }
            e2->setProx(aux);
            if (aux!= NULL) aux->setAnt(e2);

            e2->setFace(ext);
            ext->setOuterComp(e2);
        }

    }

    faces.push_back(ext);
}
