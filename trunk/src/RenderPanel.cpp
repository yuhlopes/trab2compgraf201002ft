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
    
    QImage tmp(screenW, screenH,QImage::Format_RGB32);
    fundolb->setPixmap(QPixmap::fromImage(tmp));
    fundolb->show();

    repaint();
    r->start();
}

void RenderPanel::update(const QImage& screen)
{
    //qDebug() << "Renderizando";
    //qDebug() << "#" << screen.width() << "x" << screen.height() <<"#";
    fundolb->setPixmap(QPixmap::fromImage(screen));
    fundolb->show();
    repaint();
    emit atualizaMain();
}

void RenderPanel::recebeArquivo(const QString &filename)
{
    emit enviaArquivo(filename);
}
