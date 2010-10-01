#ifndef RENDER_H
#define RENDER_H

#include <QThread>
#include <QPixmap>
#include <QTimer>
#include <QPainter>
#include <QDebug>
#include <CommandQueue.h>

class Render : public /*QObject*/QThread
{
	Q_OBJECT

signals:
        void renderizado(/*const QPixmap *px*/);
        
public slots:
    void updatePainter(QPainter* p);
                
public:
	Render(QPainter * p, CommandQueue *c);
//public slots:
    void run(void);

private:
	//QPixmap *qp;
        //QTimer *tm;
        QPainter* painter;
        int i,j;
        int r,g,b;
        CommandQueue *cmdq;
        

};

#endif
