/***************************************************************************
 *   Copyright (C) 2005-2007 by Grup de Gràfics de Girona                  *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/

#include "qdicomdumpbrowser.h"

#include "dicomtagreader.h"
#include "dicomvalueattribute.h"
#include "dicomsequenceitem.h"
#include "dicomsequenceattribute.h"
#include "image.h"

// Llibreries QT
#include <QTreeWidgetItem>

namespace udg {

const QString NotAvailableValue(QObject::tr("N/A"));

QDICOMDumpBrowser::QDICOMDumpBrowser(QWidget *parent)
 : QDialog(parent)
{
    setupUi(this);

    createConnections();
}

QDICOMDumpBrowser::~QDICOMDumpBrowser()
{

}

void QDICOMDumpBrowser::createConnections()
{
    // Connectem els butons
    connect(m_searchTagLineEdit, SIGNAL(textChanged(const QString &)), SLOT(updateSearch()));
    connect(m_showAllTagsCheckBox, SIGNAL(clicked(bool)), SLOT(updateSearch()));
}


void QDICOMDumpBrowser::searchTag(const QString &textToSearch, bool highlightOnly)
{
    clearSearch();

    if (textToSearch.isEmpty())
    {
        return;
    }

    m_tagsListQTree->expandAll();

    QListIterator<QString> wordsIterator(textToSearch.split(QRegExp("\\s+"), QString::SkipEmptyParts));


    QTreeWidgetItemIterator iterator(m_tagsListQTree);
    while (*iterator)
    {
        bool found = false;
        wordsIterator.toFront();
        while (wordsIterator.hasNext() && !found)
        {
            QString word = wordsIterator.next();
            found = (*iterator)->text(0).contains(word, Qt::CaseInsensitive) || (*iterator)->text(1).contains(word, Qt::CaseInsensitive);
        }

        if (highlightOnly)
        {
            if (found)
            {
                (*iterator)->setSelected(true);
            }
        }
        else
        {
            if (found)
            {
                (*iterator)->setHidden(false);
                QTreeWidgetItem *parent = (*iterator)->parent();
                while (parent && parent->isHidden())
                {
                    parent->setHidden(false);
                    parent = parent->parent();
                }
            }
            else
            {
                (*iterator)->setHidden(true);
            }
        }

        ++iterator;
    }

    if (highlightOnly)
    {
        if (m_tagsListQTree->selectedItems().size() > 0)
        {
            m_tagsListQTree->scrollToItem(m_tagsListQTree->selectedItems().at(0));
        }
    }
}

void QDICOMDumpBrowser::setCurrentDisplayedImage(Image *currentImage)
{
    DICOMTagReader dicomReader;
    bool ok = dicomReader.setFile(currentImage->getPath());

    if (ok)
    {	
        bool resizeColumnsToContents = m_tagsListQTree->topLevelItemCount() == 0;

        m_tagsListQTree->clear();

        QTreeWidgetItem *rootTreeItem = m_tagsListQTree->invisibleRootItem();

        QList<DICOMAttribute*> dicomAttributesList = dicomReader.getDICOMAttributes(DICOMTagReader::ExcludeHeavyTags);
        
        foreach (DICOMAttribute *dicomAttribute, dicomAttributesList)
        {
            if (dicomAttribute->isValueAttribute())
            {
                DICOMValueAttribute *value = dynamic_cast<DICOMValueAttribute*>(dicomAttribute);
                this->addLeaf(rootTreeItem, value);
            }
            else
            {
                DICOMSequenceAttribute *sequence = dynamic_cast<DICOMSequenceAttribute*>(dicomAttribute);
                this->addBranch(rootTreeItem, sequence);
            }
        }

        if (resizeColumnsToContents)
        {
            m_tagsListQTree->resizeColumnToContents(0);
            m_tagsListQTree->resizeColumnToContents(1);
        }

        updateSearch();
        qDeleteAll(dicomAttributesList.begin(), dicomAttributesList.end());
        dicomAttributesList.clear();
    }
}

void QDICOMDumpBrowser::addLeaf(QTreeWidgetItem *trunkTreeItem, DICOMValueAttribute *value)
{
    QTreeWidgetItem *leafTreeItem = new QTreeWidgetItem();
    leafTreeItem->setText(0, value->getTag()->getName());
    leafTreeItem->setText(1, value->getTag()->getKeyAsQString());
    leafTreeItem->setText(2, value->getValueAsQString());
    
    trunkTreeItem->addChild(leafTreeItem);
}

void QDICOMDumpBrowser::addBranch(QTreeWidgetItem *trunkTreeItem, DICOMSequenceAttribute *sequence)
{
    QTreeWidgetItem *trunkBranchItem = new QTreeWidgetItem();
    trunkBranchItem->setText(0, sequence->getTag()->getName());
    trunkBranchItem->setText(1, sequence->getTag()->getKeyAsQString());
    trunkBranchItem->setText(2, "");

    foreach (DICOMSequenceItem *sequenceItem, sequence->getItems())
    {
        QTreeWidgetItem *qTreeSequenceItem = new QTreeWidgetItem();
        qTreeSequenceItem->setText(0, "Item");
        qTreeSequenceItem->setText(1, "");
        qTreeSequenceItem->setText(2, "");

        foreach (DICOMAttribute *dicomAttribute, sequenceItem->getAttributes())
        {
            if (dicomAttribute->isValueAttribute())
            {
                DICOMValueAttribute *subValue = dynamic_cast<DICOMValueAttribute*>(dicomAttribute);
                this->addLeaf(qTreeSequenceItem, subValue);
            }
            else
            {
                DICOMSequenceAttribute *subSequence = dynamic_cast<DICOMSequenceAttribute*>(dicomAttribute);
                this->addBranch(qTreeSequenceItem, subSequence);
            }
        }
        trunkBranchItem->addChild(qTreeSequenceItem);
    }
    trunkTreeItem->addChild(trunkBranchItem);
}

void QDICOMDumpBrowser::updateSearch()
{
    searchTag(m_searchTagLineEdit->text(), m_showAllTagsCheckBox->isChecked());
}

void QDICOMDumpBrowser::clearSearch()
{
    m_tagsListQTree->clearSelection();

    QTreeWidgetItemIterator iterator(m_tagsListQTree);
    while (*iterator)
    {
        (*iterator)->setHidden(false);
        ++iterator;
    }
}

};
