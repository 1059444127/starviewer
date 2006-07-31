/***************************************************************************
 *   Copyright (C) 2005-2006 by Grup de Gr�fics de Girona                  *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/
#ifndef UDGDISPLAYABLE_H
#define UDGVISIBLEID_H

#include <QList>
#include <QString>

namespace udg {

/**
Identificador que es fa servir en llocs on es necessita un identificador que l'usuari podr� veure per pantalla. Aquest t� una part fixa, invariable, que �s la que realment es fa servir com a identificador, internament. Tamb� t� una part que serveix per poder-la mostrar a l'interf�cie i que es pugui traduir. Per exemple, els noms de les Extensions.

No t� "setters" expressament.
Exemple d'assignacions i creaci� d'id's nuls:
@code
DisplayableID id(QString::null);

DisplayableID id2("test", tr("test"));

id = DisplayableID("nouValor",tr("nouValor"));

id = id2;
@endcode

	@author Grup de Gr�fics de Girona  ( GGG ) <vismed@ima.udg.es>
*/

class DisplayableID {

public:

    DisplayableID(const QString & id, const QString & name = QString::null);

    ~DisplayableID();

    QString getID() const { return m_id; };
    QString getLabel() const { return m_name; };

    friend inline bool operator==(const DisplayableID &, const DisplayableID &);
    friend inline bool operator!=(const DisplayableID &, const DisplayableID &);
    friend inline bool operator<(const DisplayableID &, const DisplayableID &);
    friend inline bool operator>(const DisplayableID &, const DisplayableID &);

private:

    QString m_id;
    QString m_name;

};

typedef QList<DisplayableID> DisplayableIDList;

}

#endif
