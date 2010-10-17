#include "Interface.h"
#include <QDebug>
#include <math.h>
#include <QLineF>
#include <QPolygonF>

Interface::Interface()
{
    minX = INF;
    minY = INF;
    maxX = -INF;
    maxY = -INF;
    faceExterna = NULL;
}

bool Interface::isExterna(Face* f)
{
    return (f == faceExterna);
}

QList<QPair<QPointF, QPointF> > Interface::getTodasArestas()
{
    return map.keys();
}

void Interface::addFace(QVector<QPointF> in)
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

Vertex* Interface::addVertex(QPointF p)
{
    QMap<QPointF,Vertex*>::iterator it;

    it = vertices.find(p);
    if (it == vertices.end())
    {
        Vertex *v = new Vertex(p);
        vertices[p] = v;

        return v;
    }

    return it.value();
}

HalfEdge* Interface::findTwin(QPointF u, QPointF v)
{
    QMap<QPair<QPointF, QPointF>, HalfEdge*>::iterator it;

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

    kdt = new KDTree(map.values(),QRectF(minX,minY,minX+maxX, minY+maxY));
}

HalfEdge* Interface::getArestaNear(QPointF p)
{
    QList<HalfEdge *> *lista = kdt->find(p);

    HalfEdge *e, *best = NULL;
    QPointF p1, p2;
    double min = INF;

    //qDebug() << "Ponto p: " << p;

    for (int i = 0; i < lista->size(); i++)
    {
        e = lista->at(i);

        p1 = e->getOrigem()->getPoint();
        p2 = e->getDestino()->getPoint();

       // qDebug() << "Reta: " << p1 << " - " << p2;

        double mod2 = modulo2(p1,p2);

        double u = ((p.x() - p1.x())*(p2.x() - p1.x())) + ((p.y() - p1.y())*(p2.y() - p1.y()));
        u = u / mod2;

        double x = p1.x() + u * (p2.x() - p1.x());
        double y = p1.y() + u * (p2.y() - p1.y());

        double dist2 = (p.x() - x)*(p.x() - x) + (p.y() - y)*(p.y() - y);

       // qDebug() << "dist2 " << dist2 << "min " << min;

        if (dist2 < min)
        {
            min = dist2;
            best = e;
        }

    }

    return best;


}
Face* Interface::getFaceNear(QPointF p)
{
    HalfEdge *min = getArestaNear(p);

    if(dentroFace(min,p))
        return min->getFace();

    return min->getTwin()->getFace();
}

Vertex* Interface::getVerticeNear(QPointF p)
{
    QList<HalfEdge *> *lista = kdt->find(p);
    Vertex* menor = NULL;
    double distMenor = INF;
    double dist;
    QPointF ponto;

    for(int it = 0; it < lista->size(); ++it)
    {
        ponto = lista->operator [](it)->getOrigem()->getPoint();
        dist = sqrt((p.x()-ponto.x())*(p.x()-ponto.x()) + (p.y()-ponto.y())*(p.y()-ponto.y()));
        if(dist < distMenor)
        {
            distMenor = dist;
            menor = lista->operator [](it)->getOrigem();
        }

        ponto = lista->operator [](it)->getDestino()->getPoint();
        dist = sqrt((p.x()-ponto.x())*(p.x()-ponto.x()) + (p.y()-ponto.y())*(p.y()-ponto.y()));
        if(dist < distMenor)
        {
            distMenor = dist;
            menor = lista->operator [](it)->getDestino();
        }
    }

    return menor;
}

bool Interface::dentroFace(HalfEdge* h, QPointF p)
{
    HalfEdge::iterator it;
    QVector<QPointF> list;

    list.push_back(h->getOrigem()->getPoint());
    for(it = h->f_begin(); it != h->f_end(); ++it)
    {
        list.push_back(it->getOrigem()->getPoint());
    }

    QPolygonF po(list);

    return po.containsPoint(p, Qt::OddEvenFill);
}

double Interface::vProd(QPointF p1, QPointF p2)
{
    return (p1.x() * p2.y() - p1.y() * p2.x());
}

double Interface::eProd(QPointF p1, QPointF p2)
{
    return (p1.x() * p2.x() + p1.y() * p2.y());
}

double Interface::modulo2(QPointF p1, QPointF p2)
{
    double x = p2.x() - p1.x();
    double y = p2.y() - p1.y();

    return (x*x + y*y);

}
