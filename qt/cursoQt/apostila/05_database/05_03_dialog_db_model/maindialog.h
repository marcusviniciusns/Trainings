#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QDialog>
#include "dlg_database.h"

namespace Ui {
    class MainDialog;
}

class QMenu ;
class QString;
class QGroupBox;
class QSqlRelationalTableModel;
class QDataWidgetMapper;

class MainDialog : public QDialog {
    Q_OBJECT
public:
    MainDialog(QWidget *parent = 0);
    ~MainDialog();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::MainDialog *ui;

	QMenu * m_editBtnMenu;
	QMenu * m_editFinBtnMenu;
	QMenu * m_navBtnMenu ;
	dlg_database m_db;

	// model:
	QSqlRelationalTableModel * m_tableModel;
	QDataWidgetMapper * m_mapper;

	// indica se uma edi��o est� em curso e qual � ela:
	enum EditActions { EA_None, EA_Insert, EA_Update };
	EditActions m_editAction;

	// habilita/desabilita bot�es e widgets conforme modo de edi��o
	void changeEditAction(EditActions editAction);

	// "limpa" 'widgets' quando n�o h� dados a exibir:
	void clearWidgets();

	// "Actions" para navega��o:
	QAction * m_firstAct ;  // "primeiro registro"
	QAction * m_prevAct ; // "registro anterior"
	QAction * m_nextAct ; // "pr�ximo registro"
	QAction * m_lastAct ;  // "�ltimo registro"

	// "Actions" para entrada em edi��o (apenas as que s�o usadas em v�rias fun��es):
	QAction * m_editUpdAct; // "alterar registro"
	QAction * m_editDelAct;  // "deletar registro"

private slots:
	void onInsert();
	void onUpdate();
	void onDelete();

	void onOK();
	void onCancel();

	// muda estado das 'actions' (habilita / desabilita):
	void updateActionsState(int row);

protected:
	// redefine a virtual contextMenuEvent
	// � aqui que o menu ser� executado
	// quando o bot�o direito do mouse for acionado
	// (ou outra a��o padr�o prevista pela plataforma)
	void contextMenuEvent(QContextMenuEvent *event);

};

#endif // MAINDIALOG_H
