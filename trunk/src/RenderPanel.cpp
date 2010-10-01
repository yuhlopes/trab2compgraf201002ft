#include <RenderPanel.h>

RenderPanel::RenderPanel(CommandQueue *c)
{  
    cmdq = c;
    buffer = new QImage(750,550, QImage::Format_RGB32);
    fundolb = new QLabel(this);
    
    Render *r = new Render(new QPainter(buffer), c);
    connect(r, SIGNAL(renderizado()), this, SLOT(update()), Qt::QueuedConnection);
    connect(this, SIGNAL(sendNewPainter(QPainter *)), r, SLOT(updatePainter(QPainter* )), Qt::QueuedConnection);
    
    fundolb->setPixmap(QPixmap::fromImage(*buffer));
    fundolb->show();
    
    repaint();
    r->start();
}

void RenderPanel::update(void)
{
    fundolb->setPixmap(QPixmap::fromImage(*buffer));
    fundolb->show();
    repaint();
    emit atualizaMain();
}
