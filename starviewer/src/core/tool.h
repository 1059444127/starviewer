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
public:
    /// Enumeraci� d'events
    enum EventID{ LeftButtonClick , LeftButtonRelease, RightButtonClick , RightButtonRelease , MiddleButtonClick , MiddleButtonRelease , MouseWheelForward , MouseWheelBackward , MouseMove };

    /// Retorna la QAction de la tool. Cada tool espec�fica es crea la seva.
    QAction *getAction(){ return m_action; };

    /// Retorna el nom que identifica la tool. \TODO Es pot fer servir l'ObjectName del propi QObject o posar un nou membre que desi el nom de la tool
    virtual QString getToolName(){ return "none"; };
    
public slots:
    /// Decideix qu� s'ha de fer per cada event rebut. M�tode virtual pur que es re-implementa obligat�riament en cada classe filla.
    virtual void handleEvent( unsigned long eventID ) = 0;

protected:
    /// M�tode virtual re-implementat a cada sub-classe que crear� i configurar� la QAction a mida
    virtual void createAction(){};
    
    /// Per controlar l'estat de la tool
    int m_state;
    
    /// QAction associada a la tool. La QAction ha de ser static, ja que per exemple, si en una extensi� tenim 3 finestres diferents que fan servir la mateixa tool, tindrem tres inst�ncies de tool que modficaran cada finestra, per� nom�s una sola tool que val igual per les tres. �s a dir, donem un acc�s unificat (enable,disable,etc), per� una interacci� individual
    /*static*/ QAction *m_action;

}; 

};  //  end  namespace udg 

#endif

