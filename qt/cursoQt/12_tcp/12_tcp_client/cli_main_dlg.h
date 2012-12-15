#ifndef CLI_MAIN_DLG_H
#define CLI_MAIN_DLG_H

#include <QtGui/QDialog>
#include <QTcpSocket>

#include <TcpDataProtocol.h> // <- protocolo de aplica��o para troca de dados

#include "ui_cli_main_dlg.h"

class cli_main_dlg : public QDialog
								, private Ui::cli_main_dlgClass
{
    Q_OBJECT

public:
    cli_main_dlg(QWidget *parent = 0, Qt::WFlags flags = 0);
    ~cli_main_dlg();
	
private slots:

	 // == slots adicionados via "go to Slot" (respostas ao click nos bot�es):

	 void on_startButton_clicked() ;
	void on_stopButton_clicked() ;
	void on_sendRequestButton_clicked() ;

	// == slots adicionados manualmente no c�digo:
	
	// O signal 'connected' � emitido por QTcpSocket quando a conex�o � estabelecida
	// isso permitiria aguardar a conex�o assincronamente...
	 void sendRequest(); // <- e este slot ser� conectado a esse signal.

	 // O signal  'readyRead' � emitido quando h� dados dispon�veis para leitura...
	 void clientRead(); // <- e este slot ser� conectado a esse signal.

	 void connectionClosed(); // slot para o signal 'disconnected' (conex�o encerrou)
	 void errorOccurred(); // slot para o signal 'error'
	
private:
	 // objeto 'QTcpSocket': estabelecer� a conex�o TCP ao server e cuidar� do interc�mbio de dados:
	QTcpSocket m_tcpSocket;
	TcpDataProtocol::requestSizeType m_nextBlockSize;
};

#endif // CLI_MAIN_DLG_H
