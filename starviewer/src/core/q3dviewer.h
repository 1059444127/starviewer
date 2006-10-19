/***************************************************************************
 *   Copyright (C) 2005 by Grup de Gr�fics de Girona                       *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/
#ifndef UDGQ3DVIEWER_H
#define UDGQ3DVIEWER_H

#include "qviewer.h"

// FWD declarations
class vtkRenderer;
class vtkImageCast;
class vtkRenderWindowInteractor;
class vtkInteractorStyle;

namespace udg {

/**
Classe base per als visualitzadors 3D

@author Grup de Gr�fics de Girona  ( GGG )
*/

// FWD declarations
class Volume;
class Q3DOrientationMarker;
class Q3DViewerToolManager;

class Q3DViewer : public QViewer{
Q_OBJECT
public:
    enum RenderFunction{ RayCasting , MIP3D, IsoSurface , Texture2D , Texture3D };
    
    Q3DViewer( QWidget *parent = 0 );
    ~Q3DViewer();
    
    /// retorna el tipu de visualitzaci� que es t� assignat
    RenderFunction getRenderFunction() const { return m_renderFunction; }

    /// retorna el tipu de visualitzaci� que es t� assignat com a un string
    QString getRenderFunctionAsString();
    
    virtual void setInput( Volume* volume );

    virtual vtkRenderer *getRenderer();

    /// Retorna el vtkInteractorStyle que t� associat
    vtkInteractorStyle *getInteractorStyle();
    
public slots:

    virtual void render();
    /// assignem el tipus de visualitzaci� 3D que volem. RayCasting, MIP, reconstrucci� de superf�cies...
    void setRenderFunction(RenderFunction function){ m_renderFunction = function; };
    void setRenderFunctionToRayCasting(){ m_renderFunction = RayCasting; };
    void setRenderFunctionToMIP3D(){ m_renderFunction = MIP3D; };
    void setRenderFunctionToIsoSurface(){ m_renderFunction = IsoSurface; };
    void setRenderFunctionToTexture2D(){ m_renderFunction = Texture2D; };
    void setRenderFunctionToTexture3D(){ m_renderFunction = Texture3D; };
    
    /// Reinicia la vista
    void resetViewToAxial();
    void resetViewToSagital();
    void resetViewToCoronal();

    /// m�todes per controlar la visibilitat de l'orientation marker widget
    void enableOrientationMarker( bool enable );
    void orientationMarkerOn();
    void orientationMarkerOff();

    void setEnableTools( bool enable );
    void enableTools();
    void disableTools();
    
    /// Interroga al tool manager per la tool demanada. Segons si aquesta tool est� disponible o no el viewer far� el que calgui
    void setTool( QString toolName );
    
protected:
    /// el renderer
    vtkRenderer* m_renderer;
    /// la funci� que es fa servir pel rendering
    RenderFunction m_renderFunction;

private:
    /// fa la visualitzaci� per raycasting
    void renderRayCasting();

    /// fa la visualitzaci� per MIP3D
    void renderMIP3D();

    /// fa la visualitzaci� per reconstrucci� de superf�ces
    void renderIsoSurface();

    /// fa la visualitzaci� per textures 2D \TODO afegir comprovaci� de si el hard o suporta o no
    void renderTexture2D();
    
    /// fa la visualitzaci� per textures 3D \TODO afegir comprovaci� de si el hard o suporta o no
    void renderTexture3D();
    
    /// rescala les dades en el format adequat per als corresponents algorismes. Retorna fals si no hi ha cap volum assignat
    bool rescale();

    enum { Axial , Sagital , Coronal };
    /// Canvia la orientaci� de la c�mera
    void setCameraOrientation( int orientation );
    
    /// el caster de les imatges
    vtkImageCast* m_imageCaster;

    /// Orientaci� que tenim
    int m_currentOrientation;

    /// Widget per veure la orientaci� en 3D
    Q3DOrientationMarker *m_orientationMarker;
    
    /// reinicia la orientaci�
    void resetOrientation();

    /// El manager de les tools
    Q3DViewerToolManager *m_toolManager;
};

};  //  end  namespace udg 

#endif
