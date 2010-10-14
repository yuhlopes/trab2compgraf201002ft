#include <Render.h>
#include <QPaintDevice>
#include <PlyParser.h>
#include <QRgb>


Render::Render(int w, int h, CommandQueue *c) {

    screenW = w;
    screenH = h;
    cmdq = c;

    sel = NULL;
    
    screen = new QImage(w,h,QImage::Format_RGB32);
    //zoom = 1.0;
    //buffer = new QImage(screenW * zoom, screenH * zoom, QImage::Format_RGB32);
    //backBuffer = new QImage(screenW * zoom, screenH * zoom, QImage::Format_RGB32);
    ponto = new QPoint(0,0);

    corArestaFina = qRgb(0,0,255);
    corArestaGrossa = qRgb(255,255,0);
    corVerticeFino = qRgb(0, 255,255);
    corVerticeGrosso = qRgb(0,255,0);
    corFace = qRgb(255,255,255);
    corFaceExt = qRgb(0,0,0);


    arestaScreen.setColor(QColor(0,0,0,255));
    selecionadoScreen.setColor(QColor(0,128,128,255));
    //faceSelecionadaScreen;
    faceExternaBack.setColor(corFaceExt);
    arestaGrossaBack.setColor(corArestaGrossa);
    verticeGrossoBack.setColor(corVerticeGrosso);
    arestafinaBack.setColor(corArestaFina);
    verticefinoBack.setColor(corVerticeFino);
    arestaGrossaBack.setWidth(10);
    verticeGrossoBack.setWidth(10);
}

void Render::run(void) {
    ExCom ex;
    
    do {
        //if(!mostraPonto && !mostraAresta && !mostraFace)
        //{
        //    if(sel != NULL)
        //            delete sel;
        //}

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
                seleciona();
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

    sel = NULL;

    zoom = 1.0;
    ponto->setX(0);
    ponto->setY(0);
    if(buffer != NULL)
    {
        delete buffer;
        delete backBuffer;
    }
    buffer = new QImage(screenW * zoom, screenH * zoom, QImage::Format_RGB32);
    backBuffer = new QImage(screenW * zoom, screenH * zoom, QImage::Format_RGB32);

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
    screen = new QImage((backBuffer->copy(ponto->x(), ponto->y(),screenW,screenH)));
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

    if(zoom == 1.0)
        return;

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
    QMap<QPoint, QPoint>::Iterator it = map.find(in);
    if(it != map.end())
        return it.value();
    else
        return p;
}

void Render::renderiza(void)
{
    QPoint p1, p2;
    QList<QPair<QPoint,QPoint> > lista = interface.getArestas();
    QColor branco(255,255,255,255);
    QPainter buff(buffer);
    QPainter back(backBuffer);
    QBrush b(branco);

    buff.fillRect(0,0,buffer->width(), buffer->height(),branco);
    back.fillRect(0,0,buffer->width(), buffer->height(),branco);

    buff.setPen(arestaScreen);

    map.clear();
    for(int i = 0; i < lista.size() ; ++i)
    {
        p1 = transforma(lista[i].first);
        p2 = transforma(lista[i].second);

        buff.drawLine(p1,p2);

        back.setPen(arestaGrossaBack);
        back.drawLine(p1,p2);
        back.setPen(arestafinaBack);
        back.drawLine(p1,p2);
        back.setPen(verticeGrossoBack);
        back.drawEllipse(p1,5,5);
        back.drawEllipse(p2,5,5);
        back.setPen(verticefinoBack);
        back.drawPoint(p1);
        back.drawPoint(p2);

    }
    //for(int i = 0; i < lista.size() ; ++i)
    //{
    //    //qDebug() << lista[i];
    //    p1 = transforma(lista[i].first);
    //    p2 = transforma(lista[i].second);

    //
    //    back.setPen(verticefinoBack);
    //    back.drawPoint(p1);
    //    back.drawPoint(p2);
    // }
}

void Render::seleciona(void)
{
    qDebug() << "selciona: x=" << sel->x() << ", y=" << sel->y();
    sel->setX(sel->x() + ponto->x());
    sel->setY(sel->y() + ponto->y());

    QRgb rgb = backBuffer->pixel(*sel);

    if(rgb == corVerticeFino)
    {
        qDebug() << "Clicou Ponto Fino";
    }else if(rgb == corVerticeGrosso)
    {
        qDebug() << "Clicou Ponto Grosso";
        QPoint fonte = buscaPontoFino(*sel);
        qDebug() << "Fonte: " << fonte;
    }else if(rgb == corArestaFina)
    {
        qDebug() << "Clicou aresta Fina";
    }
    else if(rgb == corArestaGrossa)
    {
        qDebug() << "Clicou aresta grossa";
    }else if(rgb == corFace)
    {
        qDebug() << "Clicou Face";
    }else if(rgb == corFaceExt)
    {
        qDebug() << "Clicou Face Externa";
    }

    //int r = qRed(rgb);
    //int g = qGreen(rgb);
    //int b = qBlue(rgb);

    //arestafinaBack.
}

QPoint Render::buscaPontoFino(QPoint ent)
{
    QRgb cor;
    QPoint ret;

    int incx = 1;
    int i = ent.x();
    int j = ent.y();
    while(true)
    {
        cor = backBuffer->pixel(i,j);
        if(cor == corVerticeFino)
        {
            ret.setX(i);
            ret.setY(j);
            return ret;
        }else if(cor != corVerticeGrosso)
        {
            j++;
            incx=-incx;
            i+=incx;
            cor = backBuffer->pixel(i,j);
            if(cor != corVerticeGrosso)
            {
                break;
            }
         }else
             i+= incx;
     }


    incx = -1;
    i = ent.x()-1;
    j = ent.y();
    while(true)
    {
        cor = backBuffer->pixel(i,j);
        if(cor == corVerticeFino)
        {
            ret.setX(i);
            ret.setY(j);
            return ret;
        }else if(cor != corVerticeGrosso)
        {
            j--;
            incx=-incx;
            i+=incx;
            cor = backBuffer->pixel(i,j);
            if(cor != corVerticeGrosso)
            {
                break;
            }
         }else
             i+= incx;
     }
    qDebug() << "FUUUU!";

    return ret;
}
QPair<QPoint, QPoint> Render::buscaPontoGrosso(QPoint ent)
{
}
QPair<QPair<QPoint, QPoint> , QPair<QPoint, QPoint> > Render::buscaArestaFina(QPoint ent)
{
}
QPair<QPoint, QPoint> Render::buscaArestaGrossa()
{
}
