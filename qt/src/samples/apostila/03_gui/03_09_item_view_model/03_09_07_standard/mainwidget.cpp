#include <QSplitter>
#include <QStandardItemModel>
#include <QTreeView>
#include <QTableView>
#include <QListView>
#include <QComboBox>
#include <QHeaderView>
#include <QDir>
#include <QPushButton>
#include <QGridLayout>
#include <QMessageBox>
#include <QCloseEvent> 
#include "MainWidget.h"

#include <iostream>

MainWidget::MainWidget(const QByteArray & data, QWidget * parent )
		 : QWidget(parent)
		 , lsItems(QString(data).split('\n', QString::SkipEmptyParts ) )
		 , itemChanged(false)
		// a StringList 'lsITems" � obtida com o "split('\n)" 
		// - essa fun��o adiciona itens(strings) na StringList
		// a partir de cada linha do texto terminada com um '\n';
		// al�m disso decarta linhas vazias (SkipEmptyParts)
		 
 {
		
		// cria um objeto "splitter" para compartilhar widgets:    
	QSplitter *splitter = new QSplitter( this );

	// cria um "model" usando o "StandardModel"
	model = new QStandardItemModel;
	// selectionModel permitir� sincronizar o item selecionado
	// em uma "view" em todas as "views"
	selectionModel = new QItemSelectionModel(model);
	
	// define os t�tulos das colunas:
	model->setHorizontalHeaderItem(0, new QStandardItem("Alimentos"));	
	model->setHorizontalHeaderItem(1, new QStandardItem("Propriedades"));	
	
	// alimenta linhas, colunas e sub-n�veis:	
	QStandardItem *parentItem = model->invisibleRootItem();
	
	// A stringlist "lsTree" contem a estrutura da "tree".	
	// Vamos popular o modelo a partir dela:
	QStringList::const_iterator iterBegin = lsItems.begin();
	QStringList::const_iterator iterEnd = lsItems.end();
	populateModel ( parentItem, iterBegin, iterEnd, 0);	
	
	// Neste exemplo,
	// O "model" foi alimentado com linhas, colunas e sub-n�veis:
	// E ser�o criadas 4 "views" (uma "tree", uma "table", uma "list" e uma "comboBox")
	// relacionadas ao mesmo "model";
	// Cada "view" exibe os dados de uma determinada maneira;

	// 1- ==== a primeira "view" � uma "tree":
	tree = new QTreeView(splitter);
	tree->setModel(model);
	tree->setSelectionModel(selectionModel);
	tree->setSelectionBehavior ( QAbstractItemView::SelectRows);
	tree->setSelectionMode( QAbstractItemView::SingleSelection );
	tree->setAllColumnsShowFocus(true);

	// permite edi��o:

	QAbstractItemView::EditTriggers editTrig =   QAbstractItemView::DoubleClicked   
											   | QAbstractItemView::SelectedClicked 
											   | QAbstractItemView::EditKeyPressed ;

	tree->setEditTriggers( editTrig );
	// habilita classifica��o na tree:
	tree->setSortingEnabled(true);
	// classifica
	tree->sortByColumn(1);
	
	// configura o header para permitir mudan�a na ordem de classificac�o:
	QHeaderView * hdrTree = tree->header();
	hdrTree->setClickable (true);
	hdrTree->setSortIndicator(0,Qt::AscendingOrder);
	hdrTree->setSortIndicatorShown(true);
	hdrTree->setMovable(true); // permite mover colunas do header

	// 2- ==== a segunda "view" � uma "table"
	table = new QTableView(splitter);
	table->setModel(model);
	table->setSelectionModel(selectionModel);
	table->setSelectionBehavior ( QAbstractItemView::SelectRows);
	table->setSelectionMode( QAbstractItemView::SingleSelection );
	//table->setAlternatingRowColors(true);
	// permite edi��o:
	table->setEditTriggers( editTrig );
	// habilita classifica��o na table:
	table->setSortingEnabled(true);
	// classifica
	table->sortByColumn(1);
	
	// configura o header para permitir mudan�a na ordem de classificac�o:
	QHeaderView * hdrTable = table->horizontalHeader();
	hdrTable->setClickable (true);
	hdrTable->setSortIndicator(0,Qt::AscendingOrder);
	hdrTable->setSortIndicatorShown(true);
	hdrTable->setMovable(true); // permite mover colunas do header
			
	// 3- ==== a terceira view � uma "list": 
	list = new QListView(splitter);
	list->setModel(model);
	list->setSelectionModel(selectionModel);
	list->setSelectionBehavior ( QAbstractItemView::SelectRows);
   // list->setAlternatingRowColors(true);

	// 4- ==== a quarta "view" � uma "comboBox"
	combo = new QComboBox;
	combo->setModel(model);
	combo->view()->setSelectionModel(selectionModel);

	// configura a "splitter" definindo a largura de cada "view"
	int width = 800;	
	QList< int > cols;
	cols << int(width* 0.45) << int(width*0.45) << int(width*0.1);
	splitter->setSizes(cols);	

	// bot�o para salvar a tree:
	QPushButton * btSave = new QPushButton( "Salvar");
	
	// layout para agrupar a "combo" e a "splitter":
	QGridLayout * glayMain = new QGridLayout;
	setLayout( glayMain);
	glayMain->addWidget( combo, 0, 0, Qt::AlignHCenter); // linha 0, coluna 0;
	glayMain->addWidget( btSave,0, 1, Qt::AlignHCenter);
	glayMain->setRowMinimumHeight(1, glayMain->verticalSpacing() * 4); // linha 1: linha de separa��o
	glayMain->addWidget( splitter, 2, 0, 1, 2 ); // linha 2, coluna 0

	setWindowTitle("07_standard - 4 'views' (StandardModel) - recursivo - salvar - views sincronizadas");
	resize(800,500);	

	// expande a �rvore para visualiza��o inicial de todos os n�veis:
	tree->expandAll();			
	// largura da primeira coluna da "tree" deve ser suficiente 
	// para exibir inicialmente todos os itens:	
	tree->resizeColumnToContents(0); // coluna zero
	
	combo->adjustSize();
	combo->setMinimumWidth( int(combo->width()*2));	
	
	connect(tree->header(), SIGNAL ( sortIndicatorChanged ( int, Qt::SortOrder) ),
					   this, SLOT( on_treehdr_sortIndicator_changed( int, Qt::SortOrder ) ) );
	connect(table->horizontalHeader(), SIGNAL ( sortIndicatorChanged ( int, Qt::SortOrder) ),
					   this, SLOT( on_tablehdr_sortIndicator_changed ( int, Qt::SortOrder ) ) );

	connect ( selectionModel, SIGNAL (currentChanged (const QModelIndex&, const QModelIndex&) ),
				this , SLOT( on_selection_changed(const QModelIndex&, const QModelIndex &) ) );
	
	void on_itemChanged ( QStandardItem * item ) ;		
	
	connect( model, SIGNAL ( itemChanged (QStandardItem *) ),
				this, SLOT (  on_itemChanged ( QStandardItem * ) ) );

	connect( btSave, SIGNAL ( clicked() ),
				this, SLOT (  on_btSave_clicked() ) );
	
	tree->setCurrentIndex(model->index(0 , 0));	
}

void MainWidget::on_selection_changed(const QModelIndex & current, const QModelIndex & previous)
{			
	list->setModelColumn (current.column());	
	combo->setModelColumn(current.column());

	// Quando um sub-n�vel � selecionado em uma "tree",
	// nada � selecionado nas demais views.
	// Se quisermos que, nas demais "views", fique selecionado
	// o item de primeiro n�vel que � a raiz desse sub-n�vel,
	// ent�o, para essas "views", � preciso buscar
	// os itens de primeiro n�vel:
	
	QModelIndex firstlevelIndex(current);	
	// Quando o "parent" do "index" for um "QModelIndex" vazio
	// � porque esse "index" corresponde ao primeiro n�vel:
	while ( firstlevelIndex.parent() != QModelIndex() ) 
		firstlevelIndex = firstlevelIndex.parent();
	
	combo->setCurrentIndex(firstlevelIndex.row());
	combo->update();
}

void MainWidget::on_treehdr_sortIndicator_changed ( int col, Qt::SortOrder order )
{
	int oldCol   = table->horizontalHeader()->sortIndicatorSection(); 
	int oldOrder = table->horizontalHeader()->sortIndicatorOrder() ;
	if ( col != oldCol || order != oldOrder )
	{
		table->horizontalHeader()->setSortIndicator(col, order);
		on_colOrder_changed(col);
	}
}
void MainWidget::on_tablehdr_sortIndicator_changed ( int col, Qt::SortOrder order )
{
	int oldCol = tree->header()->sortIndicatorSection();
	int oldOrder = tree->header()->sortIndicatorOrder(); 
	if (col != oldCol || order != oldOrder )
	{
		tree->header()->setSortIndicator(col, order); 	
		on_colOrder_changed(col);	
	}
}
void MainWidget::on_colOrder_changed(int col)
{
	list->setModelColumn(col);
	combo->setModelColumn(col);			
	
	int row = selectionModel->currentIndex().row();	
	QModelIndex firstlevelIndex ( model->invisibleRootItem()->child(row,col)->index());	
	selectionModel->setCurrentIndex( firstlevelIndex, QItemSelectionModel::ClearAndSelect);
	combo->setCurrentIndex( selectionModel->currentIndex().row());	
	combo->update();
}


void MainWidget::populateModel (QStandardItem * item, QStringList::const_iterator & iter,  
				QStringList::const_iterator & iterEnd, int prevLevel)
{
	if ( iter == iterEnd )
		return;
	
	const QString & strItem = *iter;	// pega um �tem para inserir na �rvore		

	// '\t' no in�cio da string indica um aprofundamento de n�vel:
	int currentLevel = 0;
	while ( strItem[currentLevel] == '\t' )
		++currentLevel; // n�vel inferior		

	while ( currentLevel < prevLevel ) // && 
	{
		item = item->parent();
		--prevLevel;
	}
	if ( !item )
		item = model->invisibleRootItem();
		
	std::cout << currentLevel << ":" << strItem.toStdString() << std:: endl;	
	if ( currentLevel > prevLevel) // deve popular o n�vel inferior a este		
		item = item->child( item->rowCount()-1 , 0 );
				
	QList<QStandardItem*> itemCols;		
	// Procura colunas, preenchendo a "list" "itemCols"
	// com sub-strings separadas por '\t' (se existirem).
	// Faz a procura em "strItem" ap�s os '\t' iniciais, indicadores de n�vel, 
	// situados no in�cio da string (level):
	QString strCols = strItem.mid(currentLevel);
	QString strCol;			
	// cria uma lista de itens (colunas) buscando as "sections" separadas por '\t':		
	for ( int tabCol=0; !(strCol=strCols.section('\t', tabCol, tabCol)).isEmpty(); ++tabCol) 
		itemCols << new QStandardItem ( strCol );
	
	// insere coluna(s) no model:
	item->appendRow( itemCols );
	++iter; // evoluir o iterator

	populateModel(item, iter, iterEnd, currentLevel);		
}

void MainWidget::on_btSave_clicked()
{
	QString appPath = QDir::currentPath ();
	appPath.remove( "/debug" );
	appPath.remove( "/release");
	QFile file(appPath + "/tree.dat");
	file.open(QIODevice::WriteOnly);
	QTextStream stream(&file);
	
	saveModel( stream , 0);
	
	itemChanged = false;
}

void MainWidget::saveModel(QTextStream & stream, int level, QModelIndex parentIndex)
{	
	for ( int row = 0; model->index(row,0,parentIndex).isValid() ; ++row)
	{
		for ( int tabs=0; tabs<level; ++tabs)
			stream <<  '\t'  ;
	
		for ( int col=0 ; ; ++col)
		{
			QModelIndex index = model->index(row, col, parentIndex);	
			stream << index.data(Qt::DisplayRole).toString();
			
			if ( model->index(row, col+1, parentIndex).isValid() )
				stream << '\t';
			else
				break;
		}
	
		stream << '\n';
								
		// chamada recursiva para tratar n�veis filhos:
		saveModel(stream, level+1, model->index(row,0, parentIndex));
	}	
}
void MainWidget::on_itemChanged ( QStandardItem * item ) 
{
	itemChanged = true;
}
void MainWidget::closeEvent ( QCloseEvent * event ) 
{
	if (itemChanged )
	{
		if ( QMessageBox::question(this, "Fechar", "Os dados foram alterados. Deseja Salvar?",
				QMessageBox::Yes | QMessageBox::No)
			 == QMessageBox::Yes )
		{
			on_btSave_clicked();
		}
	}
	event->accept();
}


