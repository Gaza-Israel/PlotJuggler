/*DataStreamServer PlotJuggler  Plugin license(Faircode)

Copyright(C) 2018 Philippe Gauthier - ISIR - UPMC
Permission is hereby granted to any person obtaining a copy of this software and
associated documentation files(the "Software"), to deal in the Software without
restriction, including without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and / or sell copies("Use") of the Software, and to permit persons
to whom the Software is furnished to do so. The above copyright notice and this permission
notice shall be included in all copies or substantial portions of the Software. THE
SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
#pragma once

#include <QUdpSocket>
#include <QtPlugin>
#include <thread>
#include "PlotJuggler/datastreamer_base.h"
#include "PlotJuggler/messageparser_base.h"

#include <QThread>

using namespace PJ;


class WorkerThread;
class UDP_Server : public PJ::DataStreamer
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID "facontidavide.PlotJuggler3.DataStreamer")
  Q_INTERFACES(PJ::DataStreamer)

public:
  UDP_Server();

  virtual bool start(QStringList*) override;

  virtual void shutdown() override;

  virtual bool isRunning() const override
  {
    return _running;
  }

  virtual ~UDP_Server() override;

  virtual const char* name() const override
  {
    return "UDP Server";
  }

  virtual bool isDebugPlugin() override
  {
    return false;
  }

public:
  QUdpSocket* _udp_socket;

  int normal_socket(char* address_str, int port);

private:
  bool _running,_threadStarted = false,_threadFinished = false;
  WorkerThread* workerThread;
  PJ::MessageParserPtr _parser;
};

class WorkerThread : public QThread
{
  Q_OBJECT
public:
  WorkerThread(QObject* parent = nullptr) : QThread(parent)
  {
  }

  UDP_Server* udpServer;
  int port;
  QString address_str;

protected:
  void run() override
  {
    QByteArray byteArray = address_str.toUtf8();  // Convert QString to QByteArray using UTF-8 encoding
    char* charPtr = byteArray.data();  // Get pointer to the underlying data
    udpServer->normal_socket(charPtr, port);
  }
};