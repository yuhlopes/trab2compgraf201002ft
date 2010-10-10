#include <Render.h>
#include <QPaintDevice>
#include <PlyParser.h>

bool operator<(const QPoint &v, const QPoint &u)
{
    if(v.x() == u.x())
        return v.y() < u.y();
    return v.x() < u.x();
}

Render::Render(int w, int h, CommandQueue *c) {

    screenW = w;
    screenH = h;
    cmdq = c;

    sel = NULL;
    
    screen = new QImage(w,h,QImage::Format_RGB32);
    zoom = 1.0;
    buffer = new QImage(screenW * zoom, screenH * zoom, QImage::Format_RGB32);
    backBuffer = new QImage(screenW * zoom, screenH * zoom, QImage::Format_RGB32);
    ponto = new QPoint(0,0);
}

void Render::run(void) {
    ExCom ex;
    
    do {
        if(!mostraPonto && !mostraAresta && !mostraFace)
        {
            if(sel != NULL)
                    delete sel;
        }

        ex = cmdq->consome();
        switch(ex.cmd)
        {
            case NENHUM:
                msleep(100);
                break;
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
                //renderiza
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
           delete buffer;
           delete backBuffer;
           buffer = new QImage(screenW * zoom, screenH * zoom, QImage::Format_RGB32);
           backBuffer = new QImage(screenW * zoom, screenH * zoom, QImage::Format_RGB32);
           //renderiza
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
    
    PlyParser ply(filename);
    
    tmp = ply.proximo();
    while(tmp.size() > 0)
    {
        //interface.addFace(tmp);
        tmp = ply.proximo();
    }
    //interface.finaliza();
    //renderiza;
    
    atualizaScreen();
 }
 
void Render::atualizaScreen(void)
{
    delete screen;
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
    if(ponto->x() - INCPOS > 0)
        ponto->setX(ponto->x() - INCPOS);
}
void Render::incY()
{
    if(ponto->y() + INCPOS + screenH < buffer->height())
        ponto->setY(ponto->y() + INCPOS);
}
void Render::decY()
{
    if(ponto->y() - INCPOS > 0)
        ponto->setY(ponto->y() - INCPOS);
}
void Render::incZoom()
{
    if(zoom + INCZ < ZOOMLIMIT)
        zoom += INCZ;
    else
        zoom = ZOOMLIMIT;

    delete buffer;
    delete backBuffer;
    buffer = new QImage(screenW * zoom, screenH * zoom, QImage::Format_RGB32);
    backBuffer = new QImage(screenW * zoom, screenH * zoom, QImage::Format_RGB32);
    //renderiza
}
void Render::decZoom()
{
    if(zoom - INCZ > 1.0)
        zoom -= INCZ;
    else
        zoom = 1.0;

    delete buffer;
    delete backBuffer;
    buffer = new QImage(screenW * zoom, screenH * zoom, QImage::Format_RGB32);
    backBuffer = new QImage(screenW * zoom, screenH * zoom, QImage::Format_RGB32);
    //renderiza
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

    //xwmin = interface.getMinX();
    if(xwmin > xmin)
        xwmin = xmin;
    //xwmax = interface.getMaxX();
    if(xwmax < xmax)
        xwmax = xmax;
    //ywmin = interface.getMinY();
    if(ywmin > ymin)
        ywmin = ymin;
    //ywmax = interface.getMaxY();
    if(ywmax < ymax)
        ywmax = ymax;

    sx = (xmax - xmin)/(xwmax - xwmin);
    sy = (ymax - ymin)/(ywmax - ywmin);

    if(sx < sy)
        sy = sx;
    else if(sx > sy)
        sx = sy;

    QPoint p(sx*(in.x() - xwmin) + xmin, sy*(in.y() - ywmin) + ymin);
    map[p] = in;
    return p;
}
QPoint Render::destransforma(const QPoint &in)
{
    QPoint p;
    if(map.find(in) != map.end())
        return map[in];
    else
        return p;
}

void Render::renderiza(void)
{
    QMap<QPoint, QPoint> m;
    QPoint p1, p2;

    QMap<QPoint,QPoint>::Iterator i;
    QPainter painter(buffer);
    QPen pen;
    QColor cor(255,255,255,0);
    pen.setColor(cor);

    painter.fillRect(0,0,buffer->width(), buffer->height(),cor);

    for(i = m.begin(); i != m.end(); ++i)
    {
        p1 = transforma(i.key());
        p2 = transforma(i.value());
        painter.drawLine(p1,p2);
    }
}
