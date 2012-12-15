#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QTcpServer>
#include <QTcpSocket>

// ==================================================

// ==== TcpServer: classe que ir� aguardar conex�es:
class TcpServer : public QTcpServer
{
	Q_OBJECT

public:
	 TcpServer(QObject * parent=0);
protected:
	 virtual void	incomingConnection ( int socketDescriptor ); // nova conex�o...
};

// ==================================================

// ==== DataExchangeSocket: classe para troca de dados.
//			Um objeto desta classe ser� criado especificamente
//			para cada cliente que se conectou e far� a troca de dados
//			(ler pedidos / enviar respostas) no slot  'newRequest'
class DataExchangeSocket : public QTcpSocket
{
	Q_OBJECT

public:
	 DataExchangeSocket(QObject * parent=0);

 private slots:
	 void newRequest();  // dados chegaram...
 };

#endif // TCPSERVER_H

