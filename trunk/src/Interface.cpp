#include "Interface.h"
#include <QDebug>

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
            qDebug() << in[i-1] << "-" << in[i];
            map[qMakePair(in[i-1], in[i])] = ant;

            twin = findTwin(in[i],in[i-1]);
            ant->setTwin(twin);
            if (twin!= NULL)
                twin->setTwin(ant);
        }

        ant = e;
    }
    qDebug() << "Saiu do for";

    first->setAnt(ant);
    ant->setProx(first);

    int n = in.size();

    qDebug() << "Fora do for:" << in[n-1] << "-" << in[0];
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

void Interface::adicionaface(HalfEdge* e, Face* f)
{
    HalfEdge *ori = e;
    HalfEdge *ant = NULL;
    HalfEdge *nova = NULL;

    nova = new HalfEdge();

    nova->setFace(f);
    nova->setTwin(ori);
    ori->setTwin(nova);
    ori = ori->getProx();
    nova->setOrigem(ori->getOrigem());
    f->setOuterComp(nova);
    ant = nova;

    while(ori != e)
    {
        while(ori->getTwin() != NULL)
            ori = ori->getTwin()->getProx();

        nova = new HalfEdge();

        nova->setFace(f);
        nova->setTwin(ori);
        ori->setTwin(nova);
        ori = ori->getProx();
        nova->setProx(ant);
        ant->setAnt(nova);
        nova->setOrigem(ori->getOrigem());
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

    //faces.push_back(faceExterna);

/*
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
    */
}

QList<QPair<QPoint, QPoint> > Interface::getArestasNearAresta(HalfEdge* h)
{
    QList<QPair<QPoint, QPoint> > lista;
    return lista;
}
QList<QPair<QPoint, QPoint> > Interface::getArestasNearFace(Face* f)
{
    QList<QPair<QPoint, QPoint> > lista;
    return lista;
}
QList<QPair<QPoint, QPoint> > Interface::getArestasNearVertice(Vertex* v)
{
    QList<QPair<QPoint, QPoint> > lista;
    return lista;
}
QList<QPoint> Interface::getVerticesNearAresta(HalfEdge* h)
{
    QList<QPoint> lista;
    return lista;
}
QList<QPoint> Interface::getVerticesNearFace(Face* f)
{
    QList<QPoint> lista;
    return lista;
}
QList<QPoint> Interface::getVerticesNearVertice(Vertex* v)
{
    QList<QPoint> lista;
    return lista;
}
QList<QList<QPoint> > Interface::getFacesNearAresta(HalfEdge* h)
{
    QList<QList<QPoint> > lista;
    return lista;
}
QList<QList<QPoint> > Interface::getFacesNearFace(Face* f)
{
    QList<QList<QPoint> > lista;
    return lista;
}
QList<QList<QPoint> > Interface::getFacesNearVertice(Vertex* v)
{
    QList<QList<QPoint> > lista;
    return lista;
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
    return NULL;
}
