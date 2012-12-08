#include "Widget.h"
#include "ui_Widget.h"

#include <QMessageBox>

Widget::Widget(QSqlDatabase & db,  QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::Widget)
	, m_db(db)
{
    ui->setupUi(this);

	 m_model = new QSqlTableModel(this, m_db);
	 m_model->setTable("scooter");

	 // ------------------- fieldIndex (performance)
	 // se fosse usar em v�rias fun��es da classe
	 // estas vari�veis deveriam ser membras da classe
	 // para n�o ter que chamar "fieldIndex" outras vezes:
	 m_nameCol = m_model->fieldIndex("name");
	 m_idCol = m_model->fieldIndex("id");
	 m_maxSpeedCol = m_model->fieldIndex("maxspeed");
	 m_maxRangeCol = m_model->fieldIndex("maxrange");
	 // ... o mesmo para os demais campos que ser�o usados

	 m_model->setSort( m_nameCol, Qt::AscendingOrder);
	 m_model->select();

	 ui->tableView->setModel(m_model);

	 // esconde a coluna "id":
	 ui->tableView->hideColumn(m_idCol);

	 // comportamento de sele��o (Rows ou Items)
	 ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
	 // SingleSelection n�o permite a sele��o de m�ltiplas linhas de cada vez
	 ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);

	 // vai permitir o "sort"  na table (que acionar� o sort no model)
	 ui->tableView->setSortingEnabled(true);

	 // coloca o "sortIndicator" na coluna "name",
	 // j� que acima fiz um sort por essa coluna
	 ui->tableView->horizontalHeader()->setSortIndicator(
																										m_nameCol, Qt::AscendingOrder);

	 // linhas "zebradas":
	 ui->tableView->setAlternatingRowColors(true);

	 // �ltima coluna pode preencher todo o espa�o vazio � direita (se houver)
	 ui->tableView->horizontalHeader()->setStretchLastSection(true);

	 connect(m_model, SIGNAL(dataChanged(QModelIndex,QModelIndex)),
				this, SLOT(dataChanged(QModelIndex))) ;
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_sortEdit_returnPressed()
{
	QString sort(ui->sortEdit->text());
	int sortIndex = m_model->fieldIndex(sort);
	if ( sortIndex < 0 )
	{
		QMessageBox::warning(this, "Erro", "Campo para sort n�o existe");
		ui->sortEdit->setFocus();
		return;
	}
//	m_model->sort(sortIndex, Qt::AscendingOrder);
	//ou:
	ui->tableView->sortByColumn(sortIndex, Qt::AscendingOrder);
}

void Widget::on_filterEdit_returnPressed()
{
	QString filter (ui->filterEdit->text().simplified());
	m_model->setFilter(filter);
	if ( !m_model->select() )
	{
		QMessageBox::warning(this, "Erro", "Filtro incorreto");
		ui->filterEdit->setFocus();
	}
}

void Widget::dataChanged(const QModelIndex &index)
{
	int col = index.column();

	if ( col == m_nameCol)
	{
		QString name ( index.data().toString().simplified() );
		if ( name.length() < 5 || name.length() > 40 )
		{
			QMessageBox::information(this, "Erro",
											 "Nome: no m�nimo 5 e no m�ximo 40 caracteres");
			// desfaz altera��o:
			m_model->revert();
		}
	}
	else if ( col == m_maxSpeedCol)
	{
		int maxSpeed = index.data().toInt();
		if ( maxSpeed < 1 || maxSpeed > 90 )
		{
			QMessageBox::information(this, "Erro",
											 "Speed: no m�nimo 1 e no m�ximo 90 ");
			// desfaz altera��o:
			m_model->revert();
		}
	}
	else if ( col == m_maxRangeCol)
	{
		int maxRange = index.data().toInt();
		if ( maxRange < 10 || maxRange > 60 )
		{
			QMessageBox::information(this, "Erro",
																				"Range: no m�nimo 10 e no m�ximo 60 ");
			// desfaz altera��o:
			m_model->revert();
		}
	}

}
