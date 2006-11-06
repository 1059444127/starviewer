/***************************************************************************
 *   Copyright (C) 2005 by Grup de Gr�fics de Girona                       *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/
#ifndef UDGTOOL_H
#define UDGTOOL_H

#include <QObject>

// FWD declarations
class QAction;

namespace udg {

class QViewer;

/**
Classe base per a totes les classes de tools

@author Grup de Gr�fics de Girona  ( GGG )
*/

class Tool : public QObject{
Q_OBJECT

public slots:
    /// Decideix qu� s'ha de fer per cada event rebut. M�tode virtual pur que es re-implementa obligat�riament en cada classe filla.
    virtual void handleEvent( unsigned long eventID ) = 0;

protected:
    /// Per controlar l'estat de la tool
    int m_state;

};

};  //  end  namespace udg

#endif

