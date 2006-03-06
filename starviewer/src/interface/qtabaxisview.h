/***************************************************************************
 *   Copyright (C) 2005 by Grup de Gr�fics de Girona                       *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/
#ifndef UDGQTABAXISVIEW_H
#define UDGQTABAXISVIEW_H

#include <qtabaxisviewbase.h>

namespace udg {

class Volume;
/**
@author Grup de Gr�fics de Girona  ( GGG )
*/
class QTabAxisView  : public QTabAxisViewBase{
Q_OBJECT
public:
    QTabAxisView(QWidget *parent = 0, const char *name = 0);

    ~QTabAxisView();

    /**
        Assigna el volum a visualitzar
        \TODO aquest sistema �s temporal, ja que a la llarga el que si li haurien de donar �s una s�rie de recursos, com p. exe el repositori i si un cas l'id del volum a visualitzar en aquell moment
    */
    void setInput(Volume* newImageData);

};

};  //  end  namespace udg 

#endif
