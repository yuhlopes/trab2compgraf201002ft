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

bool HalfEdge::operator==(HalfEdge& e)
{
    HalfEdge* t = this->getProx();
    HalfEdge* p = e.getProx();
    Vertex *v1 = this->getOrigem();
    Vertex *v2 = e.getOrigem();
    Vertex *u1 = t->getOrigem();
    Vertex *u2 = p->getOrigem();

    return ((*v1 == *v2) && (*u1 == *u2));
}

HalfEdge::iterator HalfEdge::begin()
{
    iterator i(this);
    return i;
}

HalfEdge::iterator HalfEdge::end()
{
    iterator i((HalfEdge *)NULL);
    return i;
}

HalfEdge::Iterator::Iterator(HalfEdge::Iterator *i)
{
    this->atual = &(i->operator *());
    this->primeira = this->atual;
}
HalfEdge::Iterator::Iterator(HalfEdge *atual)
{
    this->atual = atual;
    this->primeira = this->atual;
}
HalfEdge::Iterator::Iterator()
{
    this->atual = NULL;
    this->primeira = NULL;
}
HalfEdge::Iterator& HalfEdge::Iterator::operator++(int i)
{
    ++(*this);
    HalfEdge::Iterator it(*this);
    return it;
}

HalfEdge::Iterator& HalfEdge::Iterator::operator++()
{
    if(atual != NULL)
    {
        this->atual = atual->getTwin();
        this->atual = atual->getProx();
        if(atual == primeira)
        {
            atual = NULL;
        }
    }
    return *this;
}

HalfEdge* HalfEdge::Iterator::operator->()
{
    return this->atual;
}
HalfEdge::Iterator& HalfEdge::Iterator::operator=(const HalfEdge::Iterator& i)
{
    this->atual = i.atual;
    return *this;
}
bool HalfEdge::Iterator::operator==(const HalfEdge::Iterator& i)const
{
    HalfEdge e;
    if(atual == NULL)
        return (e == i.operator *());

    return (*atual == *i);
}
bool HalfEdge::Iterator::operator!=(const HalfEdge::Iterator& i)const
{
    return ! (*this == i);
}
HalfEdge& HalfEdge::Iterator::operator*()const
{
    HalfEdge e;
    if(atual == NULL)
           return e;

    return *atual;
}
