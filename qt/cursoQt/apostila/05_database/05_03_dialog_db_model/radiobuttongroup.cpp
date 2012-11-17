#include "radiobuttongroup.h"
#include <QRadioButton>

RadioButtonGroup::RadioButtonGroup(QWidget * parent)
	: QGroupBox(parent)
{
	m_rbGroup = new QButtonGroup(this);
	m_rbGroup->setExclusive(true); // s� um pode estar marcado
}

// N�o � possivel inserir os 'RadioButtons" no "ButtonGroup" j� na construtora,
// pois quando ela � chamada o "GroupBox" ainda est� vazio (os "RadioButtons" devem ter sido acrescentados ap�s sua cria��o)
// Ent�o a fun��o abaixo dever� ser chamada na construtora do Widget que estiver usando o GroupBox
// para que o "QButtonGroup" adicione todos os "RadioButtons" j� inseridos no GroupBox
void RadioButtonGroup::groupRadioButtons()
{
	// percorre a lista de "childs" do GroupBox:
	foreach ( QObject * obj, children())
	{
		QRadioButton * rb = qobject_cast<QRadioButton*>(obj);
		// se n�o est� nulo (ent�o � um 'QRadioButton'!)
		if ( rb )
			m_rbGroup->addButton( rb );
	}
}

// desmarca todos:
void RadioButtonGroup::uncheckAll ()
{
	m_rbGroup->setExclusive(false);

	foreach ( QAbstractButton * btn , m_rbGroup->buttons() )
		btn->setChecked(false);

	m_rbGroup->setExclusive(true);
}

// marca o 'RadioButton' que contiver o texto especificado:
void RadioButtonGroup::checkButton(const QString & textToCheck)
{
	m_rbGroup->setExclusive(false);

	foreach ( QAbstractButton * btn , m_rbGroup->buttons() )
		btn->setChecked( btn->text()==textToCheck );

	m_rbGroup->setExclusive(true);
}


