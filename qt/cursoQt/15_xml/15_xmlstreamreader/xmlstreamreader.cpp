#include <QtGui>
#include <QtXml>
#include <iostream>
#include <QDebug>

#include "xmlstreamreader.h"

XmlStreamReader::XmlStreamReader(QTreeWidget *tree)
	:  treeWidget ( tree )
{
}

bool XmlStreamReader::readFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        std::cerr << "Error: Cannot read file " << qPrintable(fileName)
                  << ": " << qPrintable(file.errorString())
                  << std::endl;
        return false;
    }

	 reader.setDevice(&file);

  reader.readNext();
	while (!reader.atEnd())
	{
		if (reader.isStartElement())
		{
			if (reader.name() == "bookindex")
			{
                readBookindexElement();
			}
			else
			{
                reader.raiseError(QObject::tr("Not a bookindex file"));
            }
		}
		else
		{
            reader.readNext();
        }
    }

    file.close();
    if (reader.hasError()) {
        std::cerr << "Error: Failed to parse file "
                  << qPrintable(fileName) << ": "
                  << qPrintable(reader.errorString()) << std::endl;
        return false;
    } else if (file.error() != QFile::NoError) {
        std::cerr << "Error: Cannot read file " << qPrintable(fileName)
                  << ": " << qPrintable(file.errorString())
                  << std::endl;
        return false;
    }
    return true;
}

void XmlStreamReader::readBookindexElement()
{
    reader.readNext();
	while (!reader.atEnd())
	{
		if (reader.isEndElement())
		{
				qDebug() << "END: " ;
            reader.readNext();
            break;
        }

		if (reader.isStartElement())
		{
			if (reader.name() == "entry")
			{
					qDebug() << "ENTRY: " ;
					 readEntryElement(treeWidget->invisibleRootItem());
			}
			else
			{
					qDebug() << "UNKNOWN: ";
					 skipUnknownElement();
            }
		}
		else
		{
				qDebug() << "NEXT: " ;
				reader.readNext();
        }
    }
}

void XmlStreamReader::readEntryElement(QTreeWidgetItem *parent)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(parent);
    item->setText(0, reader.attributes().value("term").toString());

    reader.readNext();
	while (!reader.atEnd())
	{
		if (reader.isEndElement())
		{
				qDebug() << "END SUB: ";
            reader.readNext();
            break;
        }

		if (reader.isStartElement())
		{
			if (reader.name() == "entry")
			{
					qDebug() << "ENTRY SUB: " ;
					 readEntryElement(item);
			}
			else if (reader.name() == "page")
			{
				qDebug() << "PAGE: " ;
				 readPageElement(item);
			}
			else
			{
					qDebug() << "UNKNOWN SUB: " ;
                skipUnknownElement();
            }
		}
		else
		{
				qDebug() << "NEXT SUB: " ;
            reader.readNext();
        }
    }
}

void XmlStreamReader::readPageElement(QTreeWidgetItem *parent)
{
    QString page = reader.readElementText();
    if (reader.isEndElement())
        reader.readNext();

    QString allPages = parent->text(1);
    if (!allPages.isEmpty())
		allPages += ", ";
    allPages += page;
    parent->setText(1, allPages);
}

void XmlStreamReader::skipUnknownElement()
{
    reader.readNext();
	 while (!reader.atEnd())
	 {
		  if (reader.isEndElement())
		  {
			  qDebug() << "UNKNOWN END: " ;

            reader.readNext();
            break;
        }

		  if (reader.isStartElement())
		  {
				qDebug() << "UNKNOWN SUB-SUB: " ;
				skipUnknownElement();
		  }
		  else
		  {
				reader.readNext();
        }
    }
}
