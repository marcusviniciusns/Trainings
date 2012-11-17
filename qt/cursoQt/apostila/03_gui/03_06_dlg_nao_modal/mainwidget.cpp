#include "mainwidget.h"
#include "ui_mainwidget.h"
#include <QPalette>

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::MainWidgetClass)
{
    ui->setupUi(this);

	// ==== mudar cores de um Widget qualquer:
	QPalette pal;

	// configura uma MessageBox n�o-modal para testar cores
	// (e aproveita para testar algumas funcionalidades de QMessageBox):

	// acrescenta o flag "StaysOnTop" aos flags da MessageBox,
	// para que fique sempre no topo das demais janelas (sempre visivel)
	msgBox.setWindowFlags (
                        msgBox.windowFlags()
                        | Qt::Tool
                        | Qt::WindowStaysOnTopHint);

	// n�o ser� modal:
	msgBox.setModal(false);
	// tera apenas o bot�o "Close":
	msgBox.setStandardButtons( QMessageBox::Close );// bot�o close

	msgBox.setIcon ( QMessageBox::Information ); // �cone
	msgBox.setWindowTitle( "lineEdit"  ); // t�tulo

	// texto adicional (informativo):
	msgBox.setInformativeText(
			"... continue digitando normalmente...");

	// acrescenta op��o para abrir uma caixa de detalhamento do texto da "MsgBox"
	msgBox.setDetailedText(
			"Esta MessageBox exibe o texto do 'lineEdit'");

	// muda cores da msgbox:

	// cor de fundo:
	pal.setColor( QPalette::Window,     QColor(Qt::yellow) );
	// cor do texto:
	pal.setColor( QPalette::WindowText, QColor(Qt::red) );
	// cor de fundo dos bot�es:
	pal.setColor( QPalette::Button,     QColor(0,0,223) );
	// cor do texto dos bot�es:
	pal.setColor( QPalette::ButtonText, QColor(0, 255, 255) );

	// deve preencher o fundo automaticamente
	// usando a cor correspondende da "Palette".
	// (do contrario, precisar�amos redefinir a virtual "PaintEvent"):
	msgBox.setAutoFillBackground(true);

	// modifica a "Palette" da MessageBox:
	msgBox.setPalette(pal);

	// posiciona o Widget principal nas coordenadas especificadas:
	move( 200, 300);
	msgBox.setMinimumWidth(400);

}
// === slot para o signal "textChanged" do"lineEdit":
void MainWidget::on_lineEdit_textChanged()
{
	// passa o texto do lineEdit para MessageBox:
	msgBox.setText( ui->lineEdit->text()  );
	// coloca a MessageBox no topo do desktop:
	msgBox.move( 0,0 );
	// exibe:
	msgBox.show(); // n�o-modal (ao contr�rio de "exec()" -> modal)
	activateWindow();
	QCoreApplication::processEvents();

	// for�a o foco no edit, para n�o interromper a digita��o:
	ui->lineEdit->setFocus();
}

MainWidget::~MainWidget()
{
    delete ui;
}
