#include <QtGui>

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	QWidget main_wg;

	// cria um objeto "splitter" para compartilhar widgets:    
	QSplitter *splitter = new QSplitter(&main_wg);

	// cria um "model" usando o "QStringListModel"
	QStringListModel *model = new QStringListModel(&main_wg);

	QStringList slist;
	
	slist << "Maria"<< "Jo�o"<< "Zulmira" << "Pedro"<< "Antonio"
				<< "Berenice" << "Artur" << "Carlos";
	
	model->setStringList(slist);
	model->sort(0); // classifica pela coluna zero

	// Neste exemplo,
	// O "model" ser� configurado para pegar dados 
	// de uma stringlist.
	// E ser�o criadas 4 "views" (uma "tree", uma "table", uma "list" e uma "comboBox")
	// relacionadas ao mesmo "model";
	// Cada "view" exibe os dados de uma determinada maneira;

	// 1- ==== a primeira "view" � uma "tree":
	QTreeView *tree = new QTreeView(splitter);
	tree->setModel(model);
	tree->setHeaderHidden(true);

	// retira a linha indicadora de n�vel da �rvore:
//	tree->setRootIsDecorated(false);

	// 2- ==== a segunda "view" � uma "table"
	QTableView *table = new QTableView(splitter);
	table->setModel(model);
	table->setAlternatingRowColors(true);
	table->horizontalHeader()->setVisible(false);
	table->horizontalHeader()->setStretchLastSection(true);

	// 3- ==== a terceira view � uma "list": 
	QListView *list = new QListView(splitter);
	list->setModel(model);

	// deixo todas as views editarem, menos a listView:
	list->setEditTriggers(QAbstractItemView::NoEditTriggers);
	
	// 4- ==== a quarta "view" � uma "comboBox"
	QComboBox *combo = new QComboBox;
	combo->setModel(model);

	// layout para agrupar a "combo" e a "splitter":
	QGridLayout * glayMain = new QGridLayout;
	main_wg.setLayout( glayMain);

	glayMain->addWidget(combo, 0, 1 ); // linha 0, coluna 1;
	glayMain->setRowMinimumHeight(1, glayMain->verticalSpacing() * 4); // linha 1: linha de separa��o
	glayMain->addWidget( splitter, 2, 0, 1, 3 ); // linha 2, coluna 0, rowSpan 1, colSpan 3

	main_wg.setWindowTitle("02_strings - 4 'views' usando o mesmo 'model' (StringList)");
	main_wg.resize(800,500);

	main_wg.show();
	int ret = app.exec();

	// se dados foram alterados, altera a stringlist:
	int rows = model->rowCount();
	for ( int r=0; r<rows; ++r)
	{
		QModelIndex index ( model->index(r,0) );
		QString itemData (model->data(index, Qt::DisplayRole).toString() );
		if ( slist[r] != itemData)
			slist[r] = itemData;
	}
        foreach ( const QString & str, slist)
              qDebug() << str;

	return ret;
}

