#include <MainWindow.h>
#include <QPainter>
#include <Qt>
#include <CommandQueue.h>
#include <PlyParser.h>

MainWindow::MainWindow()
{
    centralpanel = new RenderPanel(new CommandQueue());
    setCentralWidget(centralpanel);

    tb = new QToolBar("Tool Bar Teste", this);
    fd = new QFileDialog(this, Qt::Window);
    
    
    connect(tb, SIGNAL(actionTriggered( QAction * )), this, SLOT(clicou(QAction*)));
    connect(fd, SIGNAL(fileSelected(const QString &)), this, SLOT(abriu(const QString &)));
    
    
    open = tb->addAction("T1");
    tb->addAction("T2");
    tb->addSeparator();
    tb->addAction("T3");

    addToolBar(Qt::LeftToolBarArea, tb);

    connect(centralpanel, SIGNAL(atualizaMain()), this, SLOT(update()));

    setWindowTitle("trab2FT 0.0.1");
    setFixedSize(800, 600);
}

void MainWindow::abriu(const QString &s)
{
    QVector<QPoint> tmp;
    PlyParser ps(s);
    tmp = ps.proximo();
    int i =0;
    while(tmp.size() > 0)
    {
        
        qDebug() << i++ << ":" << tmp;
        tmp = ps.proximo();
    }
    qDebug() << "FIM!";
}

void MainWindow::clicou(QAction* a)
{
    if(a == open) fd->open();
}

void MainWindow::update(void)
{
	repaint();
}
