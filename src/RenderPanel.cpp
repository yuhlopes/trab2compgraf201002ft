#include <RenderPanel.h>

RenderPanel::RenderPanel(CommandQueue *c)
{  
    cmdq = c;
    fundolb = new QLabel(this);

    screenW = 750;
    screenH = 550;

    Render *r = new Render(screenW, screenH, c);
    connect(r, SIGNAL(renderizado(const QImage &)), this, SLOT(update(const QImage &)), Qt::QueuedConnection);
    connect(this, SIGNAL(atualizaTamanho(int, int)), r, SLOT(updateScreen(int, int)), Qt::QueuedConnection);
    connect(this, SIGNAL(enviaArquivo(const QString &)), r, SLOT(recebeArquivo(const QString &)), Qt::QueuedConnection);
    
    QImage tmp(screenW, screenH,QImage::Format_ARGB32_Premultiplied);
    QPainter p;
    p.begin(&tmp);
    p.fillRect(tmp.rect(), Qt::white);
    p.end();
    fundolb->setPixmap(QPixmap::fromImage(tmp));
    fundolb->show();

    repaint();
    r->start();
}

void RenderPanel::update(const QImage& screen)
{
    fundolb->setPixmap(QPixmap::fromImage(screen));
    fundolb->show();
    repaint();
    emit atualizaMain();
}

void RenderPanel::recebeArquivo(const QString &filename)
{
    emit enviaArquivo(filename);
}

void RenderPanel::mouseReleaseEvent(QMouseEvent *event)
{
    qDebug() << "mouseReleaseEvent: x=" << event->x() << ", y=" << event->y();
    cmdq->produz(SELECT,event->x(), event->y());
}
