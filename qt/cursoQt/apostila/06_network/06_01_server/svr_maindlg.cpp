#include "svr_maindlg.h"
#include <QtGui>
#include <iostream>

// ======== classe da janela principal (derivada de QWidget):

svr_mainDlg::svr_mainDlg(QWidget *parent, Qt::WFlags flags)
    : QWidget(parent, flags)
{
    setupUi(this);

	stopButton->setEnabled( false ); // desabilita o 'stop' pois ainda n�o inciiou...
	portEdit->setText("6666"); // valor default para o n�mero de porta TCP

	// Conecta o signal do TcpServer que envia novas mensagens de log
	// ao slot apropriado deste objeto.
	// Esse signal poder� ser emitido originalmente por um objeto "TcpDataExchangeSocket"
	// e, se isso ocorrer, o objeto TcpServer tamb�m emitir� o mesmo signal,
	// pois o signal de "TcpDataExchangeSocket" foi conectado ao signal do TcpServer.
	// Al�m disso, o pr�prio objeto TcpServer poder� tamb�m enviar suas
	// pr�prias mensagens de 'log' (emitindo o signal diretamente).
	connect( &tcpServ, SIGNAL(logMessage(const QString  &)) ,
						this, SLOT(newLogMessage(const QString &)));
}
svr_mainDlg::~svr_mainDlg()
{

}

// slot para o signal 'click' do bot�o 'startButton';
// INICIAR aqui a espera de conex�es:
void svr_mainDlg::on_startButton_clicked()
{
	// recupera a porta:
	quint16 nPort = portEdit->text ().toUShort();

	// Inicia a espera de conex�es a partir de qualquer IP deste host (ou seja, qualquer de suas placas de rede)
	// o que e indicado por 'QHostAddress::Any'.
	// A espera de conex�es ser� feita na porta especificada por 'nPort':
	if ( tcpServ.listen ( QHostAddress::Any,  nPort ) )
	{
		// OK. Espera por conex�es iniciada com sucesso.
		// Ent�o, desabilitar o bot�o 'startButton' e habilitar o bot�o 'stopButton':
		startButton->setEnabled( false );
		stopButton->setEnabled ( true );
	}
	else
		QMessageBox::information(this, "Tcp Server", 
										"Falha ao iniciar o server" );
						
}

// slot para o signal 'click' do bot�o 'stopButton';
// ENCERRAR aqui a espera de conex�es:
void svr_mainDlg::on_stopButton_clicked()
{
	  // como vai encerrar, habilita o bot�o 'startButton' e desabilita o bot�o  'stopButton':
		startButton->setEnabled( true );
		stopButton->setEnabled ( false );

		// para de receber novas conex�es:
		tcpServ.close();

		// Interrompe comunica��o com os clientes que j� estejam conectados
		// sabendo-se que cada cliente � atendido por um objeto "QTcpSocket" espec�fico
		// que, neste exemplo, � implementado com a derivada "TcpDataExchangeSocket":
		foreach ( QObject * obj , tcpServ.children())
		{
				TcpDataExchangeSocket * sock = qobject_cast<TcpDataExchangeSocket *>(obj);
				if ( sock )
					sock->close(); // interrompe comunica��o com este cliente.
		}
}

// recebeu uma nova mensagem de log:
void svr_mainDlg::newLogMessage(const QString & msg)
{
	// adiciona a mensagem no QWidgetList:
	messagesList->addItem( new QListWidgetItem(msg));
	// seleciona a linha adicionada:
	messagesList->setCurrentRow( messagesList->count()-1);
}
