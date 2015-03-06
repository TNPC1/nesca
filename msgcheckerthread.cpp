#include "msgcheckerthread.h"
#include "externData.h"
#include "mainResources.h"
#include <Utils.h>
#include <Connector.h>

void MSGCheckerThread::doEmitionShowNewMsg(QString str)
{
	emit mct->showNewMsg(str);
}

void _getNewMsg()
{
    char request[256] = {0};
    sprintf(request, "http://nesca.d3w.org/mailbox?key=%s", trcPersKey);

    std::string buffer;
    std::vector<std::string> headerVector {"X-Nescav3: True"};
    Connector::nConnect(request, 80, &buffer, NULL, &headerVector);

    char *ptr1 = NULL;
    if(buffer.size() > 0)
	{
        if(Utils::ci_find_substr(buffer, std::string("\r\n\r\n")) != -1
                && Utils::ci_find_substr(buffer, std::string("HTTP/1.1 404 Not Found")) == -1
                && Utils::ci_find_substr(buffer, std::string("HTTP/1.1 502 Bad Gateway")) == -1
                && Utils::ci_find_substr(buffer, std::string("HTTP/1.1 400 Bad Request")) == -1
                && Utils::ci_find_substr(buffer, std::string("\r\n\r\nEmpty")) == -1
                )
        {
                ptr1 = strstr((char*)buffer.c_str(), "\r\n\r\n");
                if(strlen(ptr1 + 4) != 0)
                {
                    mct->doEmitionShowNewMsg(QString(ptr1 + 4));
                };
        }
	};
}

void MSGCheckerThread::run() 
{
	for(;;)
	{
		Sleep(60000);
		_getNewMsg();
	};
}
