#include <QProcess> // permitir� criar processos (a partir de um bin�rio execut�vel)
#include <iostream>
#include <QDebug>

#include "TcpDataExchangeSocket.h"


// ============= CLASSE TcpDataExchangeSocket (derivada de QTcpSocket) =============
// === respons�vel pelo  intercambio de dados com cada cliente conectado
// ===  um novo objeto dessa classe ser� criado em 'TcpConnectionServer::incomingConnection'
// === para cada cliente que se conectar;


// construtora: faz as conex�es signal->slot:
TcpDataExchangeSocket::TcpDataExchangeSocket(QObject * parent)
			: QTcpSocket(parent)
			, m_nextBlockSize(0)
{
	// Signal da base, slot da derivada
	connect ( this, SIGNAL(readyRead() ) ,  // <- chegaram dados
					 this , SLOT( readClient() ) );  // <- vou ler esses dados

	// Signal da base, slot da base:
	connect ( this, SIGNAL ( disconnected() ) , // <- conex�o encerrada
						this, SLOT( deleteLater()) );   // 'deleteLater': objeto deve ser liberado quando processamento retornar ao 'event-loop'

	connect(this, SIGNAL(error(QAbstractSocket::SocketError)),
			  this, SLOT(onError(QAbstractSocket::SocketError)));
}

// slot 'readClient', conectado ao signal 'readyRead'
// o qual � emitido quando existem dados dispon�veis para leitura
// (um pedido enviado por um cliente, neste exemplo)
void TcpDataExchangeSocket::readClient()
{
    // pedidos ou comandos que poder� receber:
    // 1: Mensagem_a_Exibir     (TcpDataProtocol::showMessage)
    // 2: Programa_a_Executar (TcpDataProtocol::startProcess)

	/*
	 // Trabalhando em baixo n�vel com uma das fun��es 'read' ('read', 'readAll' ou readLine'):
	 // Nesse caso, por simplicidade, o comando poderia ser separado do seu argumento por uma 'newline':
	// 1\nMensagem_a_Exibir
	// 2\nPrograma_a_Executar

	QByteArray bytes;
	bytes = readLine();  // l� o comando (pois interrompe a leitura no '\n')
	request = QString ( bytes ).toUInt(); // converte para inteiro

	bytes = readLine(); // l� o argumento (ate um '\n' ou um terminador zero);
	QString requestArgument = bytes.data() ; // converte para QString
	*/

	// em n�vel mais alto, com 'QDataStream' (ou 'QTextStream': analise o caso de uso...):
	QDataStream in(this);
	in.setVersion(QDataStream::Qt_4_6);

	if (m_nextBlockSize == 0)
	{
		if (bytesAvailable() < sizeof(TcpDataProtocol::requestSizeType))
			return;  // n�o completou a leitura do in�cio do stream(tamanho total do stream); espera a pr�xima chamada.

		in >> m_nextBlockSize;
	}
	
	// | tamanho | comando | argumento-do-comando |
	
	if (bytesAvailable() <	m_nextBlockSize)
		 return;  // n�o completou leitura da requisi��o; 
							// espera a pr�xima chamada.
							// Mas poderia tamb�m ir acumulando o que j� chegou

	m_nextBlockSize = 0;  // j� recebeu uma requisi��o completa

 TcpDataProtocol::requestType request;  // identificador do pedido
 QString requestArgument ; // argumento para a execu��o do pedido

    in >> request >> requestArgument; // QDataStream suporta uma variedade de tipos... ver sua documenta��o

    QString reply;  // respostas para o cliente
    bool ok = true;  // ser� falso se um pedido inesperado ocorrer, e isso causar� a desconex�o do client que envio o pedido esp�rio.

    // Agora,
    // - examina qual foi o pedido do cliente (1 ou 2, neste exemplo)
    // - executa aquilo que foi pedido
    // - e define uma resposta para o cliente:

    switch ( request )
    {
        case TcpDataProtocol::showMessage: // ==== exibir mensagem

                // emite signal, enviando um texto de mensagem para quem estiver interessado
                // ('requestArgument' deve conter o texto da mensagem a exibir):
                emit logMessage( QString( "Server: recebi msg: %1").arg(requestArgument) );

                // resposta para o client:
                reply = requestArgument ;
                reply += " - mensagem recebida e exibida";
                break;

            case TcpDataProtocol::startProcess:  // ==== inciar processo a partir de um nome de bin�rio execut�vel

                // 'requestArgument' deve conter o nome de um bin�rio execut�vel:
			 if ( QProcess::startDetached( requestArgument,
								 QStringList() << "c:/cursoQt/AULA_01.txt")  )
                {
                        /*
                                        // Poder�amos tamb�m aguardar o in�cio do processo com um timeout
                                        {
                                                QProcess process;
                                                process.start( arg );
                                        //	if ( process.waitForStarted( 3000 ) ) // timeout em 3 segundos
                                        // ..... // ok
                                        }
                         */

                        // emite signal, enviando um texto de mensagem para quem estiver interessado
                        emit logMessage( QString( "Server: programa executado: %1").arg(requestArgument) );

                        // resposta para o client:
                        reply = requestArgument ;
                        reply += " - programa executado ";
                }
					 else
                    {
                        // emite signal, enviando um texto de mensagem para quem estiver interessado
                        emit logMessage( QString( "Server: ERRO ao executar: %1").arg(requestArgument) );

                        // resposta para o client:
                        reply = requestArgument;
                        reply += " - ERRO ao executar programa";
                }
                     break;

            default: // ==== pedido inv�lido

                    // emite signal, enviando um texto de mensagem para quem estiver interessado
                    emit logMessage("Server: recebi comando nao suportado");

                    // resposta para o client:
                    reply = " comando n�o suportado ";
                    ok = false; // provocar� desconex�o do cliente que enviou o pedido, logo ap�s reponder a esse client.
    }

    // = envia resposta ao cliente:

	 QByteArray block;
	 QDataStream out ( &block,  QIODevice::WriteOnly ); // (ou 'QTextStream': analise o caso de uso...)
    out.setVersion(QDataStream::Qt_4_6);

	 out << TcpDataProtocol::requestSizeType(0) << reply ;  // QDataStream suporta uma variedade de tipos... ver sua documenta��o
	 out.device()->seek(0);
	 out << TcpDataProtocol::requestSizeType(block.size()
														  - sizeof(TcpDataProtocol::requestSizeType));
	 write(block);

    if ( !ok ) // Se pedido inv�lido...
            close();  // desconecta este objeto "TcpDataExchangeSocket", com isso desconectando o client que enviou o pedido.
                                                    // E este objeto "TcpDataExchangeSocket" ser� deletado no proximo retorno ao 'event-loop'
	 // pois o seu signal "disconnected" est� conectado ao slot "deleteLater".
}

void TcpDataExchangeSocket::onError(QAbstractSocket::SocketError sockErr)
{
	QString msg;
	QTextStream stream(&msg);

	stream << "ERRO: " << sockErr << " - o mesmo que: " << error()
				<< " - texto do erro: " << errorString();

	emit logMessage(msg);
	qDebug() << msg;
}




