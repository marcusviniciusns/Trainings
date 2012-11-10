#include "maindlg.h"
#include "ui_maindlg.h"
#include <QMessageBox>
#include <QCloseEvent>

MainDlg::MainDlg(QWidget *parent)
    : QDialog(parent), ui(new Ui::MainDlgClass)
	, m_pendingTasks( 0 )
{
    ui->setupUi(this);

	// conectar signals -> slots:

	// o signal "taskDone" do workerThread
	// poderia ser conectado diretamente ao slot "setText" do "textResult"
	// para imprimir os resultados da tarefa executada;
	// mas queremos tratar o texto e o aspecto do "textResult";
	// assim, conectamos esse signal ao slot "on_taskDone" desta classe:
	connect ( &workThread , SIGNAL ( taskDone( const QString & ) ) ,
					this, SLOT( on_taskDone( const QString &) ) );

	ui->lineEdit->setText( QString().setNum( m_pendingTasks ) );

	// agenda o thread:
	workThread.start();

}

MainDlg::~MainDlg()
{
    delete ui;
}

// este slot (conectado ao bot�o "btnTask")
// solicita ao thread a execu��o de sua tarefa:
void MainDlg::on_btnTask_clicked()
{
	// atualiza quantidade de tarefas pendentes:
	++m_pendingTasks;
	ui->lineEdit->setText( QString().setNum( m_pendingTasks ) );

	// "acorda" o thread para a execu��o de uma tarefa:
	workThread.doTask();
}

// slot que recebe resultados do trabalho do "workThead"
void MainDlg::on_taskDone( const QString & taskResult )
{
	// mantem os resultados anteriores dentro do textEdit
	// acrescentando o novo resultado:
	const QString & oldText = ui->textResult->toPlainText();

	if ( oldText.isEmpty() )
		ui->textResult->setText( taskResult );
	else
		ui->textResult->setText( oldText + "\n" + taskResult );

	// for�a um scroll para que esta nova linha esteja sempre vis�vel
	// mesmo que o textEdit tenha toda a �rea vis�vel j� preenchida:
	ui->textResult->moveCursor( QTextCursor::End );

	// atualiza quantidade de tarefas pendentes:
	--m_pendingTasks;
	ui->lineEdit->setText( QString().setNum( m_pendingTasks ) );
}

// trata o fechamento da janela:
void MainDlg::closeEvent ( QCloseEvent  * event )
{
	// parar o thread:
	workThread.stop();

	// aguarda encerramento:
	if ( workThread.wait(5000) )  // ou sai por timeout...
		QMessageBox::information ( this, "close",
					"O workerThread encerrou normalmente");
	else
		QMessageBox::critical ( this, "close",
					"O workerThread n�o encerrou: TIMEOUT");

	event->accept(); // aceita fechamento da janela.
}
