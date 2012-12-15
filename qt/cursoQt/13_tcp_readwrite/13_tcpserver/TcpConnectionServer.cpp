#include "TcpConnectionServer.h"
#include "DataExchangeSocket.h"
#include <QDateTime>
TcpConnectionServer::TcpConnectionServer(QObject *parent) :
    QTcpServer(parent)
{
}

void TcpConnectionServer::incomingConnection(int socketDescriptor)
{
	// um novo cliente chegou; vou avisar quem me criou:
	emit logMessage( QString("%1 - Novo cliente conectado")
						  .arg(QDateTime::currentDateTime().toString("dd/MM/yyyy - hh:mm:ss")));
	
	// vou criar um TcpSocket para interc�mbio de dados com o novo cliente:
	DataExchangeSocket * sock = new DataExchangeSocket(this);
	sock->setSocketDescriptor(socketDescriptor);
	
	// o TcpSocket tamb�m pode emitir sinais de log; mas quem me criou
	// n�o conhece o TcpSocket. S� eu o conhe�o e s� aqui. ENT�O:	
	connect ( sock, SIGNAL(logMessage(QString)),		
				 this, SIGNAL(logMessage(QString)));
}

bool TcpConnectionServer::begin(quint16 portNumber)
{
	return listen(QHostAddress::Any, portNumber);
}

void TcpConnectionServer::stop()
{
	close();  // para de receber conex�es
	
	QList <DataExchangeSocket *> sockets = findChildren<DataExchangeSocket*>();
	foreach ( DataExchangeSocket * sock , sockets)
		sock->close(); // fecha os sockets de dados que ainda estejam ativos;
}
