#ifndef MODEL_H
#define MODEL_H

#include <QtCore>
#include <QAbstractTableModel>

class Model : public QAbstractTableModel
{
	Q_OBJECT

public:
	// constructor:
	Model(const QString & fileName, QObject * parent);

	// Numera��o das duas colunas da table:
	enum { ColunaNome, ColunaCidade, TotalColunas } ; // onde "Total Colunas" ser� 2...

	// Tamanho dos campos/colunas:
	enum { NomeLen = 40, CidadeLen = 30 };

	// Tamanho do registro na fonte de dados (um arquivo texto):
	// soma das colunas mais quebra de linha:
	enum { RegistroLen = NomeLen + CidadeLen + sizeof(char('\n')) };

	// === Redefine virtuais da base:

	// Retorna flags para tratamento das c�lulas:
	Qt::ItemFlags flags(const QModelIndex &index) const;
	// Fornece t�tulos das colunas quando solicitado:
	QVariant headerData(int section, Qt::Orientation orientation , int role) const ;
	// Fornece dados para preenchimento das c�lulas quando solicitado:
	QVariant data(const QModelIndex &index, int role) const ;
	// Se dados foram alterados no model, ir� alter�-los na fonte de dados:
	bool setData(const QModelIndex &index, const QVariant &value, int role);

	// Retorna quantidade de linhas quando solicitado:
	int rowCount(const QModelIndex & ) const { return m_file.size()/RegistroLen; }
	// Retorna quantidade de colunas quando solicitado:
	int columnCount(const QModelIndex & ) const { return TotalColunas ; }

	// === Outras fun��es p�blicas:

	// Retorna situa��o da fonte de dados:
	bool isFileOpen ( ) const { return m_file.isOpen(); }

private:
	// auxiliar para definir deslocamento no arquivo
	bool fileOffset_(int row, int col) const ;

	// arquivo que ser� a fonte de dados:
	mutable QFile m_file ;
	// �ltimos nome e cidade lidos no arquivo:
	mutable QString m_nome;
	mutable QString m_cidade ;

	// OBS: "m_file", "m_nome" e "m_cidade" necessitam da especifica��o "mutable"
	//	j� que ser�o alterados na fun��o "data" que � especificada como "const" (const this)
	// e isso � obrigat�rio, pois est� redefinindo a fun��o da base de mesmo prot�tipo.
};

#endif // MODEL_H
