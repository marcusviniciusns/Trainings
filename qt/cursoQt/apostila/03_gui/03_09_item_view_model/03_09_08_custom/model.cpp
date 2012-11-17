
#include <stdexcept>
#include <QMessageBox>

#include "model.h"

Model::Model(const QString & fileName, QObject * parent)
	: QAbstractTableModel( parent ) // construtora base
	, m_file(fileName) // construtora de QFile (recebe nome do arquivo)
{
	// abre o arquivo para leitura e grava��o:
	if ( !m_file.open(QIODevice::ReadWrite) )
		throw std::runtime_error("Erro ao abrir arquivo");
}

// Retorna flags para definir comportamento
// de uma c�lula:
Qt::ItemFlags Model::flags(const QModelIndex &index) const
{
	Qt::ItemFlags flags = QAbstractItemModel::flags(index);

	// se a coluna for "Cidade":
	if (index.column() == ColunaCidade)
		flags |= Qt::ItemIsEditable;  // esta coluna pode ser editada.
	else
		flags &= ~Qt::ItemIsEditable;  // esta n�o.

	return flags;
}

// Fornece t�tulos das colunas quando solicitado:
QVariant Model::headerData(int section,
							   Qt::Orientation orientation ,
							   int role) const
{
	// Se est� pedindo o texto para t�tulos de colunas:
	if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
	{
		if ( section == ColunaNome ) // "section" � o n�mero da coluna
			return "Nome";
		else
			return "Cidade";
	}
	if (role == Qt::DisplayRole && orientation == Qt::Vertical)
		return section+1;

	// Nada a acrescentar:
	return QVariant();  // "variant" vazia.
}

// Retorna dados para preencher uma c�lula
// por solicita��o da base:
QVariant Model::data(const QModelIndex &index, int role) const
{
	static const char * strErro = "***ERRO***" ;

	if (!index.isValid())
		return QVariant();

	// analisa que tipo de dados est� sendo pedidos pela base:
	if (role == Qt::TextAlignmentRole) // alinhamento
	{		
		if ( index.column() == ColunaNome )
			return int(Qt::AlignRight | Qt::AlignVCenter);
		else
			return int(Qt::AlignLeft | Qt::AlignVCenter);
	}
	else if (role == Qt::DisplayRole) // texto para uma c�lula
	{
		// Ler o campo correspondente � linha e coluna atuais do model.

		if ( ! fileOffset_(index.row(),  index.column() ) )  // reposiciona, se necess�rio
			return strErro ; // falha no posicionamento

		if ( index.column() == ColunaNome )
		{
			m_nome = m_file.read(NomeLen);
			if ( m_nome.length() == NomeLen)
				return m_nome.trimmed(); // retira brancos e retorna
			else
				return strErro;  // Falha ao ler. N�o leu a quantidade de bytes solicitada.
		}
		else
		{
			m_cidade = m_file.read(CidadeLen);
			if ( m_cidade.length() == CidadeLen)
				return m_cidade.trimmed(); // retira brancos e retorna;
			else
				return strErro;  // Falha ao ler. N�o leu a quantidade de bytes solicitada.
		}
	}

	// nada a acrescentar:
	return QVariant(); // "variant" vazia.
}

// Se dados foram alterados (caso a coluna seja edit�vel)
// confirma ou nega a altera��o:
bool Model::setData(const QModelIndex &index,
						const QVariant &value, int role)
{
	// se os dados foram editados:
	if ( role == Qt::EditRole)
	{
		// Gravar na linha e coluna correspondentes ao model:

		if ( ! fileOffset_( index.row(), index.column() ) )  // reposiciona, se necess�rio
			return false; // falha ao posicionar

		if ( index.column() == ColunaNome )
		{
			m_nome = QString("%1").arg(value.toString(),  -Model::NomeLen)  ;   // "-" alinhar � esquerda, com brancos � direita
			if ( m_file.write(m_nome.toAscii(), Model::NomeLen) != Model::NomeLen )
                        return false; // erro (n�o conseguiu gravar a quantidade de bytes solicitada)
		}
		else
		{
			m_cidade = QString("%1").arg(value.toString(),  -Model::CidadeLen)  ;   // "-" alinhar � esquerda, com brancos � direita
			if ( m_file.write(m_cidade.toAscii(), Model::CidadeLen) != Model::CidadeLen )
				return false; // erro (n�o conseguiu gravar a quantidade de bytes solicitada)
		}

		// confirma mudan�a dos dados no model:
		emit dataChanged(index, index);
		return true;
	}

	// nada a ser alterado no model:
	return false;
}

// auxiliar para definir deslocamento no arquivo
bool Model::fileOffset_( int row, int col ) const
{
	// deslocamento no arquivo:
	int offset = row * RegistroLen;

	if ( col == ColunaCidade )
		offset += NomeLen ; // � necess�rio pular o tamanho do nome

	if ( offset != m_file.pos() ) // se � necess�rio um reposicionamento
		return m_file.seek(offset); // reposiciona

	// posi��o atual j� � a solicitada:
	return true ;
}

