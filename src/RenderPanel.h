#ifndef RENDERPANEL_H
#define RENDERPANEL_H

#include <QtGui>
#include <CommandQueue.h>

class MainWindow;

class RenderPanel : public QWidget
{
    Q_OBJECT

private slots:
    void update(const QImage &);
    void feedBackBondary(bool);

protected:
    void mouseReleaseEvent(QMouseEvent *event);

signals:
    void atualizaMain(void);
    void atualizaTamanho(int , int);
    void enviaArquivo(const QString &);

public:
    RenderPanel(CommandQueue *c, MainWindow *mw);
    void recebeArquivo(const QString &);

private:

    int screenW;
    int screenH;
    QLabel* fundolb;
    CommandQueue *cmdq;
    MainWindow *m_mw;
    
};

#endif
