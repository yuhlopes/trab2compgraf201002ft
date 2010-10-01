#ifndef RENDERPANEL_H
#define RENDERPANEL_H

#include <QtGui>
#include <Render.h>

class RenderPanel : public QWidget
{
    Q_OBJECT

private slots:
    void update(void);

signals:
    void atualizaMain(void);
    void sendNewPainter(QPainter *p);

public:
    RenderPanel(CommandQueue *c);

private:

    QImage* buffer;
    QLabel* fundolb;
    CommandQueue *cmdq;
    
};

#endif
