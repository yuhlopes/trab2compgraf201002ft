#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include <RenderPanel.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT
   
public slots:
    void update(void);

public:
    MainWindow();
    
private:
   RenderPanel* centralpanel;

};


#endif
