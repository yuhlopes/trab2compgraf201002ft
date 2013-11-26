#include <MainWindow.h>
#include <QPainter>
#include <Qt>
#include <CommandQueue.h>
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    fila = new CommandQueue();
    centralpanel = new RenderPanel(fila, this);
    setCentralWidget(centralpanel);

    connect(ui->toolBar, SIGNAL(actionTriggered( QAction * )), this, SLOT(clicou(QAction*)));

    connect(ui->actionBondary, SIGNAL(toggled(bool)), this, SLOT(bondaryClick(bool)));

    connect(centralpanel, SIGNAL(atualizaMain()), this, SLOT(update()));

    setFixedSize(800, 600);
}

void MainWindow::clicou(QAction* a)
{
    if(a != ui->actionBondary)
        bondaryReset();

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

void MainWindow::bondaryClick(bool value)
{
    ui->actionBondary->setIcon(QIcon(":/bond"));
    if(value)
        fila->produz(EXTERN);
}

void MainWindow::bondaryFeedBack(bool value)
{
    if(value)
        ui->actionBondary->setIcon(QIcon(":/true"));
    else
        ui->actionBondary->setIcon(QIcon(":/false"));
}

void MainWindow::bondaryReset()
{
    ui->actionBondary->setIcon(QIcon(":/bond"));
    ui->actionBondary->setChecked(false);
}
