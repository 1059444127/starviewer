/***************************************************************************
 *   Copyright (C) 2005 by Grup de Gr�fics de Girona                       *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/
#ifndef UDGQVOLUMEREPOSITORYCOMBOBOX_H
#define UDGQVOLUMEREPOSITORYCOMBOBOX_H

#include <qcombobox.h>
#include <qstring.h>
#include <qmap.h>
#include "identifier.h"

namespace udg {

// Fwd Declarations
class VolumeRepository;

/**
ComboBox espec�fic per al repositori de volums. Mostrar� una llista dels volums que hi ha actualment al repositori, mostrant com a informaci� d'aquests un string que els caracteritzi ( nom de la s�rie p.ex. ). A aquest valor tindrem associat el valor de l'id del volum en el repositori.

Hem agafat com exemple d'her�ncia de combobox l'exemple del llibre de Qt "C++ GUI Programming with Qt 3", Cap�tol 12, p�gs 280,281

\TODO Aquest combo box, al estar lligat al repositori de volums que �s un singleton, podria ser tamb� un altre singleton, o si no, una alternativa seria que aquest contingu�s un punter al repositori ( simplement declarnt una inst�ncia en far�em prou, llavors amb els signals que emet�s el repositori el combo s'actualitzaria sol )

De moment ens decantem per la opci� de fer servir aqu� una inst�ncia del repositori

\TODO �s possible que els m�todes que ara s�n p�blics passin a privats i que nom�s e spemeti omplir-se a trav�s de signals i slots amb el repositori

@author Grup de Gr�fics de Girona  ( GGG )
*/
class QVolumeRepositoryComboBox  : public QComboBox{
Q_OBJECT
//Q_PROPERTY( Identifier m_volumeID READ getVolumeID WRITE setVolumeID )
public:
    QVolumeRepositoryComboBox(QWidget *parent = 0, const char *name = 0);

    ~QVolumeRepositoryComboBox();

    /// Obtenim l'ID del volum actual
    Identifier getVolumeID( ) const;
    /// Indiquem l'actual volum a partir del seu ID
    void setVolumeID( Identifier id );
    /// Afegir un nou volum a la llista [Nom, id del repositori, index opcional]
    void addItem( const QString &name, Identifier id /*, int index = -1 */);
    /// Esborrem un volum de la llista a trav�s de l'identificador
    void deleteItem( Identifier id );
private:
    // Els QMap ens serveixen per mapejar els id de int a Identifier i viceversa en el combo
    QMap< int , Identifier > m_idFromIndex;
    QMap< Identifier , int > m_indexFromID;
    // La inst�ncia del repositori
    VolumeRepository* m_volumeRepository;

    /// remapeja els id i els identificadors segons el contingut del repositori
    void updateMapping();
private slots:
    // Aquest slot estar� connectat al signal del repositori quan s'afegeixi un nou volum
    void insertVolume( Identifier id );
    // Aquest slot estar� connectat al signal del repositori quan s'esborri un nou volum
    void removeVolume( Identifier id );
};

};  //  end  namespace udg 

#endif
