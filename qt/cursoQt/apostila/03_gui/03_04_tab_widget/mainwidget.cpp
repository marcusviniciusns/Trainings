#include "mainwidget.h"
#include "ui_mainwidget.h"
#include <QStyleFactory>
#include <QMessageBox>

MainWidget::MainWidget(QWidget *parent)
	: QWidget(parent), ui(new Ui::MainWidget)
	, nameOK(false)
	, mailOK(false)
{
	ui->setupUi(this);

	// o "tabWidget" deve come�ar sempre com a primeira aba selecionada
	// (pois, no QtDesigner, outra aba pode ter sido a �ltima selecionada):
	ui->tabWidget->setCurrentIndex(0);

	// valores iniciais para a aba "par�metros":
	ui->spNameMinLen->setValue( 3 ); // m�nimo de 3 caracteres para nome
	ui->ckbMailArroba->setChecked(true); // e-mail deve conter arroba.

	// aba "estilos":
	// popula a comboBox da aba "Estilos" com os estilos dispon�veis:
	ui->cbStyles->addItems(QStyleFactory::keys());
	// for�a o uso do estilo que esteja na primeira linha da comboBox:
	on_cbStyles_activated( ui->cbStyles->itemText(0) );
}

MainWidget::~MainWidget()
{
	delete ui;
}

// ==== ==== slots:

// ==== aba cadastro:

// nome em ma�usculas:
void MainWidget::on_ckbUpperName_stateChanged()
{
	if (  ui->ckbUpperName->isChecked() )  // em ma�usculas
		ui->edName->setText( ui->edName->text().toUpper()  );
}

// verifica/valida o nome:
void MainWidget::on_edName_textChanged()
{
	// valida o nome, habilitando/desabilitando o bot�o "Aceitar":
	if ( ui->edName->text().length() < ui->spNameMinLen->value()   ) // compara com o valor do spinBox de par�metros
		nameOK = false; // erro: tamanho menor que minimo
	else
		nameOK = true; // tamanho ok;

	on_ckbUpperName_stateChanged(); // checa se o nome precisa ficar em ma�usculas

	updateStatus();
}
// verifica/valida o e-mail:
void  MainWidget::on_edMail_textChanged()
{
	mailOK = true;
	// valida o e-mail, habilitando/desabilitando o bot�o "Aceitar":
	if ( ui->ckbMailArroba->isChecked() ) // verifica se a checkBox de par�metros determina que o e-mail tem que conter '@'
	{
		int posArroba = ui->edMail->text().indexOf( '@' );
		// poderia continuar a busca para ver se existe mais que uma '@'...
		if ( posArroba < 1 || ui->edMail->text().length() < 3 ) // <- '1' -> deve haver ao menos um caracter antes da '@' e outro depois
			mailOK = false; // n�o contem '@', ou ent�o ela � o primeiro caracter, ou ent�o n�o h� um caracter depois dela
	}
	updateStatus();
}
void MainWidget::on_okButton_clicked()
{
	QString result;
	result  = "Nome: " + ui->edName->text() + "\n";
	result += "Mail:  " + ui->edMail->text();
	QMessageBox::information( this, "Dados Aceitos",
							  result );
	close();
}

// ==== aba parametros:

// o par�metro "tamanho m�nimo do nome" foi alterado;
// altera o spinBox de "par�metros" e for�a uma revalida��o do nome em "cadastro";
// OBS: se o spinBox foi alterado esta fun��o tamb�m ser� chamada
// pois o signal "valueChanged" do spinBox foi conectado ao slot "setText" deste lineEdit no QtDesigner
void MainWidget::on_edNameMinLen_textChanged()
{
	// altera o valor do spinBox
	ui->spNameMinLen->setValue( ui->edNameMinLen->text().toInt() );
	// for�a revalidac�o do nome na aba "cadastro":
	on_edName_textChanged();
}
// o par�metro "e-mail deve conter uma arroba" foi alterado:
// ent�o for�a uma revalidac�o do e-mail em "cadastro":
void MainWidget::on_ckbMailArroba_stateChanged ()
{
	// for�a revalida��o do e-mail na aba "cadastro":
	on_edMail_textChanged();
}

// ==== aba estilos:

// solicitada mudan�a de estilo:
void MainWidget::on_cbStyles_activated(
								const QString & styleName)
{
	QStyle * style = QStyleFactory::create(styleName);
}

// ==== ==== fun��es auxiliares:

// - habilita/desabilita o bot�o "Aceitar"
// - escreve mensagem no label "Status"
void MainWidget::updateStatus()
{
	if ( nameOK && mailOK )
	{
		ui->okButton->setEnabled(true); // habilita bot�o "Aceitar"
		ui->statusLabel->setText("Dados aceit�veis.");
	}
	else
	{
		ui->okButton->setEnabled(false); // desabilita bot�o "Aceitar"

		QString status ;

		if ( !nameOK   )
			status = QString("Nome: m�nimo de %1 caracteres").arg(  ui->spNameMinLen->value() );

		if ( !mailOK )
		{
			if ( !status.isEmpty() )
				status += '\n';

			status += "e-mail: deve conter arroba, com (no m�nimo) um caractere antes e outro depois";
		}

		ui->statusLabel->setText( status );
	}
}



