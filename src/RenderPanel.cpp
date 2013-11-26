#include <RenderPanel.h>
#include <MainWindow.h>
#include <Render.h>

RenderPanel::RenderPanel(CommandQueue *c, MainWindow *mw)
{  
    cmdq = c;
    fundolb = new QLabel(this);

    screenW = 750;
    screenH = 550;

    Render *r = new Render(screenW, screenH, c);
    connect(r, SIGNAL(renderizado(const QImage &)), this, SLOT(update(const QImage &)), Qt::QueuedConnection);
    connect(r, SIGNAL(feedBackBondary(bool)),this, SLOT(feedBackBondary(bool)));
    connect(this, SIGNAL(atualizaTamanho(int, int)), r, SLOT(updateScreen(int, int)), Qt::QueuedConnection);
    connect(this, SIGNAL(enviaArquivo(const QString &)), r, SLOT(recebeArquivo(const QString &)), Qt::QueuedConnection);
    
    QImage tmp(screenW, screenH,QImage::Format_ARGB32_Premultiplied);
    QPainter p;
    p.begin(&tmp);
    p.fillRect(tmp.rect(), Qt::white);
    p.end();
    fundolb->setPixmap(QPixmap::fromImage(tmp));
    fundolb->show();

    m_mw = mw;

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
    m_mw->bondaryReset();
    if(event->x() >= 0 && event->y() >=0 && event->x() < fundolb->width() && event->y() < fundolb->height())
    {
        cmdq->produz(SELECT,event->x(), event->y());
    }
}

void RenderPanel::feedBackBondary(bool value)
{
    m_mw->bondaryFeedBack(value);
}
