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
    
    zoom = 1.0;
    screen = new QImage(w,h,QImage::Format_RGB32);
    ponto = new QPoint(0,0);

    corArestaGrossa = qRgb(255,255,0);
    corVerticeGrosso = qRgb(0,255,0);
    corFace = qRgb(255,255,255);
    corFaceExt = qRgb(0,0,0);

    arestaScreen.setColor(QColor(0,0,0,255));
    selecionadoScreen.setColor(QColor(0,128,128,128));
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
                break;
            case ARESTAS:
                mostraAresta = !mostraAresta;
                break;
            case FACES:
                mostraFace = !mostraFace;
                break;
            case SELECT:
                if(sel != NULL)
                    delete sel;
                sel = new QPoint(ex.x,ex.y);
                selecionados();
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
        if(buffer == NULL)
        {
           delete buffer;
           delete backBuffer;
           delete frontBuffer;
        }
        buffer = new QImage(screenW * zoom, screenH * zoom, QImage::Format_RGB32);
        backBuffer = new QImage(screenW * zoom, screenH * zoom, QImage::Format_RGB32);
        frontBuffer = new QImage(screenW * zoom, screenH * zoom, QImage::Format_RGB32);
        renderiza();
    }

    if(ponto->x() + w > buffer->width())
        ponto->setX(buffer->width() - w);
    if(ponto->y() + h > buffer->height())
        ponto->setY(buffer->height() - h);

    atualizaScreen();
}

void Render::recebeArquivo(const QString &filename)
{
    QVector<QPoint> tmp;

    if(sel != NULL)
    {
        delete sel;
        sel = NULL;
    }

    mostraAresta = false;
    mostraFace = false;
    mostraPonto = false;

    zoom = 1.0;
    ponto->setX(0);
    ponto->setY(0);
    if(buffer != NULL)
    {
        delete buffer;
        delete backBuffer;
        delete frontBuffer;
    }
    buffer = new QImage(screenW * zoom, screenH * zoom, QImage::Format_RGB32);
    backBuffer = new QImage(screenW * zoom, screenH * zoom, QImage::Format_RGB32);
    frontBuffer = new QImage(screenW * zoom, screenH * zoom, QImage::Format_RGB32);


    //interface.clear();

    PlyParser ply(filename);
    
    tmp = ply.proximo();
    while(tmp.size() > 0)
    {
        qDebug() << tmp;
        interface.addFace(tmp);
        qDebug() << "Passou";
        tmp = ply.proximo();
    }
    qDebug() << "Saiu";
    interface.addExtEdges();
    qDebug() << "Completou";
    renderiza();
    qDebug() << "Renderizou";
    
    atualizaScreen();
 }
 
void Render::atualizaScreen(void)
{
    delete screen;
    //fazer o sprite do frontBuffer por cima do buffer e passar para screen
    screen = new QImage((buffer->copy(ponto->x(), ponto->y(),screenW,screenH)));

    emit renderizado(*screen);
}

void Render::incX()
{
    if(ponto->x() + INCPOS + screenW < buffer->width())
        ponto->setX(ponto->x() + INCPOS);
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
    if(ponto->y() + INCPOS + screenH < buffer->height())
        ponto->setY(ponto->y() + INCPOS);
}
void Render::decY()
{
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

    if(buffer != NULL)
    {
        delete buffer;
        delete backBuffer;
        delete frontBuffer;
    }
    //qDebug() << "Deletou";
    buffer = new QImage(screenW * zoom, screenH * zoom, QImage::Format_RGB32);
    backBuffer = new QImage(screenW * zoom, screenH * zoom, QImage::Format_RGB32);
    frontBuffer = new QImage(screenW * zoom, screenH * zoom, QImage::Format_RGB32);
    //qDebug() << "Criou";
    renderiza();
}
void Render::decZoom()
{
    int w = screenW;
    int h = screenH;

    if(zoom == 1.0)
        return;

    if(zoom - INCZ > 1.0)
        zoom -= INCZ;
    else
        zoom = 1.0;

    if(buffer == NULL)
    {
        delete buffer;
        delete backBuffer;
        delete frontBuffer;
    }
    buffer = new QImage(screenW * zoom, screenH * zoom, QImage::Format_RGB32);
    backBuffer = new QImage(screenW * zoom, screenH * zoom, QImage::Format_RGB32);
    frontBuffer = new QImage(screenW * zoom, screenH * zoom, QImage::Format_RGB32);



    if(ponto->x() + w > buffer->width())
        ponto->setX(buffer->width() - w);
    if(ponto->y() + h > buffer->height())
        ponto->setY(buffer->height() - h);

    renderiza();
}

QPoint Render::transforma(const QPoint &in)
{
    int xwmax, xwmin, ywmax, ywmin;
    int xmax, xmin, ymax, ymin;
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

    QPoint p(x, y);
    map[p] = in;
    return p;
}
QPoint Render::destransforma(const QPoint &in)
{
    QPoint p;
    QMap<QPoint, QPoint>::Iterator it = map.find(in);
    if(it != map.end())
        return it.value();
    else
        return p;
}

void Render::renderiza(void)
{
    QPoint p1, p2;
    QList<QPair<QPoint,QPoint> > lista = interface.getTodasArestas();
    QPainter buff(buffer);
    QPainter back(backBuffer);
    //QBrush b(branco);

    buff.fillRect(0,0,buffer->width(), buffer->height(),QColor(corFace));
    back.fillRect(0,0,buffer->width(), buffer->height(),QColor(corFace));

    buff.setPen(arestaScreen);

    map.clear();
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

void Render::selecionados(void)
{
    qDebug() << "selciona: x=" << sel->x() << ", y=" << sel->y();
    sel->setX(sel->x() + ponto->x());
    sel->setY(sel->y() + ponto->y());

    QRgb rgb = backBuffer->pixel(*sel);
    QPoint p = destransforma(*sel);

    // Apagar o frontBuffer e pinta-lo de alfa

    if(mostraFace)
    {
        if(rgb == corVerticeGrosso)
        {
            renderizaFaces(interface.getFacesNearVertice(interface.getVerticeNear(p)));
        }else if(rgb == corArestaGrossa)
        {
            renderizaFaces(interface.getFacesNearAresta(interface.getArestaNear(p)));
        }else if(rgb == corFace)
        {
            renderizaFaces(interface.getFacesNearFace(interface.getFaceNear(p)));
        }
    }

    if(mostraAresta)
    {
        if(rgb == corVerticeGrosso)
        {
            renderizaArestas(interface.getArestasNearVertice(interface.getVerticeNear(p)));
        }else if(rgb == corArestaGrossa)
        {
            renderizaArestas(interface.getArestasNearAresta(interface.getArestaNear(p)));
        }else if(rgb == corFace)
        {
            renderizaArestas(interface.getArestasNearFace(interface.getFaceNear(p)));
        }
    }

    if(mostraPonto)
    {
        if(rgb == corVerticeGrosso)
        {
            renderizaVertices(interface.getVerticesNearVertice(interface.getVerticeNear(p)));
        }else if(rgb == corArestaGrossa)
        {
            renderizaVertices(interface.getVerticesNearAresta(interface.getArestaNear(p)));
        }else if(rgb == corFace)
        {
            renderizaVertices(interface.getVerticesNearFace(interface.getFaceNear(p)));
        }
    }

    // Faltou fazer para face externa caso precise

    atualizaScreen();
}

void Render::renderizaFaces(QList<QList<QPoint> > lista)
{
    // implementacao aqui
    // desenhar no frontBuffer

}
void Render::renderizaArestas(QList<QPair<QPoint,QPoint> > lista)
{
    // implementacao aqui
    // desenhar no frontBuffer
}
void Render::renderizaVertices(QList<QPoint> lista)
{
    // implementacao aqui
    // desenhar no frontBuffer
}
