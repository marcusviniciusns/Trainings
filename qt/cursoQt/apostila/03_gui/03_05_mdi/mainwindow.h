#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>

namespace Ui
{
	class MainWindow;
}
class QMdiSubWindow;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();

private:
	Ui::MainWindow *ui;

	// ponteiros membro para duas das subWindows;
	// esses ponteiros permitir�o saber se uma subWindow
	// j� est� instanciada ou n�o:
	QMdiSubWindow * m_subwinForm1;
	QMdiSubWindow * m_subwinForm2;

	// fun��o auxiliar que cria nova inst�ncia de uma "subWindow",
	// j� definindo qual ser� o "internal widget" dessa "subWindow":
	QMdiSubWindow * createSubWindow_( QWidget * internalWidget );

	// fun��o auxiliar que exibe cada uma das "subWindows":
	void showSubWindow_( QMdiSubWindow * subWin);

	// fun��o auxiliar que conecta um signal de "Form1" a um slot de "Form2"
	// (o signal enviar� o texto do "lineEdit" de "Form1"
	//  o qual sera adicionado na "comboBox" de "Form2":
	void connectForm1ToForm2_();

private slots:

	// slots para o signal "triggered" das "actions"
	// inseridas no menu (menuBar) e na toolbar;
	void on_actionAbout_triggered();
	void on_actionBrowser_triggered();
	void on_actionForm2_triggered();
	void on_actionForm1_triggered();
	void on_actionQuit_triggered();

	// slot para o signal "subWindowActivated" do objeto "mdiArea";
	// ser� chamada quando uma subWindow for ativada:
	void on_mdiArea_subWindowActivated(QMdiSubWindow* );

	// slots para o signal "destroyed" as subWindows "Form1" e "Form2".
	// Nesses slots simplesmente atribu�mos zero aos ponteiros
	// para as subWindows, e desse modo a aplica��o saber�
	// se uma subWindow est� criada ou n�o
	// (e, por isso mesmo, esses ponteiros devem tamb�m
	//  ser inicializados com zero na construtora):
	void subWinForm1Destroyed()
	{
		m_subwinForm1 = 0;
	}
	void subWinForm2Destroyed()
	{
		m_subwinForm2 = 0;
	}
	// Observar que, neste exemplo, os 2 slots acima s� s�o necess�rios
	// para aquelas subwindows que n�o devem ter mais que uma inst�ncia criada simultaneamente.
	// J� para a terceira subwindow (o webBrowser) n�o criamos esse tipo de slot
	// pois o webBrowser podera ter m�ltiplas inst�ncias simult�neas.

	// slot para o signal "activated" da comboBox
	// para sele��o do modo de visualiza��o das subWindows:
	void selectViewMode(int index);

protected:
	//  closeEvent - quando a mainWindow receber o evento "close":
	void closeEvent(QCloseEvent * event);
};

#endif // MAINWINDOW_H
