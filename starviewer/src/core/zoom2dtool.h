/***************************************************************************
 *   Copyright (C) 2005-2006 by Grup de Gr�fics de Girona                  *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/
#ifndef UDGZOOM2DTOOL_H
#define UDGZOOM2DTOOL_H

#include "tool.h"

namespace udg {

/**
	@author Grup de Gr�fics de Girona  ( GGG ) <vismed@ima.udg.es>
*/

class Q2DViewer;

class Zoom2DTool : public Tool
{
Q_OBJECT
public:
    enum { NONE , ZOOMING };
    Zoom2DTool( Q2DViewer *viewer , QObject *parent = 0, const char *name = 0 );

    ~Zoom2DTool();
    
    void handleEvent( unsigned long eventID );

private:
    void createAction(){};
    
    Q2DViewer *m_2DViewer;
    
/// \TODO potser aquests m�todes slots passen a ser p�blics
private slots: 
    /// Comen�a el zoom
    void startZoom();

    /// Calcula el nou zoom
    void doZoom();

    /// Atura l'estat de zoom
    void endZoom();

};

}

#endif
