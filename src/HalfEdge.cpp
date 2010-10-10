#include <HalfEdge.h>

HalfEdge::HalfEdge()
{
    this->prox = NULL;
    this->twin = NULL;
    this->ant = NULL;
    this->f = NULL;
}

void HalfEdge::setOrigem(Vertex *v)
{
    this->origem = v;
}

void HalfEdge::setFace(Face *f)
{
    this->f = f;
}

void HalfEdge::setTwin(HalfEdge *twin)
{
    this->twin = twin;
}

void HalfEdge::setProx(HalfEdge *prox)
{
    this->prox = prox;
}

void HalfEdge::setAnt(HalfEdge *ant)
{
    this->ant = ant;
}

Vertex* HalfEdge::getOrigem(void)
{
    return this->origem;
}

Face* HalfEdge::getFace(void)
{
    return this->f;
}

HalfEdge* HalfEdge::getAnt(void)
{
    return this->ant;
}

HalfEdge* HalfEdge::getProx(void)
{
    return this->prox;
}

HalfEdge* HalfEdge::getTwin(void)
{
    return this->twin;
}
