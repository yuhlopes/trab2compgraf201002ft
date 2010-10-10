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
    void abriu(const QString &s);

public:
    MainWindow();
    
private:
   RenderPanel* centralpanel;
   QFileDialog *fd;
   QAction *open;
   QToolBar * tb;

};


#endif
