#include <Render.h>
#include <QPaintDevice>
#include <PlyParser.h>
#include <QRgb>


Render::Render(int w, int h, CommandQueue *c) {

    screenW = w;
    screenH = h;
    cmdq = c;

    sel = NULL;
    buffer = NULL;
    backBuffer = NULL;
    frontBuffer = NULL;
    screen = NULL;
    vsel = NULL;
    hsel = NULL;
    fsel = NULL;
    
    zoom = 1.0;
    screen = new QImage(w, h, QImage::Format_ARGB32_Premultiplied);
    ponto = new QPoint(0,0);

    corArestaGrossa = qRgb(255,255,0);
    corVerticeGrosso = qRgb(0,255,0);
    corFace = qRgb(255,255,255);
    corFaceExt = qRgb(0,0,0);

    arestaScreen.setColor(QColor(0,0,0,255));
    selecionadoScreen.setColor(QColor(0,128,128,128));
    selecionadoScreen.setColor(QColor(255,0,0,128));
    selecionadoScreen.setWidth(10);
    vizinhoScreen.setColor(QColor(0,255,0,128));
    vizinhoScreen.setWidth(10);
    faceExternaBack.setColor(corFaceExt);
    arestaGrossaBack.setColor(corArestaGrossa);
    verticeGrossoBack.setColor(corVerticeGrosso);
    arestaGrossaBack.setWidth(10);
    verticeGrossoBack.setWidth(10);

    mostraAresta = false;
    mostraFace = false;
    mostraPonto = false;


}

void Render::run(void) {
    ExCom ex;
    
    do {
        ex = cmdq->consome();
        if(buffer == NULL)
        {
             msleep(100);
             continue;
        }
        switch(ex.cmd)
        {
            case NENHUM:
                msleep(100);
                continue;
            case INCZOOM:
                incZoom();
                break;
            case DECZOOM:
                decZoom();
                break;
            case INCX:
                incX();
                break;
            case DECX:
                decX();
                break;
            case INCY:
                incY();
                break;
            case DECY:
                decY();
                break;
            case PONTOS:
                mostraPonto = !mostraPonto;
                renderizaFront();
                break;
            case ARESTAS:
                mostraAresta = !mostraAresta;
                renderizaFront();
                break;
            case FACES:
                mostraFace = !mostraFace;
                renderizaFront();
                break;
            case SELECT:
                if(sel == NULL)
                    sel = new QPoint();
                sel->setX(ex.x);
                sel->setY(ex.y);
                click();
                break;
        }
        atualizaScreen();
    } while (true);

}

void Render::updateScreen(int w, int h)
{
    screenW = w;
    screenH = h;

    if(buffer->width() < w || buffer->height() < h)
    {
        reiniciaBuffers(screenW * zoom, screenH * zoom);

    }

    if(ponto->x() + w > buffer->width())
        ponto->setX(buffer->width() - w);
    if(ponto->y() + h > buffer->height())
        ponto->setY(buffer->height() - h);

    atualizaScreen();
}

void Render::recebeArquivo(const QString &filename)
{
    QVector<QPointF> tmp;

    if(sel != NULL)
    {
        delete sel;
        sel = NULL;
    }

    mostraAresta = false;
    mostraFace = false;
    mostraPonto = false;
    vsel = NULL;
    hsel = NULL;
    fsel = NULL;

    zoom = 1.0;
    if(ponto == NULL)
        ponto = new QPoint();

    ponto->setX(0);
    ponto->setY(0);

    //interface.clear();

    PlyParser ply(filename);
    
    tmp = ply.proximo();
    while(tmp.size() > 0)
    {
        qDebug() << tmp;
        interface.addFace(tmp);
//        qDebug() << "Passou";
        tmp = ply.proximo();
    }
    interface.addExtEdges();

    reiniciaBuffers(screenW * zoom, screenH * zoom);


    atualizaScreen();
 }
 
void Render::atualizaScreen(void)
{
    QPainter p;

    if(screen != NULL)
        delete screen;

    screen = new QImage(screenW, screenH, QImage::Format_ARGB32_Premultiplied);
    p.begin(screen);
    p.setCompositionMode(QPainter::CompositionMode_Source);
    p.drawImage(0,0, *backBuffer);//debug
    p.drawImage(0,0, buffer->copy(ponto->x(),ponto->y(),screenW, screenH));
    p.setCompositionMode(QPainter::CompositionMode_SourceOver);
    p.drawImage(0,0, frontBuffer->copy(ponto->x(),ponto->y(),screenW, screenH));
    p.end();
    emit renderizado(*screen);
}

void Render::incX()
{
    qDebug() << "incX";
    if(ponto->x() + INCPOS + screenW/2 < buffer->width())
    {
        qDebug() << "Posso";
        ponto->setX(ponto->x() + INCPOS);
    }else
        qDebug() << "Nao Posso";
}
void Render::decX()
{
    if(ponto->x() - INCPOS >= 0)
        ponto->setX(ponto->x() - INCPOS);
    else
        ponto->setX(0);

}
void Render::incY()
{
    qDebug() << "incY";
    if(ponto->y() + INCPOS + screenH/2 < buffer->height())
        ponto->setY(ponto->y() + INCPOS);
}
void Render::decY()
{
    qDebug() << "decY";
    if(ponto->y() - INCPOS >= 0)
        ponto->setY(ponto->y() - INCPOS);
    else
        ponto->setY(0);
}
void Render::incZoom()
{
    if(zoom == ZOOMLIMIT)
        return;

    if(zoom + INCZ < ZOOMLIMIT)
        zoom += INCZ;
    else
        zoom = ZOOMLIMIT;

    reiniciaBuffers(screenW * zoom, screenH * zoom);
}
void Render::decZoom()
{
    if(zoom == 1.0)
        return;

    if(zoom - INCZ > 1.0)
        zoom -= INCZ;
    else
        zoom = 1.0;

    if(ponto->x() + screenW > screenW*zoom)
        ponto->setX(screenW*zoom - screenW);
    if(ponto->y() + screenH > screenH*zoom)
        ponto->setY(screenH*zoom - screenH);

    reiniciaBuffers(screenW * zoom, screenH * zoom);
}

QPoint Render::transforma(const QPointF &in)
{
    double xwmax, xwmin, ywmax, ywmin;
    double xmax, xmin, ymax, ymin;
    double sx,sy;

    xmin = buffer->width() * MARGEM;
    ymin = buffer->height() * MARGEM;
    xmax = buffer->width() - xmin;
    ymax = buffer->height() - ymin;

    xwmin = interface.getMinX();
    xwmax = interface.getMaxX();
    ywmin = interface.getMinY();
    ywmax = interface.getMaxY();

    sx = (xmax - xmin + 0.0)/(xwmax - xwmin + 0.0);
    sy = (ymax - ymin + 0.0)/(ywmax - ywmin + 0.0);

    if(sx < sy)
        sy = sx;
    else
        sx = sy;

    int x = sx*in.x() - sx*xwmin + xmin + 0.5;
    int y = sy*in.y() - sy*ywmin + ymin + 0.5;

    QPoint p(qRound(x), qRound(y));
    return p;
}

QPointF Render::destransforma(const QPoint &in)
{
    double xwmax, xwmin, ywmax, ywmin;
    double xmax, xmin, ymax, ymin;
    double sx,sy;

    xwmin = buffer->width() * MARGEM;
    ywmin = buffer->height() * MARGEM;
    xwmax = buffer->width() - xwmin;
    ywmax = buffer->height() - ywmin;

    xmin = interface.getMinX();
    xmax = interface.getMaxX();
    ymin = interface.getMinY();
    ymax = interface.getMaxY();

    sx = (xmax - xmin + 0.0)/(xwmax - xwmin + 0.0);
    sy = (ymax - ymin + 0.0)/(ywmax - ywmin + 0.0);

    if(sx > sy)
        sy = sx;
    else
        sx = sy;

    int x = sx*in.x() - sx*xwmin + xmin + 0.5;
    int y = sy*in.y() - sy*ywmin + ymin + 0.5;

    QPointF p(x, y);
    return p;
}

void Render::renderiza(void)
{
    QPoint p1, p2;
    QList<QPair<QPointF,QPointF> > lista = interface.getTodasArestas();
    QPainter buff(buffer);
    QPainter back(backBuffer);

    buff.fillRect(buffer->rect(),Qt::transparent);
    back.fillRect(backBuffer->rect(), Qt::transparent);

    buff.setPen(arestaScreen);

    for(int i = 0; i < lista.size() ; ++i)
    {
        p1 = transforma(lista[i].first);
        p2 = transforma(lista[i].second);

        buff.drawLine(p1,p2);

        back.setPen(arestaGrossaBack);
        back.drawLine(p1,p2);
        back.setPen(verticeGrossoBack);
        back.drawEllipse(p1,5,5);
        back.drawEllipse(p2,5,5);
    }
}

void Render::click(void)
{
    Vertex* v = NULL;
    HalfEdge* h = NULL;
    Face *f = NULL;
    HalfEdge* t = NULL;
    QPointF p1;
    QPoint p2;

    p2.setX(sel->x() + ponto->x());
    p2.setY(sel->y() + ponto->y());

    QRgb rgb = backBuffer->pixel(p2);
    p1 = destransforma(p2);

    if(rgb == corVerticeGrosso)
    {
        qDebug() << "Vertice";
        v = interface.getVerticeNear(p1);
    }else if(rgb == corArestaGrossa)
    {
        qDebug() << "Aresta";
        h = interface.getArestaNear(p1);
    }else if(rgb == corFace)
    {
        qDebug() << "Face";
        f = interface.getFaceNear(p1);
    }

    if(hsel != NULL)
        t = hsel->getTwin();

    if(v == vsel && (h == hsel || h == t) && f == fsel)
    {
        vsel = NULL;
        hsel = NULL;
        fsel = NULL;
    }else
    {
        vsel = v;
        hsel = h;
        fsel = f;
    }

    renderizaFront();
}

void Render::renderizaFront(void)
{
    QPainter p(frontBuffer);
    p.setCompositionMode(QPainter::CompositionMode_Source);
    p.fillRect(frontBuffer->rect(), Qt::transparent);
    p.end();

    if(mostraFace)
        renderizaFaces();

    if(mostraAresta)
        renderizaArestas();

    if(mostraPonto)
        renderizaVertices();

    verticeSelecionado();
    arestaSelecionada();
    faceSelecionada();
    // Faltou fazer para face externa caso precise

    atualizaScreen();
}

void Render::renderizaFaces()
{
    HalfEdge *partida;
    HalfEdge::iterator it;

    if(vsel != NULL)
        partida = vsel->getEdge();
    if(hsel != NULL)
        partida = hsel;
    if(fsel != NULL)
        partida = fsel->getOuterComp();

    if(vsel != NULL)
    {
        renderizaFace(partida, vizinhoScreen);
        for(it = partida->v_begin(); it != partida->v_end(); ++it)
        {
            renderizaFace(&it, vizinhoScreen);
        }
    }

    if(hsel != NULL)
    {
        renderizaFace(partida, vizinhoScreen);
        renderizaFace(partida->getTwin(), vizinhoScreen);
    }

    if(fsel != NULL)
    {
        renderizaFace(partida->getTwin(), vizinhoScreen);
        for(it = partida->f_begin(); it != partida->f_end(); ++it)
        {
            renderizaFace(it->getTwin(), vizinhoScreen);
        }
    }
}

void Render::renderizaArestas()
{
    QPainter buff(frontBuffer);
    QPoint p;
    HalfEdge *partida;
    HalfEdge::iterator it;

    buff.setPen(vizinhoScreen);

    if(vsel != NULL)
        partida = vsel->getEdge();
    if(hsel != NULL)
        partida = hsel;
    if(fsel != NULL)
        partida = fsel->getOuterComp();

    if(vsel != NULL || hsel != NULL)
    {
        for(it = partida->v_begin(); it != partida->v_end(); ++it)
        {
            buff.drawLine(transforma(it->getOrigem()->getPoint()), transforma(it->getDestino()->getPoint()));
        }
        if(hsel != NULL)
        {
            partida = partida->getTwin();
            for(it = partida->v_begin(); it != partida->v_end(); ++it)
            {
                buff.drawLine(transforma(it->getOrigem()->getPoint()), transforma(it->getDestino()->getPoint()));
            }
        }else
            buff.drawLine(transforma(partida->getOrigem()->getPoint()), transforma(partida->getDestino()->getPoint()));
    }
    if(fsel != NULL)
    {
        for(it = partida->f_begin(); it != partida->f_end(); ++it)
        {
            buff.drawLine(transforma(it->getOrigem()->getPoint()), transforma(it->getDestino()->getPoint()));
        }
        buff.drawLine(transforma(partida->getOrigem()->getPoint()), transforma(partida->getDestino()->getPoint()));
    }

}
void Render::renderizaVertices()
{
    QPainter buff(frontBuffer);
    QPoint p;
    HalfEdge *partida;
    HalfEdge::iterator it;

    buff.setPen(vizinhoScreen);

    if(vsel != NULL)
        partida = vsel->getEdge();
    if(hsel != NULL)
        partida = hsel;
    if(fsel != NULL)
        partida = fsel->getOuterComp();

    if(vsel != NULL || hsel != NULL)
    {
        for(it = partida->v_begin(); it != partida->v_end(); ++it)
        {
            buff.drawEllipse(transforma(it->getDestino()->getPoint()), 5,5);
        }
        if(hsel != NULL)
        {
            partida = partida->getTwin();
            for(it = partida->v_begin(); it != partida->v_end(); ++it)
            {
                buff.drawEllipse(transforma(it->getDestino()->getPoint()), 5,5);
            }
        }else
            buff.drawEllipse(transforma(partida->getDestino()->getPoint()), 5,5);
    }
    if(fsel != NULL)
    {
        for(it = partida->f_begin(); it != partida->f_end(); ++it)
        {
            buff.drawEllipse(transforma(it->getOrigem()->getPoint()), 5,5);
        }
        buff.drawEllipse(transforma(partida->getOrigem()->getPoint()), 5,5);
    }
}

void Render::reiniciaBuffers(int w, int h)
{
    if(buffer != NULL)
        delete buffer;
    if(backBuffer != NULL)
        delete backBuffer;
    if(frontBuffer != NULL)
        delete frontBuffer;

    buffer = new QImage(w, h, QImage::Format_ARGB32_Premultiplied);
    backBuffer = new QImage(w, h, QImage::Format_ARGB32_Premultiplied);
    frontBuffer = new QImage(w, h, QImage::Format_ARGB32_Premultiplied);


    QPainter p;
    p.begin(buffer);
    p.fillRect(buffer->rect(), Qt::white);
    p.end();
    p.begin(backBuffer);
    p.fillRect(backBuffer->rect(),Qt::white);
    p.end();
    p.begin(frontBuffer);
    p.setCompositionMode(QPainter::CompositionMode_Source);
    p.fillRect(frontBuffer->rect(), Qt::transparent);
    p.end();

    renderiza();
    renderizaFront();
}

void Render::renderizaFace(HalfEdge *h, QPen pen)
{
    QPainter buff(frontBuffer);
    QPainterPath *path;
    QPoint p;
    HalfEdge::iterator it;

    if(interface.isExterna(h->getFace()))
        return;

    if(pen == vizinhoScreen)
        qDebug() << "pintando face com vizinhoScreen";
    else if(pen == selecionadoScreen)
        qDebug() << "pintando face com seleciodoScreen";

    buff.setPen(pen);

    path = new QPainterPath();
    p = transforma(h->getOrigem()->getPoint());
    path->moveTo(p.x(),p.y());
    for(it = h->f_begin(); it !=  h->f_end(); ++it)
    {
        p = transforma(it->getOrigem()->getPoint());
        path->lineTo(p.x(),p.y());
    }
    p = transforma(h->getOrigem()->getPoint());
    path->lineTo(p.x(),p.y());

    buff.fillPath(*path,pen.brush());
    delete path;
}

void Render::verticeSelecionado()
{
    QPainter p(frontBuffer);

    if(vsel == NULL)
        return;

    p.setPen(selecionadoScreen);
    p.drawEllipse(transforma(vsel->getPoint()),5,5);

}
void Render::arestaSelecionada()
{
     QPainter p(frontBuffer);

    if(hsel == NULL)
        return;

    p.setPen(selecionadoScreen);
    p.drawLine(transforma(hsel->getOrigem()->getPoint()), transforma(hsel->getDestino()->getPoint()));

    qDebug() << "Desenhar de " << transforma(hsel->getOrigem()->getPoint()) << "a " << transforma(hsel->getDestino()->getPoint()) << "!";

}
void Render::faceSelecionada()
{
    if(fsel == NULL)
        return;

    renderizaFace(fsel->getOuterComp(), selecionadoScreen);
}
