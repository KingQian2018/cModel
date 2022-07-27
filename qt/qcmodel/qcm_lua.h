#ifndef _QCM_LUA_H
#define _QCM_LUA_H

#include "lua.hpp"

#include <QThread>

class QCM_Lua : public QThread
{
    Q_OBJECT
private:
    lua_State *m_L;

public:
    QCM_Lua(int argc, char **argv, QObject *parent = nullptr);
    ~QCM_Lua();

protected:
    void run() override;
};

#endif
