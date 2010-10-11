#ifndef RENDER_H
#define RENDER_H

#include <QThread>
#include <QPixmap>
#include <QTimer>
#include <QPainter>
#include <QDebug>
#include <CommandQueue.h>
#include <Interface.h>

#define INCPOS      20
#define INCZ        0.4
#define ZOOMLIMIT   5.0
#define MARGEM      0.05

class Render : public QThread
{
	Q_OBJECT

signals:
        void renderizado(const QImage &screen);

        
public slots:
    void updateScreen(int w, int y);
    void recebeArquivo(const QString &);

                
public:
        Render(int w, int y, CommandQueue *c);

    void run(void);

private:
        void atualizaScreen(void);
        void incX();
        void decX();
        void incY();
        void decY();
        void incZoom();
        void decZoom();
        QPoint transforma(const QPoint &);
        QPoint destransforma(const QPoint &);
        void renderiza(void);

	    
        QImage* screen;
        QImage* buffer;
        QImage* backBuffer;
        double zoom;
        CommandQueue *cmdq;
        QPoint *ponto;
        QPoint *sel;
        int screenW;
        int screenH;
        bool mostraPonto;
        bool mostraAresta;
        bool mostraFace;
        Interface interface;
        QMap<QPoint , QPoint> map;
        
};

#endif
