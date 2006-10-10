/***************************************************************************
 *   Copyright (C) 2005-2006 by Grup de Gr�fics de Girona                  *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/
#ifndef UDGMOVE2DTOOL_H
#define UDGMOVE2DTOOL_H

#include "tool.h"

namespace udg {

class Q2DViewer;

/**
	@author Grup de Gr�fics de Girona  ( GGG ) <vismed@ima.udg.es>
*/
class Move2DTool : public Tool
{
Q_OBJECT
public:
    enum { NONE , MOVING };
    Move2DTool( Q2DViewer *viewer , QObject *parent = 0, const char *name = 0 );

    ~Move2DTool();

    void handleEvent( unsigned long eventID );
    
private:
    void createAction(){};
    
    Q2DViewer *m_2DViewer;
    
/// \TODO potser aquests m�todes slots passen a ser p�blics
private slots: 
    /// Comen�a el move
    void startMove();

    /// mou
    void doMove();

    /// Atura l'estat de move
    void endMove();

};

}

#endif
