#include "TcpClient.h"
 #include <QHostAddress>
#include <QtEndian>

TcpClient::TcpClient(QObject *parent) :
    QTcpSocket(parent)
 , m_nextBlockSize(0)
{
	connect( this, SIGNAL(connected()) , 
				this, SLOT(connectionOpened()));
	
	connect(this, SIGNAL(disconnected()),
			  this, SLOT(connectionClosed()));
	
	connect( this, SIGNAL(error(QAbstractSocket::SocketError)),
				this, SLOT(onError(QAbstractSocket::SocketError)));

	connect(this, SIGNAL(readyRead()) , 
			  this, SLOT(readNewData()));
}

bool TcpClient::start(const QString &host, quint16 portNumber)
{
	if ( host == "localhost" )
		connectToHost(QHostAddress::LocalHost, portNumber );
	else
		connectToHost(host, portNumber );
	
	// quase sempre desnecess�rio, pois posso saber
	// se conectou no slot "connecionOpened" ou, se deu erro no slot "onError":
	if ( !waitForConnected(5000) )
		qDebug() << "nao conectou em 5 segundos";
	else
		qDebug() << "conectou em at� 5 segundos";
	
	return true;
		
}
void TcpClient::stop()
{
	close();
}

/*
void TcpClient::sendData(AppProto::RequestType_t request, const QString &requestArgument)
{
	QDataStream out (this);
	out << (sizeof(AppProto::RequestType_t) + requestArgument.length() )
			 << request << requestArgument;
	
}

void TcpClient::readNewData()
{
	QDataStream in (this);

	if ( m_nextBlockSize == 0)
	{
		if ( bytesAvailable() < sizeof(AppProto::BlockSize_t))
			return;

		in >> m_nextBlockSize;
	}
	
	if ( bytesAvailable() < m_nextBlockSize ) // ainda n�o chegou tudo....
		return;   // j� poderia acumular os dados que chegaram
	
	//  |   block-size | string-argumento | 
	m_nextBlockSize = 0;
	QString serverReply;
	in >>  serverReply ;
	
	emit newData(serverReply);
}
*/

void TcpClient::sendData(AppProto::RequestType_t request, const QString &requestArgument)
{	
	// envia 
	//  |   block-size |  request | string-argumento | 
	
	// tamanho total do bloco de dados:
	AppProto::BlockSize_t blockSize =
	   AppProto::BlockSize_t( sizeof(AppProto::RequestType_t) + requestArgument.length());

	// converte o tamanho dos dados para a ordem de bytes da rede:
	unsigned char blockSizeBig[ sizeof(AppProto::BlockSize_t)+1 ];
	qToBigEndian<quint32>( blockSize, blockSizeBig);
	
	// grava o tamanho do bloco de dados 
	// OBS: (usa "writeData" pois "write" encerra se HOUVER UM TERMINADOR ZERO):
	writeData((char*)blockSizeBig, sizeof(AppProto::BlockSize_t));

	// grava os dados:

	// primeiro a "request" (num�rica):
	unsigned char requestBig[ sizeof(AppProto::RequestType_t) + 1];
	qToBigEndian(request, requestBig); // ordem de bytes da rede
	writeData((char*)requestBig, sizeof(AppProto::RequestType_t));
	
	// agora grava o argumento da "request" (string):
	write(requestArgument.toAscii(), requestArgument.length()); // "write" ok, pois agora est� gravando uma string s� texto
}

void TcpClient::readNewData()
{
	// recebe:
	//  |   block-size |  string-argumento | 
	
	if ( m_nextBlockSize == 0)
	{
		if ( bytesAvailable() < sizeof(AppProto::BlockSize_t))
			return;

		// OBS: (usa "readData" pois "read" encerra se HOUVER UM TERMINADOR ZERO):
 		 char bytes[sizeof(AppProto::BlockSize_t)+1];
		 readData(bytes, sizeof(AppProto::BlockSize_t));
		 bytes[sizeof(AppProto::BlockSize_t)] = 0;
		 
		// recupera o tamanho do bloco de dados, convertendo da ordem de bytes da rede para a ordem de bytes do host
		 m_nextBlockSize = AppProto::BlockSize_t (qFromBigEndian<quint32>((uchar*)bytes));	 
		m_bytesReceived.clear(); //@@ limpa a string que acumular� dados que chegarem
	}
	
	qint64 bytesToRead = bytesAvailable();
	if ( bytesToRead < m_nextBlockSize ) // ainda n�o chegou tudo....
	{
		m_bytesReceived += read(bytesToRead); // "read" ok, pois est� lendo uma string s� texto
		return;   // acumulou os dados que j� chegaram, mas ainda h� mais para chegar.
	}
	
	// agora vai ler o que estava faltando.
	// l� apenas a quantida que falta para este bloco:
		bytesToRead = m_nextBlockSize - m_bytesReceived.length();
	
	if ( bytesToRead > 0)
		m_bytesReceived += read(bytesToRead); 
	
	m_nextBlockSize = 0; // zera o indicador de tamanho
	emit newData(m_bytesReceived);
}

void TcpClient::connectionOpened()
{
	qDebug() << "conectou; se houver alguma requisi��o inicial para o server enviar aqui";
	emit newData("conex�o iniciada");
}

void TcpClient::onError(QAbstractSocket::SocketError socketError)
{
	qDebug() << "ERRO: #" << socketError << " / " << errorString();
	emit newData( QString("FIM: erro: %1").arg(errorString()));
	emit tcpError();
}


void TcpClient::connectionClosed()
{
	emit newData("FIM: conex�o encerrada");	
	emit tcpDisconnected();
}

