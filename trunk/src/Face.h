#ifndef FACE_H
#define FACE_H

#include <QObject>
#include <QVector>
#include "HalfEdge.h"

class Face : public QObject
{
    Q_OBJECT
public:
    Face();

void setOuterComp(HalfEdge *e);
HalfEdge* getOuterComp(void);

private:
    HalfEdge *outerComp;
    QVector<HalfEdge> innerComps;
};


#endif // FACE_H
