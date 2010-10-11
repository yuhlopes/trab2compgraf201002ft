#include <Render.h>
#include <QPaintDevice>
#include <PlyParser.h>


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
                renderiza();
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
    //interface.addExtEdges();
    qDebug() << "Completou";
    renderiza();
    qDebug() << "Renderizou";
    
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
    qDebug() << "Deletou";
    buffer = new QImage(screenW * zoom, screenH * zoom, QImage::Format_RGB32);
    backBuffer = new QImage(screenW * zoom, screenH * zoom, QImage::Format_RGB32);
    qDebug() << "Criou";
    renderiza();
}
void Render::decZoom()
{
    int w = screenW;
    int h = screenH;

    if(zoom - INCZ > 1.0)
        zoom -= INCZ;
    else
        zoom = 1.0;

    delete buffer;
    delete backBuffer;
    buffer = new QImage(screenW * zoom, screenH * zoom, QImage::Format_RGB32);
    backBuffer = new QImage(screenW * zoom, screenH * zoom, QImage::Format_RGB32);


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
    if(map.find(in) != map.end())
        return map[in];
    else
        return p;
}

void Render::renderiza(void)
{
    QPoint p1, p2;
    QList<QPair<QPoint,QPoint> > lista = interface.getArestas();
    QPainter painter(buffer);
    QPen pen;
    QColor cor(255,255,255,255);
    pen.setColor(cor);
    map.clear();

    painter.fillRect(0,0,buffer->width(), buffer->height(),cor);

    qDebug() <<  "Tamanho Buffer: " << buffer->width() << " x " << buffer->height();
    for(int i = 0; i < lista.size() ; ++i)
    {
        qDebug() << lista[i];
        p1 = transforma(lista[i].first);
        p2 = transforma(lista[i].second);
        qDebug() << "Transformado: " << p1 << " - " << p2;
        painter.drawLine(p1,p2);
    }
}
