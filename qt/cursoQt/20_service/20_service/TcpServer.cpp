#include "TcpServer.h"
#include <QDateTime>
#include <QTextStream>

// ==================================================

// ==== TcpServer: classe que ir� aguardar conex�es:

TcpServer::TcpServer(QObject * parent)
	: QTcpServer(parent)
{

}

// nova conex�o foi estabelecida:
void	TcpServer::incomingConnection ( int socketDescriptor )
{
	// Quando um novo cliente � conectado, este server('QTcpServer') constr�i um objeto 'QTcpSocket'
	// e toda a comunica��o com esse cliente � feita atrav�s desse objeto 'QTcpSocket'.
	// 'QTcpSocket' trabalha assincronamente, ou seja, toda a comunica��o � feita
	// nos slots 'readClient' (novos dados recebidos do cliente)
	// e 'discardClient' (o cliente desconectou).
        DataExchangeSocket* dataExchangeSocket = new DataExchangeSocket(this);

	// conecta os signals de 'DataExchangeSocket' aos slots desta classe:

	// Signal da base(QTcpSocket), slot da derivada(DataExchangeSocket)
	// Novos dados chegaram, prontos para leitura
	// (neste exemplo, � uma nova requisi��o de um cliente conectado):
	connect(dataExchangeSocket, SIGNAL(readyRead()),
						dataExchangeSocket, SLOT(newRequest()));

	// Signal da base(QTcpSocket), slot da base(QTcpSocket)
	// Ao desconectar, agenda o 'delete' desse objeto para o 'event-loop':
	connect(dataExchangeSocket, SIGNAL(disconnected()),
						dataExchangeSocket, SLOT(deleteLater()));

	// Define para o objeto 'dataExchangeSocket' o 'socket' recebido como argumento nesta fun��o.
	// O 'socket'(um inteiro) identifica o endere�o do cliente e a porta para interc�mbio de dados:
	dataExchangeSocket->setSocketDescriptor(socketDescriptor);
}

// ==================================================

// ==== DataExchangeSocket: classe para troca de dados.
//			Um objeto desta classe ser� criado especificamente
//			para cada cliente que se conectou e far� a troca de dados
//			(ler pedidos / enviar respostas) no slot  'newRequest'
DataExchangeSocket::DataExchangeSocket(QObject *parent)
	: QTcpSocket(parent)
{
}

void DataExchangeSocket::newRequest()
{
    QDataStream in(this);
    QString request;
    in >> request;

    if ( request == "GET DATE TIME")
    {
            QDataStream out (this);
            out << QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss");
    }
    else
    {
            // pedido n�o suportado:
            close();
    }
}
