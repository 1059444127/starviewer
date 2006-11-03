/***************************************************************************
 *   Copyright (C) 2005 by Grup de Gr�fics de Girona                       *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/
#ifndef UDGEXTENSIONWORKSPACE_H
#define UDGEXTENSIONWORKSPACE_H

#include <QTabWidget>

// Forward declarations
class QAction;
class QToolButton;

namespace udg {

/**
@author Grup de Gr�fics de Girona  ( GGG )
*/
class ExtensionWorkspace : public QTabWidget{
Q_OBJECT
public:

    ExtensionWorkspace(QWidget *parent = 0, const char *name = 0);

    ~ExtensionWorkspace();

    /// Afegeix una nova aplicaci� \TODO: par�metre de recursos?, afegir un id d'aplicaci� per poder-la referenciar, afegir una label/nom, +altres possibles par�metres
    void addApplication( QWidget *application , QString caption );

    /// Treu una aplicaci� de l'espai de mini-aplicacions \TODO: fer tamb� per id?
    void removeApplication( QWidget *application );

    /// Obt�/Assigna l'�ndex de la p�gina actual
    int getLastIndex() const { return m_lastIndex; }
    void setLastIndex( int index ){ m_lastIndex = index; }

public slots:

    /// Per tancar l'aplicaci� que est� oberta en aquell moment
    void closeCurrentApplication();

private:

    /// Bot� per tancar la pestanya actual
    QToolButton *m_closeTabButton;

    /// Acci� associada al tancar la pestanya
    QAction *m_closeTabAction;

    /// �ndex de la p�gina actual que tractem
    int m_lastIndex;

    /// Crea les connexions de signals i slots
    void createConnections();

};

};  //  end  namespace udg

#endif
