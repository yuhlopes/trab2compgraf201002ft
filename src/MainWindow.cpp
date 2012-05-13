#include <MainWindow.h>
#include <QPainter>
#include <Qt>
#include <CommandQueue.h>
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    fila = new CommandQueue();
    centralpanel = new RenderPanel(fila);
    setCentralWidget(centralpanel);

    connect(ui->toolBar, SIGNAL(actionTriggered( QAction * )), this, SLOT(clicou(QAction*)));

    connect(centralpanel, SIGNAL(atualizaMain()), this, SLOT(update()));

    setFixedSize(800, 600);
}

void MainWindow::clicou(QAction* a)
{
    if(a == ui->actionOpen_File)
    {
        QString fn = QFileDialog::getOpenFileName(this, "Abrir Malha", "../..", "*.ply" );
        if(!fn.isEmpty())
            centralpanel->recebeArquivo(fn);
    }
    else if(a == ui->actionZoom_In)
    {
        fila->produz(INCZOOM);
    }else if (a == ui->actionZoom_Out)
    {
        fila->produz(DECZOOM);
    }else if (a == ui->actionUp)
    {
        fila->produz(DECY);
    }else if (a == ui->actionLeft)
    {
        fila->produz(DECX);
    }else if (a == ui->actionDown)
    {
        fila->produz(INCY);
    }else if(a == ui->actionRight)
    {
        fila->produz(INCX);
    }else if(a == ui->actionVertice)
    {
        fila->produz(PONTOS);
    }else if(a == ui->actionAresta)
    {
        fila->produz(ARESTAS);
    }else if(a == ui->actionFace)
    {
        fila->produz(FACES);
    }else if(a == ui->actionConvex_Hull)
    {
        fila->produz(CONVHULL);
    }
}

void MainWindow::update(void)
{
	repaint();
}
