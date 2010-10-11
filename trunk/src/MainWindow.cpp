#include <MainWindow.h>
#include <QPainter>
#include <Qt>
#include <CommandQueue.h>

MainWindow::MainWindow()
{
    QIcon zi(":zoom-in");
    QIcon zo(":zoom-out");
    QIcon op(":open");
    QIcon al(":arrow-left");
    QIcon au(":arrow-up");
    QIcon ar(":arrow-rigth");
    QIcon ad(":arrow-down");

    fila = new CommandQueue();
    centralpanel = new RenderPanel(fila);
    setCentralWidget(centralpanel);

    tb = new QToolBar("Tool Bar Teste", this);
    fd = new QFileDialog(this, Qt::Window);
    
    
    connect(tb, SIGNAL(actionTriggered( QAction * )), this, SLOT(clicou(QAction*)));
    connect(fd, SIGNAL(fileSelected(const QString &)), centralpanel, SLOT(recebeArquivo(const QString &)));
    
    
    open = tb->addAction(op,"");
    tb->addSeparator();
    zoomIn = tb->addAction(zi, "");
    zoomOut = tb->addAction(zo, "");
    tb->addSeparator();
    panU = tb->addAction(au, "");
    panL = tb->addAction(al, "");
    panR = tb->addAction(ar, "");
    panD = tb->addAction(ad, "");

    addToolBar(Qt::LeftToolBarArea, tb);

    connect(centralpanel, SIGNAL(atualizaMain()), this, SLOT(update()));

    setWindowTitle("trab2FT 0.0.1");
    setFixedSize(800, 600);
}

void MainWindow::clicou(QAction* a)
{
    if(a == open)
        fd->open();
    else if(a == zoomIn)
    {
        qDebug() << "Antes";
        fila->produz(INCZOOM);
        qDebug() << "Depois";
    }else if (a == zoomOut)
    {
        fila->produz(DECZOOM);
    }else if (a == panU)
    {
        fila->produz(DECY);
    }else if (a == panL)
    {
        fila->produz(DECX);
    }else if (a == panD)
    {
        fila->produz(INCY);
    }else if(a == panR)
    {
        fila->produz(INCX);
    }
}

void MainWindow::update(void)
{
	repaint();
}
