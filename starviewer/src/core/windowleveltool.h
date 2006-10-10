/***************************************************************************
 *   Copyright (C) 2005-2006 by Grup de Gr�fics de Girona                  *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/
#ifndef UDGWINDOWLEVELTOOL_H
#define UDGWINDOWLEVELTOOL_H

#include "tool.h"

namespace udg {

/**
	@author Grup de Gr�fics de Girona  ( GGG ) <vismed@ima.udg.es>
*/

class Q2DViewer;

class WindowLevelTool : public Tool
{
Q_OBJECT
public:
    /// estats de la tool
    enum { NONE , WINDOWLEVELING };
    
    WindowLevelTool( Q2DViewer *viewer , QObject *parent = 0 , const char *name = 0 );

    ~WindowLevelTool();
    
    void handleEvent( unsigned long eventID );

private:
    void createAction(){};
    
    Q2DViewer *m_2DViewer;

    /// valors per controlar el mapeig del window level
    double m_initialWindow, m_initialLevel, m_currentWindow, m_currentLevel;
    int m_windowLevelStartPosition[2], m_windowLevelCurrentPosition[2];
    
/// \TODO potser aquests m�todes slots passen a ser p�blics
private slots: 
    /// Comen�a el window level
    void startWindowLevel();

    /// Calcula el nou window level
    void doWindowLevel();

    /// Atura l'estat de window level
    void endWindowLevel();

};

}

#endif
