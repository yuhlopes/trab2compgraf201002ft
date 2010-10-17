#ifndef KDTREE_H
#define KDTREE_H

#include <QObject>
#include <QPointF>
#include "HalfEdge.h"
#include <QRect>

typedef struct _NO
{
    double chave;
    _NO *esq;
    _NO *dir;
    QList<HalfEdge*> *lista;
}NO;

class KDTree : QObject
{
    Q_OBJECT

private:
    NO *raiz;

    NO* divide(QList<HalfEdge *> *list, QRectF limite, bool primeiro);
    double pivoteia(QList<HalfEdge *> *list, QRectF limite, bool primeiro, QList<HalfEdge *> *men, QList<HalfEdge *> *mai);
    bool cruza(QPointF v1, QPointF v2 , QPointF u1, QPointF u2);
    QList<HalfEdge *>* busca(const QPointF &p, NO* no,bool primeiro);

public:
    KDTree(const QList<HalfEdge *> &list, QRectF limite);
    QList<HalfEdge *> *find(const QPointF &p);
};

#endif // KDTREE_H
