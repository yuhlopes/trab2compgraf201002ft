#include <Render.h>
#include <QPaintDevice>

Render::Render(QPainter* p, CommandQueue *c) {
    painter = p;
    cmdq = c;
    i = j = 0;
    r = g = b = 0;
}

void Render::run(void) {
    int incr = 8;
    int incg = 8;
    int incb = 8;
    bool pagina = false;
    do {

        if (i + 25 >= (painter->device()->width())) {
            //qDebug() << "Fim de linha";
            if (j + 25 >= (painter->device()->height())) {
                //qDebug() << "Fim de Pagina";
                pagina = true;
                emit renderizado();
                //sai = true;
                if (r + incr > 255 || r + incr < 0) {
                    incr*=-1;
                    r += incr;
                    if (g + incg > 255 || g + incg < 0) {
                        incg*=-1;
                        g += incg;
                        if (b + incb > 255 || b + incb < 0) {
                            incb*=-1;
                            b += incb;
                        }else b += incb;
                    }else g += incg;
                }else r += incr;
                j = 0;
            }else j += 25;
            i = 0;
        }else i += 25;

        painter->fillRect(i, j, 25, 25, QColor(r, g, b));

        if (pagina) {
            pagina = false;
            msleep(33);
        }
    } while (true);

}

void Render::updatePainter(QPainter* p)
{
    if(painter != NULL)
        delete painter;
    painter = p;
}
