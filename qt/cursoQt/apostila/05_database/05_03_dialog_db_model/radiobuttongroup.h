#ifndef RADIOBUTTONGROUP_H
#define RADIOBUTTONGROUP_H

#include <QAbstractButton>
#include <QRadioButton>
#include <QGroupBox>
#include <QButtonGroup>
#include  <QString>

class RadioButtonGroup : public QGroupBox
{
	Q_OBJECT

public:
	// Propriedade: deve definir:
	// o TIPO, uma fun��o de leitura, e opcionalmente uma fun��o de altera��o e uma de "reset" (estado vazio)
	// se "USER=true", significa que essa � a propriedade essencial
	//	/ que identifica o valor corrente do objeto: por exemplo, a propriedade "TEXT" de um lineEdit.
	Q_PROPERTY(QString checkedButtonText
						  READ checkedButtonText
						  WRITE checkButton
						  RESET uncheckAll
						  USER true)
// na nomenclatura Qt, a fun��o "WRITE" deveria chamar setCheckedButtonText

RadioButtonGroup(QWidget * parent=0);
	void groupRadioButtons();

	// retorna o ponteiro-membro para QButtonGroup (permite executar todas as suas fun��es)
	QButtonGroup * buttonGroup()
	{ return m_rbGroup;  }

	// === fun��es de conveniencia:

	void uncheckAll (); // desmarca todos;

	// retorna o texto do 'RadioButton' que estiver marcado
	// ou uma string vazia se nenhum estiver marcado:
	QString checkedButtonText() const {
		QAbstractButton * btn = m_rbGroup->checkedButton();
		return  btn ? btn->text() : "" ; // checa se o ponteiro � diferente de zero...
	}

	// marca o 'RadioButton' que contiver o texto especificado:
	void checkButton(const QString & textToCheck);


private :
		QButtonGroup * m_rbGroup;
};

#endif // RADIOBUTTONGROUP_H
