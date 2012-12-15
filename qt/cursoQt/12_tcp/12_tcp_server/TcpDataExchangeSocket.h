#ifndef TCPDATAEXCHANGESOCKET_H
#define TCPDATAEXCHANGESOCKET_H

#include <QTcpSocket>

#include "TcpDataProtocol.h" // <- protocolo de aplica��o para troca de dados

// ============= CLASSE TcpDataExchangeSocket (derivada de QTcpSocket) =============
// === respons�vel pelo  intercambio de dados com cada cliente conectado
// ===  um novo objeto dessa classe ser� criado em 'TcpConnectionServer::incomingConnection'
// === para cada cliente que se conectar;

class TcpDataExchangeSocket : public  QTcpSocket
{
	Q_OBJECT

	public:
		TcpDataExchangeSocket(QObject * parent);

	private slots:

	// slot 'readClient': ser� conectado ao signal 'readyRead'
	// o qual � emitido quando existem dados dispon�veis para leitura
	// (um pedido enviado por um cliente, neste exemplo)
		void readClient();

		// capturar erros:
		void onError(QAbstractSocket::SocketError);

public:
signals:
		// um sinal para enviar mensagens de 'log'
		// que poder�o ser recebidas por quem estiver interessado:
		void logMessage(const QString& msg);

 private:
		TcpDataProtocol::requestSizeType m_nextBlockSize;
};

#endif // TCPDATAEXCHANGESOCKET_H
