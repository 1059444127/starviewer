/***************************************************************************
 *   Copyright (C) 2005-2006 by Grup de Gr�fics de Girona                  *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/
#ifndef UDGQ3DORIENTATIONMARKER_H
#define UDGQ3DORIENTATIONMARKER_H

#include <QObject>

class vtkOrientationMarkerWidget;
class vtkRenderWindowInteractor;

namespace udg {

/**
Classe que encapsula un vtkOrienatationMarker que servir� d'element d'orientaci� per a visors 3D. Per funcionar nom�s cal crear una inst�ncia de la classe i indicar-li el vtkInteractor. La fem heretar de QObjecte per si cal connectar-li signals i slots des d'una altre classe

	@author Grup de Gr�fics de Girona  ( GGG ) <vismed@ima.udg.es>
*/
class Q3DOrientationMarker: public QObject
{
Q_OBJECT
public:
    Q3DOrientationMarker( vtkRenderWindowInteractor *interactor, QObject *parent = 0);

    ~Q3DOrientationMarker();

public slots:
    /// M�todes per controlar si s'habilita el widget o no
    void setEnabled( bool enable );
    void enable();
    void disable();
    
private:
    /// Widget per veure la orientaci� en 3D
    vtkOrientationMarkerWidget *m_markerWidget;
    
};

}

#endif
