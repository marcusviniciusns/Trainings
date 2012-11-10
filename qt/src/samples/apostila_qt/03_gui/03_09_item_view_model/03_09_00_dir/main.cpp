#include <QtGui>

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	// cria um objeto "splitter" para compartilhar widgets:    
	QSplitter *splitter = new QSplitter;

	// cria um "model" usando o "QDirModel"
	QDirModel *model = new QDirModel(&app); // ser� deletado
												// na destrutora de QApplication (QObject)

	// Neste exemplo,
	// O "model" ser� configurado para pegar dados 
	// a partir de determinado(s) drive(s) ou diret�rio(s).
	// E ser�o criadas tr�s "views" (uma "tree", uma "table", e uma "list" )
	// relacionadas ao mesmo "model";
	// Cada "view" exibe os dados de uma determinada maneira;

	// 1- ==== a primeira "view" � uma "tree":
	QTreeView *tree = new QTreeView(splitter);
	tree->setModel(model);
	
	// O DirModel j� oferece um "RootIndex" a partir do qual
	// cada "view" exibe drives, diret�rios e arquivos abaixo dele.
	// No Windows o "RootIndex" default exibir�
	// a relac�o de todos os drives; 
	// para outros Sistemas apenas o root ('/').

	// Mas podemos alterar o "RootIndex" de cada view.
	
	// Um exemplo: partindo do diret�rio atual:
	//    tree->setRootIndex(model->index(QDir::currentPath()));
		 #ifdef Q_OS_WIN
			 QString rootNdx("C:/cursoQt");
		 #else
			 QString rootNdx("/home/user_name/cursoQt");
		#endif
	
//		tree->setRootIndex(model->index(rootNdx));

	// MAS, temos a fun��o que develve o root do SO:
	// outro exemplo: partindo do diretorio "root" ( "/" ou "C:/"):
	//    tree->setRootIndex(model->index(QDir::rootPath()));
	// ou ainda, um diret�rio espec�fico:
	//    tree->setRootIndex(model->index("/cursoqt"));
	 // tree->setRootIndex(model->index("/cursoqt"));
	/*	
		// outro exemplo: partindo de determinado drive:
		QFileInfoList fiList = QDir::drives();
		foreach( const QFileInfo & fi, fiList )
		{	
			if ( fi.filePath()=="D:/" ) // apenas o drive "D:/"
			{
				tree->setRootIndex(model->index(fi.filePath()));
				break;
			}				
		}		
	*/
	
	// 2- ==== a segunda "view" � uma "table"
	QTableView *table = new QTableView(splitter);

	table->setModel(model);
	//    table->setRootIndex(...); // idem

	// 3- ==== a terceira "view" � uma "list": 
	QListView *list = new QListView(splitter);
	list->setModel(model);
	//list->setRootIndex(model->index(rootNdx)); // idem

	// configura e exibe a "splitter"
	splitter->setWindowTitle("00_dir - 3 'views' usando o mesmo 'model' (diret�rios)");
	int width=800;	
	splitter->resize(width,500);	
	
	// largura de cada "view"
	QList< int > cols;
	cols << int(width* 0.45) << int(width*0.45) << int(width*0.1); 
	splitter->setSizes(cols);	
	
	splitter->setChildrenCollapsible(false); // impede que cada banda colapse totalmente
	
	splitter->show();
	int ret  = app.exec();
	delete splitter;  // porque n�o alocou o splitter na PILHA ????

	return ret;
}
