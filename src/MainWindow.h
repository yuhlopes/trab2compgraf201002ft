#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include <RenderPanel.h>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
   
public slots:
    void update(void);
    void clicou(QAction* a);

public:
    MainWindow(QWidget* parent = 0);
    
private:
   CommandQueue *fila;
   RenderPanel* centralpanel;

    Ui::MainWindow *ui;
};


#endif
