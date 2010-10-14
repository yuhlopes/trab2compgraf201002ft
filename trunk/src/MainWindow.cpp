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
    QIcon a(":aresta");
    QIcon v(":vertice");
    QIcon f(":face");

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
    tb->addSeparator();
    vertice = tb->addAction(v,"");
    aresta = tb->addAction(a,"");
    face = tb->addAction(f, "");

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
        qDebug() << "Antes";
        fila->produz(DECZOOM);
        qDebug() << "Depois";
    }else if (a == panU)
    {
        qDebug() << "Antes";
        fila->produz(DECY);
        qDebug() << "Depois";
    }else if (a == panL)
    {
        qDebug() << "Antes";
        fila->produz(DECX);
        qDebug() << "Depois";
    }else if (a == panD)
    {
        qDebug() << "Antes";
        fila->produz(INCY);
        qDebug() << "Depois";
    }else if(a == panR)
    {
        qDebug() << "Antes";
        fila->produz(INCX);
        qDebug() << "Depois";
    }else if(a == vertice)
    {
        qDebug() << "Antes";
        fila->produz(PONTOS);
        qDebug() << "Depois";
    }else if(a == aresta)
    {
        qDebug() << "Antes";
        fila->produz(ARESTAS);
        qDebug() << "Depois";
    }else if(a == face)
    {
        qDebug() << "Antes";
        fila->produz(FACES);
        qDebug() << "Depois";
    }
}

void MainWindow::update(void)
{
	repaint();
}
