/***************************************************************************
 *   Copyright (C) 2005-2006 by Grup de Gràfics de Girona                  *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/
#include "viewerslayout.h"

#include "logging.h"

namespace udg {

ViewersLayout::ViewersLayout(QWidget *parent) 
 : QWidget(parent), m_selectedViewer(0)
{
    initLayouts();
}

ViewersLayout::~ViewersLayout()
{
}

void ViewersLayout::initLayouts()
{
    m_visibleRows = 1;
    m_visibleColumns = 1;
    m_totalRows = 1;
    m_totalColumns = 1;

    m_gridLayout = new QGridLayout();
    m_gridLayout->setSpacing(0);
    m_gridLayout->setMargin(0);

    m_viewersLayout = new QGridLayout();
    m_viewersLayout->setSpacing(0);
    m_viewersLayout->setMargin(0);

    m_gridLayout->addLayout(m_viewersLayout, 0, 0);

    this->setLayout(m_gridLayout);
    m_gridLayout->update();

    m_isRegular = true;
}

void ViewersLayout::removeLayouts()
{
    int numberOfViewers = getNumberOfViewers();

    for (int i = 0; i < numberOfViewers; ++i)
    {
        m_viewersLayout->removeWidget(getViewerWidget(i));
    }
    m_geometriesList.clear();
}

void ViewersLayout::restoreLayouts()
{
    int numberOfViewers = getNumberOfViewers();

    /// Si tenim més visors que el grid regular que teníem construït, el regenerem.
    if (numberOfViewers > m_totalColumns * m_totalRows)
    {
        m_totalColumns = ceil(sqrt((double)numberOfViewers));
        m_totalRows = m_totalColumns;

        if ((m_totalRows * m_totalColumns) > numberOfViewers)
        {
            for (int i = 0; i < (m_totalRows * m_totalColumns) - numberOfViewers; ++i)
            {
                m_vectorViewers.push_back(getNewQ2DViewerWidget());
            }
        }
    }

    m_viewersLayout->addWidget(getViewerWidget(0), 0, 0);

    int column = 1;
    int row = 0;
    // S'amaguen tots i es deixa el principal, es tornen a posar amb layout
    for (int i = 1; i < getNumberOfViewers(); ++i)
    {
        getViewerWidget(i)->hide();
        m_viewersLayout->addWidget(getViewerWidget(i), row, column);

        ++column;
        if (column >= m_totalColumns)
        {
            ++row;
            column = 0;
        }
    }

    setSelectedViewer(getViewerWidget(0));
    m_visibleRows = 1;
    m_visibleColumns = 1;
}

Q2DViewerWidget* ViewersLayout::getSelectedViewer()
{
    return m_selectedViewer;
}

Q2DViewerWidget* ViewersLayout::getNewQ2DViewerWidget()
{
    Q2DViewerWidget *newViewer = new Q2DViewerWidget(this);
    connect(newViewer, SIGNAL(selected(Q2DViewerWidget *)), SLOT(setSelectedViewer(Q2DViewerWidget *)));
    // Per defecte no li posem cap annotació
    newViewer->getViewer()->removeAnnotation(Q2DViewer::AllAnnotation);

    emit viewerAdded(newViewer);
    return newViewer;
}

void ViewersLayout::addColumns(int columns)
{
    int viewerPosition = m_visibleColumns;
    Q2DViewerWidget *newViewer = 0;
    
    while (columns > 0)
    {
        ++m_visibleColumns;
        ++m_totalColumns;
        for (int rows = 0; rows < m_viewersLayout->rowCount(); ++rows)
        {
            newViewer = getNewQ2DViewerWidget();
            m_viewersLayout->addWidget(newViewer, rows, m_totalColumns - 1);
            m_vectorViewers.insert(viewerPosition,newViewer);
            viewerPosition += m_visibleColumns;
            if (rows >= m_visibleRows)
            {
                newViewer->hide();
                emit viewerRemoved(newViewer);
            }
        }
        viewerPosition = m_visibleColumns;
        --columns;
    }
}

void ViewersLayout::addRows(int rows)
{
    Q2DViewerWidget *newViewer = 0;

    while (rows > 0)
    {
        m_visibleRows += 1;
        m_totalRows += 1;
        //Afegim tants widgets com columnes
        for (int column = 0; column < m_totalColumns; ++column)
        {
            newViewer = getNewQ2DViewerWidget();
            m_viewersLayout->addWidget(newViewer, m_visibleRows - 1, column);
            m_vectorViewers.push_back(newViewer);
            if (column >= m_visibleColumns)
            {
                newViewer->hide();
                emit viewerRemoved(newViewer);
            }
        }
        --rows;
    }
}

void ViewersLayout::removeColumns(int columns)
{
    int viewerPosition = m_visibleColumns - 1;
    Q2DViewerWidget *oldViewer = 0;
    
    while (columns > 0 && m_visibleColumns > 1)
    {
        // Eliminem un widget de cada fila per tenir una columna menys
        for (int rows = 0; rows < m_viewersLayout->rowCount(); ++rows)
        {
            oldViewer = getViewerWidget(viewerPosition);
            m_viewersLayout->removeWidget(oldViewer);
            m_vectorViewers.remove(viewerPosition);
            if (m_selectedViewer == oldViewer)
            {
                setSelectedViewer(getViewerWidget(0));
            }
            delete oldViewer;
            viewerPosition += (m_visibleColumns - 1);
        }
        --m_visibleColumns;
        viewerPosition = m_visibleColumns - 1;
        --columns;
    }
}

void ViewersLayout::removeRows(int rows)
{
    int viewerPosition = m_vectorViewers.count() - 1;
    Q2DViewerWidget *oldViewer = 0;

    while (rows > 0 && m_visibleRows > 1)
    {
        // Eliminem tants widgets com columnes
        for (int i = 0; i < m_visibleColumns; ++i)
        {
            oldViewer = getViewerWidget(viewerPosition);
            m_vectorViewers.remove(viewerPosition);
            m_viewersLayout->removeWidget(oldViewer);
            // TODO eliminar els viewers que treiem del toolManager???
            if (m_selectedViewer == oldViewer)
            {
                setSelectedViewer(getViewerWidget(0));
            }
            delete oldViewer;
            --viewerPosition;
        }
        --m_visibleRows;
        --rows;
    }
}

void ViewersLayout::setGrid(int rows, int columns)
{
    // Mirem si les tenim amagades i mostrem totes les necessaries
    int windowsToShow = 0;
    int windowsToCreate = 0;
	
    if (!m_isRegular)
    {
        restoreLayouts();
    }

    if (rows > m_visibleRows)
    {
        int windowsToHide = m_totalRows - m_visibleRows;

        if (windowsToHide < (rows - m_visibleRows))
        {
            windowsToShow = windowsToHide;
        }
        else
        {
            windowsToShow = rows - m_visibleRows;
        }

        showRows(windowsToShow);

        if (rows > m_totalRows)
        {
            windowsToCreate = rows - m_totalRows;
        }

        addRows(windowsToCreate);
    }
    else if (rows < m_visibleRows)
    {
        hideRows(m_visibleRows - rows);
    }

    windowsToShow = 0;
    windowsToCreate = 0;

    if (columns > m_visibleColumns)
    {
        int windowsToHide = m_totalColumns - m_visibleColumns;

        if (windowsToHide < (columns - m_visibleColumns))
        {
            windowsToShow = windowsToHide;
        }
        else
        {
            windowsToShow = columns - m_visibleColumns;
        }

        showColumns(windowsToShow);

        if (columns > m_totalColumns)
        {
            windowsToCreate = columns - m_totalColumns;
        }

        addColumns(windowsToCreate);
    }
    else if (columns < m_visibleColumns)
    {
        hideColumns(m_visibleColumns - columns);
    }
    m_isRegular = true;
}

void ViewersLayout::setGrid(const QStringList &geometriesList)
{
    Q2DViewerWidget *newViewer = 0;
    QString geometry;
    int numberOfElements = geometriesList.size();

    if (m_gridLayout) 
    {
        removeLayouts();
    }

    for (int i = 0; i < numberOfElements; ++i)
    {
        newViewer = getViewerWidget(i);

        if (newViewer == 0)
        {
            newViewer = getNewQ2DViewerWidget();
            m_vectorViewers.push_back(newViewer);
        }
        geometry = geometriesList.at(i);
        setViewerGeometry(newViewer, geometry);
    }

    m_geometriesList = geometriesList;
    m_isRegular = false;
}

Q2DViewerWidget* ViewersLayout::addViewer(const QString &geometry)
{
    Q2DViewerWidget *newViewer = 0;

    m_visibleRows = 0;
    m_visibleColumns = 0;
    if (m_isRegular) 
    {
        removeLayouts();
    }

    if (getNumberOfVisibleViewers() < getNumberOfViewers())
    {
        newViewer = getViewerWidget(getNumberOfVisibleViewers());
    }
    else
    {
        newViewer = getNewQ2DViewerWidget();
        m_vectorViewers.push_back(newViewer);
    }
	
    setViewerGeometry(newViewer, geometry);
    newViewer->show();

    m_geometriesList << geometry;
    m_isRegular = false;

    return newViewer;
}

void ViewersLayout::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    if (!m_isRegular)
    {
        for (int i = 0; i < getNumberOfVisibleViewers(); ++i)
        {
            setViewerGeometry(getViewerWidget(i), m_geometriesList.at(i));
        }
    }
}

void ViewersLayout::setSelectedViewer(Q2DViewerWidget *viewer)
{
    if (!viewer)
    {
        DEBUG_LOG("El Viewer donat és NUL!");
        return;
    }

    if (viewer != m_selectedViewer)
    {
        if (m_selectedViewer)
        {
            m_selectedViewer->setSelected(false);
        }
        m_selectedViewer = viewer;
        m_selectedViewer->setSelected(true);
        emit selectedViewerChanged(m_selectedViewer);
    }
}

void ViewersLayout::showRows(int rows)
{
    Q2DViewerWidget *viewer = 0;

    while (rows > 0)
    {
        for (int columnNumber = 0; columnNumber < m_visibleColumns; ++columnNumber)
        {
            viewer = getViewerWidget((m_totalColumns * m_visibleRows) + columnNumber);
            viewer->show();
        }
        ++m_visibleRows;
        --rows;
    }
}

void ViewersLayout::hideRows(int rows)
{
    Q2DViewerWidget *viewer = 0;

    while (rows > 0)
    {
        --m_visibleRows;
        for (int columnNumber = 0; columnNumber < m_visibleColumns; ++columnNumber)
        {
            viewer = getViewerWidget(((m_totalColumns * m_visibleRows) + columnNumber));
            viewer->hide();
            emit viewerRemoved(viewer);
            if (m_selectedViewer == viewer)
            {
                setSelectedViewer(getViewerWidget(0));
            }
        }
        --rows;
    }
}

void ViewersLayout::showColumns(int columns)
{
    Q2DViewerWidget *viewer = 0;

    while (columns > 0)
    {
        for (int rowNumber = 0; rowNumber < m_visibleRows; ++rowNumber)
        {
            viewer = getViewerWidget((m_totalColumns * rowNumber) + m_visibleColumns);
            viewer->show();
        }
        ++m_visibleColumns;
        --columns;
    }
}

void ViewersLayout::hideColumns(int columns)
{
    Q2DViewerWidget *viewer;

    while (columns > 0)
    {
        --m_visibleColumns;
        for (int rowNumber = 0; rowNumber < m_visibleRows; ++rowNumber)
        {
            viewer = getViewerWidget((m_totalColumns * rowNumber) + m_visibleColumns);
            viewer->hide();
            emit viewerRemoved(viewer);
            if (m_selectedViewer == viewer)
            {
                setSelectedViewer(getViewerWidget(0));
            }
        }
        --columns;
    }
}

int ViewersLayout::getNumberOfViewers()
{
    return m_vectorViewers.size();
}

Q2DViewerWidget* ViewersLayout::getViewerWidget(int number)
{
    Q2DViewerWidget *viewerWidget = 0;
    
    // Comprovem que el viewer demanat estigui dins del rang
    if (number < getNumberOfViewers() && number >= 0)
    {
        viewerWidget = m_vectorViewers.at(number);
    }

    return viewerWidget;
}

void ViewersLayout::setViewerGeometry(Q2DViewerWidget *viewer, const QString &geometry)
{
    if (!viewer)
    {
        DEBUG_LOG("El viewer proporcionat és nul! No s'aplicarà cap geometria.");
        return;
    }
    
    QStringList splittedGeometryList = geometry.split("\\");
    if (splittedGeometryList.count() < 4)
    {
        DEBUG_LOG("La geometria proporcionada no conté el nombre d'elements necessaris o està mal formada. Geometry dump: [" + geometry + "]. No s'aplicarà cap geometria al viewer proporcinat.");
        WARN_LOG("La geometria proporcionada no conté el nombre d'elements necessaris o està mal formada. Geometry dump: [" + geometry + "]. No s'aplicarà cap geometria al viewer proporcinat.");
        return;
    }

    double x1;
    double y1;
    double x2;
    double y2;
    x1 = splittedGeometryList.at(0).toDouble();
    y1 = splittedGeometryList.at(1).toDouble();
    x2 = splittedGeometryList.at(2).toDouble();
    y2 = splittedGeometryList.at(3).toDouble();

    int screenX = this->width();
    int screenY = this->height();
    viewer->setGeometry(x1 * screenX, (1 - y1) * screenY, (x2 - x1) * screenX, (y1 - y2) * screenY);
}

int ViewersLayout::getNumberOfVisibleViewers() const
{
    return m_geometriesList.size();
}

}
