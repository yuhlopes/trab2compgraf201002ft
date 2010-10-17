#include "KDTree.h"
#include <QtAlgorithms>

bool x;

KDTree::KDTree(const QList<HalfEdge *> &list, QRectF limite)
{
    if(list.size() > 0)
    {
        raiz = divide(new QList<HalfEdge *>(list), limite, true);
    }
}

NO* KDTree::divide(QList<HalfEdge *> *list, QRectF limite, bool primeiro)
{
    NO *no;
    double v;
    QRectF retE(limite);
    QRectF retD(limite);
    QList<HalfEdge *> *esq = new QList<HalfEdge *>();
    QList<HalfEdge *> *dir = new QList<HalfEdge *>();

    v = pivoteia(list, limite, primeiro, esq, dir);

    if(esq->size() < 0 || dir->size() < 0)
    {
        delete esq;
        delete dir;
        return NULL;
    }

    no = new NO();

    no->chave = v;

    if(primeiro)
    {
        retE.setRight(v);
        retD.setLeft(v);
    }else
    {
        retE.setTop(v);
        retD.setBottom(v);
    }

    no->esq = divide(esq, retE, !primeiro);
    no->dir = divide(dir, retD, !primeiro);

    if(no->esq == NULL && no->dir == NULL)
    {
        delete esq;
        delete dir;
        no->lista = list;
    }else if(no->esq == NULL)
    {
        delete list;
        no->lista = esq;
    }else if(no->dir == NULL)
    {
        delete list;
        no->lista = dir;
    }else
        delete list;

    return no;

}

double KDTree::pivoteia(QList<HalfEdge *> *list, QRectF limite, bool primeiro, QList<HalfEdge *> *men, QList<HalfEdge *> *mai)
{
    int it;
    int piv;
    QPoint p1, p2;
    HalfEdge *p;
    HalfEdge *i;

    x = primeiro;
    qSort(list->begin(), list->end());

    piv = list->size()/2 -1;
    p = list->operator [](piv);

    if(primeiro)
    {
        p1.setX(p->getOrigem()->getPoint().x());
        p2.setX(p->getOrigem()->getPoint().x());
        p1.setY(limite.top());
        p2.setY(limite.bottom());
    }else
    {
        p1.setY(p->getOrigem()->getPoint().y());
        p2.setY(p->getOrigem()->getPoint().y());
        p1.setX(limite.left());
        p2.setX(limite.right());
    }

    for(it = 0; it < list->size(); ++it)
    {
        i = list->operator [](it);
        if(cruza(p1,p2, i->getOrigem()->getPoint() , i->getDestino()->getPoint()))
        {
            men->push_back(i);
            mai->push_back(i);
        }else if(primeiro)
        {
            if(i->getOrigem()->getPoint().x() <= p->getOrigem()->getPoint().x())
                men->push_back(i);
            else
                mai->push_back(i);
        }else
        {
            if(i->getOrigem()->getPoint().y() <= p->getOrigem()->getPoint().y())
                men->push_back(i);
            else
                mai->push_back(i);
        }
    }

    if(primeiro)
        return p->getOrigem()->getPoint().x();
    else
        return p->getOrigem()->getPoint().y();

}
bool KDTree::cruza(QPointF v1, QPointF v2 , QPointF u1, QPointF u2)
{
    double a1, b1, a2, b2, xi, yi;

    b1 = (v2.y() - v1.y())/(v2.x() - v1.x());
    b2 = (u2.y() - u1.y())/(u2.x() - u1.x());

    a1 = v1.y()-b1*v1.x();
    a2 = u1.y()-b2*u1.x();

    xi = - (a1-a2)/(b1-b2);
    yi = a1+b1*xi;

    if((v1.x()-xi)*(xi-v2.x()) >= 0 && (u1.x()-xi)*(xi-u2.x()) >= 0 && (v1.y()-yi)*(yi-v2.y()) >= 0 && (u1.y()-yi)*(yi-u2.y()) >= 0)
        return true;

    return false;
}

bool operator<(HalfEdge &e1, HalfEdge& e2)
{

       if(x)
            return e1.getOrigem()->getPoint().x() < e1.getOrigem()->getPoint().x();
        else
            return e1.getOrigem()->getPoint().y() < e1.getOrigem()->getPoint().y();

    return false;

}

QList<HalfEdge *>* KDTree::find(const QPointF &p)
{
    return busca(p, raiz,true);
}

QList<HalfEdge *>* KDTree::busca(const QPointF &p, NO* no,bool primeiro)
{
    double v;
    if(primeiro)
        v = p.x();
    else
        v = p.y();

    if(v < no->chave)
    {
        if(no->esq == NULL)
            return no->lista;
        else
            return busca(p, no->esq, !primeiro);
    }else
    {
        if(no->dir == NULL)
            return no->lista;
        else
            return busca(p, no->dir, !primeiro);
    }

}
