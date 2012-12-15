#ifndef TCPCONNECTIONSERVER_H
#define TCPCONNECTIONSERVER_H

#include <QTcpServer>
#include <TcpDataExchangeSocket.h> // <- classe para intercambio de dados com cada cliente conectado


// ===========  CLASSE TcpConnectionServer (derivada de QTcpServer) ===============
// ======  sua �nica fun��o � aguardar conex�es (ou encerr�-las):

class TcpConnectionServer : public QTcpServer
{
	Q_OBJECT

	public:
		TcpConnectionServer() {}

		// virtuais da base:
	protected:

		// incomingConnection: chamada pela base quando uma nova conex�o
		// com um cliente foi estabelecida:
			void incomingConnection(int socketDescriptor)
			{
                                // um novo cliente se conectou...
				 emit logMessage("Connection Server: novo cliente");

				// Se uma nova conex�o foi estabelecida,
				// cria um novo objeto "TcpDataExchangeSocket" (derivada de QTcpSocket)
				// para troca de dados com o cliente que se conectou,
				 // atribuindo-lhe o "socketDescriptor" que identifica endere�o e porta de dados para cada cliente.
				// Assim, haver� um "TcpDataExchangeSocket" para cada cliente que se conectar,
				// cada qual com o seu pr�prio socket.
				TcpDataExchangeSocket *socket = new TcpDataExchangeSocket(this);
				socket->setSocketDescriptor(socketDescriptor);

				// Conecta o signal "logMessage" do objeto apontado por socket (TcpDataExchangeSocket)
				// ao signal "logMessage" do objeto "TcpConnectionServer".
				// ISTO, PORQUE:
				// O objeto apontado por "socket" s� e conhecido aqui.
				// Quando tem novas mensagens de 'log' ele emite um signal.
				// Mas quem vai aproveitar esse signal, se esse objeto s� � conhecido aqui?
				// Ent�o ele faz com que o objeto "TcpConnectionServer"
				// que � �nico e � conhecido por quem o criou, tamb�m emita o mesmo signal.
				// Desse modo o criador do objeto "TcpConnectionServer"
				// poder� conectar o signal deste a qualquer slot.
				connect( socket, SIGNAL(logMessage(const QString &)),
									this, SIGNAL(logMessage(const QString&)));

				// Feito isso, o objeto 'TcpConnectionServer' continuar� aguardando novas conex�es.

				// uma outra alternativa, seria dispara um thread aqui
				// e passar para ele o "socketDescriptor".
				// DENTRO da fun��o "run" do thread (isto � realmente DENTRO
				// desse thread), seria instanciado o TcpSocket, que seria tratado
				// nesse thread.

				// Mas S� use isso em casos muito especiais, em que seja obrigatorio
				// completar uma comunica��o.

			}

		public:
		signals:
				// um sinal para enviar mensagens de 'log'
				// que poder�o ser recebidas por quem estiver interessado:
				void logMessage(const QString& msg);
};

#endif // TCPCONNECTIONSERVER_H
