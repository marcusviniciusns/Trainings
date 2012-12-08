#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "aboutdlg.h"
#include <QTime> // trata hor�rios
	// n�o confundir com QTimer que � um "despertador"

#include <QMessageBox>

MainWindow::MainWindow(/* QMainWindow * this, */ QWidget *parent)
	: QMainWindow(parent)
	,  ui(new Ui::MainWindow)
	, m_progressBar(0)
{
	 ui->setupUi(this); // this: endere�o do objeto "w" instanciado em "main()"

	 ui->menuExibir->addSeparator();
	ui->menuExibir->addAction(ui->mainToolBar->toggleViewAction());
	ui->menuExibir->addAction(ui->dockWidget->toggleViewAction());
	ui->dockWidget->toggleViewAction()->setIcon(
																			QIcon(":/imagens/favorites.png"));

	m_timer.start(1000); // timeout a cada 1 segundo (1000 milisegundos)
	connect ( &m_timer, SIGNAL(timeout()),
				 this, SLOT(showTime()));

	// ir� exibir a hora daqui a um segundo e depois repetidamente
	// mas quero exibir J�:
	showTime();
}

MainWindow::~MainWindow(/* QMainWindow * this, */ )
{
	// n�o e necess�rio pois o 'this' � assumido por default para os membros:
	// delete this->ui;
	delete ui;
}

void MainWindow::on_webButton_clicked()
{
	// faz uma c�pia do texto do webEdit, retirando brancos no in�cio e no fim.
	QString text(ui->webEdit->text().trimmed());
	int posProto = text.indexOf("://");
	if (posProto < 3 )
	{
		if ( posProto >= 0)
			text.remove("://");

		text = "http://" + text;
		ui->webEdit->setText(text);
	}

	QUrl url ( ui->webEdit->text() );
	ui->webView->setUrl (url);
}

void MainWindow::on_webView_loadStarted()
{
	if ( !m_progressBar)
	{
		m_progressBar = new QProgressBar;
		m_progressBar->setRange(0, 100);
		ui->statusBar->addWidget(m_progressBar,1);
	}

	ui->statusBar->showMessage("");
	m_progressBar->setValue(0);
	m_progressBar->show();
}

void MainWindow::on_webView_loadProgress(int progress)
{
	m_progressBar->setValue(progress);
}

void MainWindow::on_webView_loadFinished(bool ok)
{
	m_progressBar->hide();
	if ( ok )
		ui->statusBar->showMessage("P�gina carregada com sucesso");
	else
		ui->statusBar->showMessage("Falha na carga da p�gina");
}

void MainWindow::on_actionSair_triggered()
{
	close();
}

void MainWindow::on_actionToggleBrowser_toggled(bool show)
{
	ui->webFrame->setVisible(show);
}

void MainWindow::on_actionSobre_triggered()
{
	AboutDlg dlg(this);
	dlg.exec();
}

void MainWindow::showTime()
{
	QTime now ( QTime::currentTime());
	QString nowStr ( now.toString("hh:mm:ss"));
	ui->digitalClock->display(nowStr);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	int canClose = QMessageBox::question(this, // parent da MessageBox
													 "Fechar",  // t�tulo (barra de t�tulo)
													 "Deseja mesmo fechar?", // mensagem
						 QMessageBox::Yes | QMessageBox::No // bot�es padr�o
													 );

	if ( canClose != QMessageBox::Yes)
		event->ignore();  // n�o fecha a janela (ignora o evento close)
	// else
	//	event->accept();  // aceita o evento e fecha a janela
										// mas n�o precisa, pois "accept" j� � o estado
										// default do evento.
}




