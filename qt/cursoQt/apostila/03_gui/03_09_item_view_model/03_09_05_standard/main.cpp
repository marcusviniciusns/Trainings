#include <QtGui>

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	QWidget * main_wg = new QWidget;

	// cria um objeto "splitter" para compartilhar widgets:    
	QSplitter *splitter = new QSplitter(main_wg);

	const int totRows=10;
	const int totCols = 3;
	// cria um "model" usando o "StandardModel"
	QStandardItemModel *model = new QStandardItemModel(main_wg);
	int n0_row, col;
	
	// define os t�tulos das colunas:
	for (col = 0; col < totCols; ++col) 
	{
		model->setHorizontalHeaderItem(col, 
			new QStandardItem( QString("COL-%1").arg(col+1) ) );
	}
	
	// alimenta linhas, colunas e sub-n�veis:	
	QStandardItem *parentItem = model->invisibleRootItem();
	
	for (n0_row = 0; n0_row < totRows; ++n0_row) 
	{
		// alimenta o primeiro n�vel:
		QList<QStandardItem*> itCols;
		for ( col = 0; col < totCols; ++ col )
			itCols <<
				new QStandardItem ( QString("item %1%2").arg(n0_row).arg(col));

		parentItem->appendRow(itCols );
		
		// alimenta o sub-n�vel:
		QStandardItem *subItem = parentItem->child(n0_row,0);				
		for ( int n1_row=0; n1_row<3; ++n1_row)
		{
			QList<QStandardItem*> subItCols;
			for (col = 0; col < totCols; ++ col )
				subItCols << new QStandardItem ( 
						QString("item %1%2%3").arg(n0_row).arg(n1_row).arg(col) );
		
			subItem->appendRow(subItCols);	
			
			// alimenta o sub-sub-n�vel:
			QStandardItem *subSubItem = subItem->child(n1_row,0);
			for ( int n2_row =0 ; n2_row<2; ++n2_row )
			{
				QList<QStandardItem*> subSubItCols;
				for ( col = 0; col < totCols; ++ col )
					subSubItCols << new QStandardItem ( 
						QString("item %1%2%3%4").arg(n0_row).arg(n1_row).arg(n2_row).arg(col) );
				
				subSubItem->appendRow(subSubItCols);			
			}
		}			
	}
		
	// Neste exemplo,
	// O "model" foi alimentado com linhas, colunas e sub-n�veis:
	// E ser�o criadas 4 "views" (uma "tree", uma "table", uma "list" e uma "comboBox")
	// relacionadas ao mesmo "model";
	// Cada "view" exibe os dados de uma determinada maneira;

	// 1- ==== a primeira "view" � uma "tree":
	QTreeView *tree = new QTreeView(splitter);
	tree->setModel(model);
	// habilita classifica��o na tree:
	tree->setSortingEnabled(true);
	// classifica
	tree->sortByColumn(0);	
	// expande toda a �rvore:
	tree->expandAll();
	// for�a largura de todas as colunas
	// para exibi��o completa do texto dos seus itens
	for (col = 0; col < totCols; ++col)
		tree->resizeColumnToContents(col);

	// configura o header para permitir mudan�a na ordem de classificac�o:
	QHeaderView * hdrTree = tree->header();
	hdrTree->setClickable (true);
	hdrTree->setSortIndicator(0,Qt::AscendingOrder);
	hdrTree->setSortIndicatorShown(true);
	hdrTree->setMovable(true); // permite mover colunas do header
	
	// 2- ==== a segunda "view" � uma "table"
	QTableView *table = new QTableView(splitter);
	table->setModel(model);
	table->setAlternatingRowColors(true);
	// habilita classifica��o na table:
	table->setSortingEnabled(true);
	// classifica
	table->sortByColumn(0);	

	// configura o header para permitir mudan�a na ordem de classificac�o:
	QHeaderView * hdrTable = table->horizontalHeader();
	hdrTable->setClickable (true);
	hdrTable->setSortIndicator(0,Qt::AscendingOrder);
	hdrTable->setSortIndicatorShown(true);
	hdrTable->setMovable(true); // permite mover colunas do header

	// 3- ==== a terceira view � uma "list": 
	QListView *list = new QListView(splitter);
	list->setModel(model);

	// 4- ==== a quarta "view" � uma "comboBox"
	QComboBox *combo = new QComboBox;
	combo->setModel(model);

	// configura a "splitter" definindo a largura de cada "view"
	int width = 800;	
	QList< int > cols;
	cols << int(width* 0.45) << int(width*0.45) << int(width*0.1);
	splitter->setSizes(cols);	
	
	// layout para agrupar a "combo" e a "splitter":
	QGridLayout * glayMain = new QGridLayout;
	main_wg->setLayout( glayMain);
	glayMain->addWidget( combo, 0, 1); // linha 0, coluna 1;
	glayMain->setRowMinimumHeight(1, glayMain->verticalSpacing() * 4); // linha 1: linha de separa��o
	glayMain->addWidget( splitter, 2, 0, 1, 3 ); // linha 2, coluna 0, rowSpan 1, colSpan 3

	main_wg->setWindowTitle("05_standard - 4 'views' usando o mesmo 'model' (StandardModel)");
	main_wg->resize(800,500);	

	main_wg->show();
	return app.exec();
}
