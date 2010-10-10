/* 
 * File:   ComandQueue.h
 * Author: thiago
 *
 * Created on 25 de Setembro de 2010, 14:36
 */

#ifndef _COMMANDQUEUE_H
#define	_COMMANDQUEUE_H

#define MAXCOMMANDS  20

#include <QObject>
#include <QMutex>

enum Commands{
    NENHUM,
    INCZOOM,
    DECZOOM,
    INCX,
    DECX,
    INCY,
    DECY,
    PONTOS,
    ARESTAS,
    FACES,
    SELECT
};

typedef struct _ExCom{
    Commands cmd;
    int x;
    int y;
}ExCom;

class CommandQueue : public QObject
{
    Q_OBJECT
public:
    CommandQueue();
    
    ExCom consome();
    void produz(Commands, int, int);

private:
    ExCom cmdList[MAXCOMMANDS];
    QMutex m;
    int indCon;
    int indPro;
        
};

#endif	/* _COMMANDQUEUE_H */

