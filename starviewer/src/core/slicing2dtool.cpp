/***************************************************************************
 *   Copyright (C) 2005-2006 by Grup de Gr�fics de Girona                  *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/
#include "slicing2dtool.h"
#include <QAction>
#include <QVTKWidget.h>
#include <vtkRenderWindowInteractor.h>

#include "logging.h"
#include "q2dviewer.h"
#include <vtkCommand.h>

namespace udg {

Slicing2DTool::Slicing2DTool( Q2DViewer *viewer , QObject *parent, const char *name )
{
    m_state = NONE;
    m_startPosition[0] = 0;
    m_startPosition[1] = 0;
    m_currentPosition[0] = 0;
    m_currentPosition[1] = 0;
    m_2DViewer = viewer;
}

Slicing2DTool::~Slicing2DTool()
{
}

void Slicing2DTool::handleEvent( unsigned long eventID )
{
    switch( eventID )
    {
    case vtkCommand::LeftButtonPressEvent:
        this->startSlicing();
    break;

    case vtkCommand::MouseMoveEvent:
        this->doSlicing();
    break;

    case vtkCommand::LeftButtonReleaseEvent:
        this->endSlicing();
    break;

    case vtkCommand::MouseWheelForwardEvent:
        m_2DViewer->setSlice( m_2DViewer->getSlice() + 1 );
        //\TODO mirar si podem fer una manera m�s atom�tica per actualitzar la informaci� de voxel
        m_2DViewer->updateVoxelInformation();
    break;

    case vtkCommand::MouseWheelBackwardEvent:
        m_2DViewer->setSlice( m_2DViewer->getSlice() - 1 );
        //\TODO mirar si podem fer una manera m�s atom�tica per actualitzar la informaci� de voxel
        m_2DViewer->updateVoxelInformation();
    break;

    default:
    break;
    }
}

void Slicing2DTool::startSlicing()
{
    m_state = SLICING;
    m_startPosition[0] = m_2DViewer->getInteractor()->GetEventPosition()[0];
    m_startPosition[1] = m_2DViewer->getInteractor()->GetEventPosition()[1];
}

void Slicing2DTool::doSlicing()
{
    if( m_state == SLICING )
    {
        m_currentPosition[0] = m_2DViewer->getInteractor()->GetEventPosition()[0];
        m_currentPosition[1] = m_2DViewer->getInteractor()->GetEventPosition()[1];
        int dx = m_currentPosition[0] - m_startPosition[0];
        int dy = m_startPosition[1] - m_currentPosition[1];

        m_startPosition[0] = m_currentPosition[0];
        m_startPosition[1] = m_currentPosition[1];

        int value, increment;
        if( dy )
            value = dy/abs(dy);
        else
            value = dx/abs(dx);

        if( value < 0 )
            increment = -1;
        else if( value > 0 )
            increment = 1;

        m_2DViewer->setSlice( m_2DViewer->getSlice() + increment );
    }
}

void Slicing2DTool::endSlicing()
{
    m_state = NONE;
}

}
