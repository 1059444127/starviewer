/***************************************************************************
 *   Copyright (C) 2005-2006 by Grup de Gr�fics de Girona                  *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/
#ifndef UDGTOOLMANAGER_H
#define UDGTOOLMANAGER_H

#include <QObject>
#include <map> // per mapejar el conjunt de tools disponibles

namespace udg {

class Tool;

/**
Classe pare que s'ocupar� de gestionar les tools associades a un visor.

	@author Grup de Gr�fics de Girona  ( GGG ) <vismed@ima.udg.es>
*/
class ToolManager : public QObject
{
Q_OBJECT
public:
    ToolManager(QObject *parent = 0);

    ~ToolManager();

    /// Retorna la tool que li demanem si existeix
    Tool *getTool( QString toolName );

public slots:
    /// Re-envia l'event cap a la tool que necessita l'event
    void forwardEvent( unsigned long eventID );

    /// Activa la tool indicada. Si el nom donat �s correcte retorna cert, altrament fals. Ser� re-implementada per cada ToolManager espec�fic de cada visor
    bool setCurrentTool( QString toolName );

protected:
    /// Inicialitza el registre de tools disponibles que proporcioni el manager
    virtual void initToolRegistration() = 0;

    /// Mapa de tools disponibles. Cada subclasse omplir� aquest conjunt amb les tools que proporciona
    typedef std::map<QString,Tool*> ToolObjectMapType;
    ToolObjectMapType m_toolList;

    /// Nom de la tool actual que s'est� fent servir
    QString m_currentToolName;

};

}

#endif
