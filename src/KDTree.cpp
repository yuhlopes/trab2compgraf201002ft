#include "KDTree.h"
#include <QtAlgorithms>

KDTree::KDTree(const QList<HalfEdge *> &list, QRectF limite)
{
    if(list.size() > 0)
    {
        raiz = divide(new QList<HalfEdge *>(list), limite, true);
    }else
        raiz = NULL;
}

KDTree::~KDTree()
{
    if(raiz != NULL)
        limpa(raiz);
}

NO* KDTree::divide(QList<HalfEdge *> *list, QRectF limite, bool primeiro)
{
    NO *no;
    double v;
    QRectF retE(limite);
    QRectF retD(limite);
    QList<HalfEdge *> *esq = new QList<HalfEdge *>();
    QList<HalfEdge *> *dir = new QList<HalfEdge *>();

 //   qDebug() << "Check 1";

    v = pivoteia(list, limite, primeiro, esq, dir);

//    qDebug() << "Check 2";

    if(esq->size() < 0 || dir->size() < 0 || esq->size() == list->size() || dir->size() == list->size())
    {
        delete esq;
        delete dir;
  //      qDebug() << "Check 3";
        return NULL;
    }

//    qDebug() << "Check 4";

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

//    qDebug() << "Check 5";

    no->esq = divide(esq, retE, !primeiro);
//    qDebug() << "Check 6";
    no->dir = divide(dir, retD, !primeiro);
//    qDebug() << "Check 7";

    if(no->esq == NULL && no->dir == NULL)
    {
        delete esq;
        delete dir;
        no->lista = list;
//        qDebug() << "Check 8";
    }else if(no->esq == NULL)
    {
        delete list;
        no->lista = esq;
//        qDebug() << "Check 8";
    }else if(no->dir == NULL)
    {
        delete list;
        no->lista = dir;
//        qDebug() << "Check 10";
    }else
    {
        delete list;
//        qDebug() << "Check 11";
    }

//    qDebug() << "Check 12";
    return no;

}

double KDTree::pivoteia(QList<HalfEdge *> *list, QRectF limite, bool primeiro, QList<HalfEdge *> *men, QList<HalfEdge *> *mai)
{
    int it;
    int piv;
    QPoint p1, p2;
    HalfEdge *p;
    HalfEdge *i;
    QList<SORTEADOR> *l = new QList<SORTEADOR>();
    SORTEADOR sort;

//    qDebug() << "Check P1";
    for(int i = 0; i < list->size(); ++i)
    {
        sort.ind = i;
        if(primeiro)
            sort.val = list->operator [](i)->getOrigem()->getPoint().x();
        else
            sort.val = list->operator [](i)->getOrigem()->getPoint().y();
        l->push_back(sort);
 //       qDebug() << "Check P2";
    }
//    qDebug() << "Check P3";
    qSort(*l);
//    qDebug() << "Check P4";

    piv = list->size()/2;
    int j = l->operator [](piv).ind;
    p = list->operator [](j);
    l->clear();
    delete l;
//    qDebug() << "Check P5";

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
//    qDebug() << "Check P6";

    for(it = 0; it < list->size(); ++it)
    {
        i = list->operator [](it);
        if(cruza(QLineF(p1,p2), QLineF(i->getOrigem()->getPoint() , i->getDestino()->getPoint())))
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
 //       qDebug() << "Check P7";
    }
 //   qDebug() << "Check P8";

    if(primeiro)
        return p->getOrigem()->getPoint().x();
    else
        return p->getOrigem()->getPoint().y();

}



bool KDTree::cruza(const QLineF &v, const QLineF &u)
{
    QPointF p = v.p1();
    QPointF q = v.p2();

    QPointF r = u.p1();
    QPointF s = u.p2();

   double a = vProd(p-r, q-r);
   double b = vProd(p-s, q-s);
   double c = vProd(r-p, s-p);
   double d = vProd(r-q, s-q);

   if (a*b < 0 && c*d < 0) return true;

   if (NAO(a) && eProd(p-r, q-r)<= 0) return true;
   if (NAO(b) && eProd(p-s, q-s)<= 0) return true;
   if (NAO(c) && eProd(r-p, s-p)<= 0) return true;
   if (NAO(d) && eProd(r-q, s-q)<= 0) return true;

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

void KDTree::limpa(NO* no)
{
    if(no->esq != NULL)
        limpa(no->esq);
    if(no->dir != NULL)
        limpa(no->dir);

    if(no->lista != NULL)
    {
        delete no->lista;
    }

    delete no;
}

bool operator<(const SORTEADOR& s1, const SORTEADOR& s2){ return s1.val < s2.val; }

double KDTree::vProd(QPointF p1, QPointF p2)
{
    return (p1.x() * p2.y() - p1.y() * p2.x());
}

double KDTree::eProd(QPointF p1, QPointF p2)
{
    return (p1.x() * p2.x() + p1.y() * p2.y());
}

