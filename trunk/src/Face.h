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
    virtual ~Face();

void setHalfEdge(HalfEdge *e);
HalfEdge* getHalfEdge(void);

private:
    HalfEdge *hEdge;
};


#endif // FACE_H
