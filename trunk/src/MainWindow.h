#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include <RenderPanel.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT
   
public slots:
    void update(void);
    void clicou(QAction* a);

public:
    MainWindow();
    
private:
   CommandQueue *fila;
   RenderPanel* centralpanel;
   QFileDialog *fd;
   QAction *open;
   QToolBar * tb;

};


#endif
