#include <CommandQueue.h>
#include <QMutexLocker>
/*
ExCom & operator=(ExCom &e, const ExCom &d)
{
    e.cmd = d.cmd;
    e.x = d.x;
    e.y = d.y;
    
    return e;
}
*/
CommandQueue::CommandQueue()
{
    indPro = 0;
    indCon = 0;
    cmdList[0].cmd = NENHUM;
    cmdList[0].x = 0;
    cmdList[0].y = 0;
}

void CommandQueue::produz(Commands cmd, int x = 0, int y = 0)
{
     QMutexLocker locker(&m);
     
    cmdList[indPro].cmd = cmd;
    cmdList[indPro].x = x;
    cmdList[indPro].y = y;
    
    indPro = (indPro + 1)%MAXCOMMANDS;
    
    cmdList[indPro].cmd = NENHUM;
    cmdList[indPro].x = 0;
    cmdList[indPro].y = 0;
}

ExCom CommandQueue::consome()
{
    QMutexLocker locker(&m);
    ExCom cmd;
    cmd.cmd = cmdList[indCon].cmd;
    cmd.x = cmdList[indCon].x;
    cmd.x = cmdList[indCon].x;
    
    if(cmd.cmd != NENHUM)
        indCon = (indCon + 1)%MAXCOMMANDS;;
        
    return cmd;
}
