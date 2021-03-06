#include "cli_main_dlg.h"
#include <QHostAddress>
#include <QMessageBox>
#include <QDebug>


cli_main_dlg::cli_main_dlg(QWidget *parent, Qt::WFlags flags)
    : QDialog(parent, flags)
	 ,  m_nextBlockSize(0)
{
    setupUi(this);

	 // signal 'connected' � emitido por QTcpSocket quando a conex�o � estabelecida
	 // isso permitiria aguardar a conex�o assincronamente:
	connect(&m_tcpSocket, SIGNAL(connected()),
			this, SLOT(sendRequest()));
    
	// para desconex�o (causada por este cliente ou pelo servidor):
	connect(&m_tcpSocket, SIGNAL(disconnected()),
				this, SLOT(connectionClosed()));

	// signal  'readyRead': emitido quando h� dados dispon�veis para leitura:
	connect(&m_tcpSocket, SIGNAL(readyRead()),
            this, SLOT(clientRead()));
	
	// signal  'error': algum erro ocorreu:
	 connect(&m_tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
				this, SLOT(errorOccurred()));

	 // alimenta a combobox com os pedidos que podem ser enviados ao server:

	 requestCombo->clear();

	  requestCombo->addItem( 
				  QString::number(TcpDataProtocol::showMessage)
                                + " - Exibir Mensagem"  , // texto
                                 TcpDataProtocol::showMessage // valor interno associado a este item da combo box
											 );																				// para recupera��o futura

	 requestCombo->addItem( QString::number(TcpDataProtocol::startProcess)
												+ " - ExecutarPrograma"  , // texto
                                                TcpDataProtocol::startProcess // valor interno associado a este item da combo box
                                );																				// para recupera��o futura

	 requestCombo->model()->sort(Qt::AscendingOrder); // classifica para que as op��es apare�am para o usu�rio seguindo a ordem
																				// de 'TcpDataProtocol', j� que os valores dos pedidos possiveis foram adicionados
																					// no in�cio de dada string adicionada � combo.

	 // valores default para host e porta:
	hostEdit->setText( "localhost");
	portEdit->setText("6666");

}
cli_main_dlg::~cli_main_dlg()
{
}

// iniciar uma conex�o (click no bot�o 'startButton'):
void cli_main_dlg::on_startButton_clicked()
{
	// recupera o texto do nome (ou IP no formato pontos) do host:
	QString host = hostEdit->text();
	// recupera o n�mero da porta TCP onde o server j� deve estar aguardando conex�es:
	quint16 nport = portEdit->text().toUShort();

	// se 'localhost', adapta o nome:
	if ( host == "localhost" )
		m_tcpSocket.connectToHost(QHostAddress::LocalHost, nport );
	else
        m_tcpSocket.connectToHost(host, nport );

	// desabilita o bot�o "begin":
	startButton->setEnabled(false);

	// Vou saber se a conex�o foi estabelecida assincronamente no slot "sendRequest"
	// (conectada ao signal  "connected").
	// Mas posso tambem esperar a conex�o de forma s�ncrona, com timeout:
	if ( m_tcpSocket.waitForConnected( 3000 ) ) // 3 segundos para timeout
	{
		// OK: conex�o estabelecida
		// desabilita o 'stopButton'
		stopButton->setEnabled(true);

		// e habilita a combo de pedidos, o edit de argumentos e o bot�o de envio:
		requestCombo->setEnabled(true);
		requestArgEdit->setEnabled(true);
		sendRequestButton->setEnabled(true);
	}
	else // falhou
	{
        QMessageBox::critical( this , "tcpServer", "Falha na conex�o"
                                               + m_tcpSocket.errorString());
		startButton->setEnabled(true);
	}
		
}

// encerrar a conex�o (clik no bot�o 'stopButton'):
void cli_main_dlg::on_stopButton_clicked()
{
	  // desabilita o bot�o 'stop', e os widgets de pedidos:
		stopButton->setEnabled(false);
		sendRequestButton->setEnabled(false);
		requestCombo->setEnabled(false);
		requestArgEdit->setEnabled(false);

		// e habilita o bot�o 'start':
		startButton->setEnabled(true);

		if ( m_tcpSocket.isOpen() )
			m_tcpSocket.close() ; // fecha conex�o
}

// slot conectado ao signal "connected" (conex�o estabelecida)
void cli_main_dlg::sendRequest()
{
	// conex�o conclu�da com sucesso;
	// alguma requisi��o inicial?  Enviar aqui.
	qDebug() << "conex�o estabelecida";
}

// enviar um pedido ao servidor (bot�o  'sendRequestButton'):
void cli_main_dlg::on_sendRequestButton_clicked()
{
	// O server em quest�o espera apenas dois tipos de pedido:
	// 1: exibir uma mensagem;
	// 2: executar um programa.

	// Para enviar o pedido, recuperar a linha selecionada na comboBox:
	int row= requestCombo->currentIndex();
	// e em seguida recupera o n�mero de pedido associado a essa linha da combo:
	TcpDataProtocol::requestType request =
					 requestCombo->itemData(row).toUInt();
	// n�mero "escondido" no item da combo(ver "addItem" na construtora)

	// argumento para esse pedido:
	// um texto de mensagem(se pedido==1)
	// ou um nome de execut�vel(se pedido==2)
	QString requestArgument = requestArgEdit->text();

	// Podemos formatar o pedido completo em "baixo n�vel"
	// usando a fun��o 'write':
	// QString requestText ( QString("%1\n%2").arg(request).arg(requestArgument) );
	// m_tcpSocket.write( requestText.toAscii() );

	// ou em n�vel mais alto, com 'QDataStream' (ou 'QTextStream': analise o caso de uso...):
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_6);


	out << TcpDataProtocol::requestSizeType(0) << request << requestArgument;  // QDataStream suporta uma variedade de tipos... ver sua documenta��o
	out.device()->seek(0);
	out << TcpDataProtocol::requestSizeType(block.size() - sizeof(TcpDataProtocol::requestSizeType));
	m_tcpSocket.write(block);
}

// "clienteRead": slot conectado ao signal 'readyRead':
// h� dados dispon�veis para leitura (resposta do server a um pedido):
void cli_main_dlg::clientRead()
{	
	// Podemos recuperar os dados em n�vel mais baixo com 'read , 'readAll' ou 'readLine':
	//	QByteArray servReply = m_tcpSocket.readAll();
	//	QMessageBox::information( this, "client",
	//		QString ( "Resposta do server:\n" ) + servReply.data()  );

		// ou em n�vel mais alto com 'QDataStream': (ou 'QTextStream': analise o caso de uso...):
		QDataStream in (&m_tcpSocket);
		in.setVersion(QDataStream::Qt_4_6);

	if (m_nextBlockSize == 0)
	{
		if (m_tcpSocket.bytesAvailable() < sizeof(TcpDataProtocol::requestSizeType))
			return;  // n�o completou a leitura do in�cio do stream(tamanho total do stream); espera a pr�xima chamada.

		in >> m_nextBlockSize;
	}

	if (m_tcpSocket.bytesAvailable() <	m_nextBlockSize)
		 return;  // n�o completou leitura da resposta; espera a pr�xima chamada.

  m_nextBlockSize = 0;  // j� recebeu uma resposta completa
	QString servReply;
	in >> servReply;

	QMessageBox::information( this, "client",
		QString ( "Resposta do server:\n" ) + servReply  );
}

// signal 'disconnected' foi emitido:
void cli_main_dlg::connectionClosed()
{
	// a conex�o pode ser encerrada pelo servidor
	// ou pelo usu�rio desta aplica��o(bot�o "stop")

	if ( isVisible() && stopButton->isEnabled() ) // o servidor encerrou a conex�o
	{
		QMessageBox::warning( this, "client", "conex�o encerrada pelo servidor");
		// habilitar/desabilitar bot�es:
		on_stopButton_clicked() ;
	}
	else // usu�rio encerrou conex�o
		QMessageBox::warning( this, "client", "conex�o encerrada pelo usu�rio");
}

// signal 'error' foi emitido:
void cli_main_dlg::errorOccurred()
{
	// recupera informa��es sobre o erro:
	 qDebug() << m_tcpSocket.error() << " : " << m_tcpSocket.errorString();
	 if ( m_tcpSocket.isOpen() )
		 m_tcpSocket.close(); // encerra
}












