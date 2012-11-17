#ifndef SVR_MAINDLG_H
#define SVR_MAINDLG_H

#include <QtGui/QWidget>
#include <QProcess>

#include "ui_svr_maindlg.h"

#include "TcpConnectionServer.h" // <- classe derivada de QTcpServer: aguardar e aceitar conex�es; proceder desconex�es.

class svr_mainDlg : public QWidget
								  , private Ui::svr_mainDlgClass

{
    Q_OBJECT

public:
    svr_mainDlg(QWidget *parent = 0, Qt::WFlags flags = 0);
    ~svr_mainDlg();
	
private slots:

 // == acrescentados via "go to Slot":
	void on_startButton_clicked() ;
	void on_stopButton_clicked() ;

	//== acrescentados manualmente:
	void newLogMessage(const QString & msg);
	// esse slot servir� para receber mensagens de  'log' que ser�o enviadas
	// pelo TcpServer que emitir� um signal sempre que tiver algo a dizer.

private:

	// objeto da classe TcpConnectionServer (derivada de QTcpServer)
	// sua �nica fun��o � aguardar conex�es (ou encerr�-las):
	TcpConnectionServer tcpServ;

};

#endif // SVR_MAINDLG_H
