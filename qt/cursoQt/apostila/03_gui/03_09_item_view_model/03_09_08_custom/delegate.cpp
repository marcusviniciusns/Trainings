#include <QtGui>

#include "model.h"
#include "delegate.h"

Delegate::Delegate(QObject * parent )
	: QItemDelegate(parent)  // construtora base
{
	// Lista de cidades que dever� alimentar a comboBox:
	m_listaCidades << "Andradina" << "Araraquara" << "Bauru"
						<< "Campinas" << "Catanduva"
						<< "Rio de Janeiro" << "Salvador" ;
}

// Definir op��es e exibir o delegate
void Delegate::paint(QPainter *painter,
						  const QStyleOptionViewItem &option,
						  const QModelIndex &index) const
{
	// personalizamos apenas a coluna cidade:
	if (index.column() == Model::ColunaCidade )
	{
		// Recupera o texto atualmente selecionado no model (apontado pelo �ndice):
		QString cidade = index.model()->data(index, Qt::DisplayRole).toString();

		// Acrescenta op��o de alinhamento:
		QStyleOptionViewItem myOption = option;
		myOption.displayAlignment = Qt::AlignLeft | Qt::AlignVCenter;

		// Exibe e d� foco:
		drawDisplay(painter, myOption, myOption.rect, cidade);
		drawFocus(painter, myOption, myOption.rect);
	}
	else // nada a acrescentar:
	{
			QItemDelegate::paint(painter, option, index);
	}
}

QWidget *Delegate::createEditor(QWidget *parent,
		const QStyleOptionViewItem &option,
		const QModelIndex &index) const
{
	// Apenas cidade:
	if (index.column() == Model::ColunaCidade )
	{
		// para a coluna cidade, cria o "editor" do delegate
		// que neste caso ser� uma comboBox:
		QComboBox *comboCid = new QComboBox(parent);

		// conecta o signal de altera��o da comboBox
		// com o slot respons�vel por finalizar a edi��o:
		connect(comboCid , SIGNAL(editTextChanged (const QString &)) ,
							this , SLOT(commitAndCloseEditor()) );

		return comboCid;  // retorna o "editor" deste delegate
	}
	else // nada a acrescentar:
	{
		return QItemDelegate::createEditor(parent, option, index);
	}
}

void Delegate::setEditorData(QWidget *editor,
								  const QModelIndex &index) const
{
	if (index.column() == Model::ColunaCidade )
	{
		// Recupera a comboBox:
		QComboBox *comboCid = qobject_cast<QComboBox *>(editor);
		// Alimenta a comboBox com a lista de cidades:
		comboCid->addItems( m_listaCidades );

		// Recupera a cidade atualmente selecionada no model
		// e ativa a linha correspondente na comboBox:
		QString cidade= index.model()->data(index, Qt::DisplayRole).toString();
		comboCid->setCurrentIndex( comboCid->findText(cidade) );
	}
	else  // nada a acrescentar:
	{
		QItemDelegate::setEditorData(editor, index);
	}
}

void Delegate::setModelData(QWidget *editor,
								 QAbstractItemModel *model,
								 const QModelIndex &index) const
{
	if (index.column() ==Model::ColunaCidade )
	{
		// Passar para a c�lula atualmente selecionada no model (referenciada pelo �ndice)
		// o texto atualmente selecionado na comboBox:
		QComboBox *comboCid = qobject_cast<QComboBox *>(editor);
		model->setData(index,  comboCid->currentText() );
	}
	else  // nada a acrescentar:
	{
		QItemDelegate::setModelData(editor, model, index);
	}
}

void Delegate::commitAndCloseEditor()
{
	// Recupera a comboBox:
	QComboBox *editor = qobject_cast<QComboBox *>(sender());
	// Emite sinais para concluir edi��o:
	emit commitData(editor);
	emit closeEditor(editor);
}
