
#include <QMainWindow>
#include <QMenu>
#include <QTextEdit>
#include <QCloseEvent>

class Editor : public QMainWindow
{
	Q_OBJECT
	
	private:
		QTextEdit * textEdit;
		QMenu *fileMenu;
		bool _m_bCloseWindow;
		QString _m_CurrentFileName;
	
	public:
		Editor( QWidget * pParent = NULL);
		void _statusMessage();
	
	// eventos (com fun��es virtuais):
	protected:
		void closeEvent ( QCloseEvent * event );

	private:
		bool _CanCloseNow();
		void _setCurrentFile(const QString & PathAndFileName);
		bool _save(const QString & PathAndFileName);

	private slots:
		void _newFile();
	    void _openFile();
		void _saveFile();
		void _saveFileAs();
		void _quit();
		void _aboutbox();
		void _print();

	 	void _textChanged(); // ... modified...
		void _statusBarMsgChanged( const QString & msg );

		// nome padr�o de um slot quando � usada
		// 	QMetaObject::connectSlotsByName( objeto_ptr );
		void on_cbFont_currentIndexChanged( int );
	
		// o slot abaixo (que era conectado manualmente)
		// n�o ser� mais necess�rio
		// pois ser� usado o slot acima 
		// void FontChanged( int );
};

