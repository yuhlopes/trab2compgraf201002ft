#include <MainWindow.h>
#include <QPainter>
#include <Qt>
#include <CommandQueue.h>

MainWindow::MainWindow()
{
    centralpanel = new RenderPanel(new CommandQueue());
    setCentralWidget(centralpanel);

    QToolBar * tb = new QToolBar("Tool Bar Teste", this);
    tb->addAction("T1");
    tb->addAction("T2");
    tb->addSeparator();
    tb->addAction("T3");

    addToolBar(Qt::LeftToolBarArea, tb);

    connect(centralpanel, SIGNAL(atualizaMain()), this, SLOT(update()));

    setWindowTitle("trab2FT 0.0.1");
    setFixedSize(800, 600);
}

void MainWindow::update(void)
{
	repaint();
}
