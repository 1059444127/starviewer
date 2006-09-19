/***************************************************************************
 *   Copyright (C) 2005 by Grup de Gr�fics de Girona                       *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/
#ifndef UDGQMPRVIEWER_H
#define UDGQMPRVIEWER_H

#include "ui_qmprextensionbase.h"
using namespace Ui; // \TODO sembla que l'obligaci� de posar aix� perqu� funcioni �s culpa dels FWD decls que hi ha fora de udg, en els que no hi ha FWD decls no cal
#include <QString>

// FWD declarations
class vtkAxisActor2D;
class vtkPlaneSource;
class vtkImageReslice;
class vtkTransform;
class vtkActor2D;
class QAction;

namespace udg {

// FWD declarations
class Volume;
class Q3DViewer;
/**
Extensi� encarregada de fer l'MPR 2D
    
@author Grup de Gr�fics de Girona  ( GGG )

\TODO Afegir l'interacci� de l'usuari. Per rotar i traslladar els plans podem seguir com a model el que fan els m�todes vtkImagePlaneWidget::Spin() i vtkImagePlaneWidget::Translate()
*/
class QMPRExtension : public QWidget , private QMPRExtensionBase /*Ui::QMPRExtensionBase*/{
Q_OBJECT
public:
    QMPRExtension( QWidget *parent = 0 );

    ~QMPRExtension();
    /**
        Li assigna el volum amb el que s'aplica l'MPR.
        A cada finestra es veur� el tall corresponent per defecte. De bon principi cada visor visualitzar� la llesca central corresponent a cada vista. En les vistes axial i sagital es veuran els respectius plans de tall.
    */
    void setInput( Volume *input );
        
    /// Rota els graus definits per 'angle' sobre l'eix i punt de rotaci� que defineixen la intersecci� de dos plans. EL primer pla es el que volem rotar i l'altre sobre el qual estroba l'eix d'intersecci�/rotaci�
    // caldria resoldre qu� fer quan els plans son coplanars!!!!
    void rotate( double degrees , double rotationAxis[3] ,  vtkPlaneSource* plane );
    
    /// Rota el pla especificat pel seu centre
    void rotateMiddle( double degrees , double rotationAxis[3] ,  vtkPlaneSource* plane );

public slots:
    /// Canvia la distribuci� horitzontal de les finestres ( el que est� a la dreta passa a l'esquerra i viceversa )
    void switchHorizontalLayout();

    /// Activa o desactiva la distribuci� de finestres al mode MIP
    void switchToMIPLayout( bool isMIPChecked );
    
signals:
    /// Notificaci� del canvi de direcci� de cadascun dels eixos que podem manipular. Aquests senyals haurien de ser enviats quan canviem la direcci� a trav�s dels controls ( l�nies blaves i vermella)
    void coronalXAxisChanged( double x1 , double x2 , double x3 );
    void coronalZAxisChanged( double z1 , double z2 , double z3 );
    void sagitalYAxisChanged( double y1 , double y2 , double y3 );
    
private:
    /// Ens diu si un eix �s paral�lel a un dels aixos de coordenades X Y o Z
    bool isParallel( double axis[3] );

    /// Calcula l'angle entre dos vectors. Retorna el valor en radians
    double angleInRadians( double vec1[3] , double vec2[3] );

    /// Calcula l'angle entre dos vectors. Retorna el valor en graus
    double angleInDegrees( double vec1[3] , double vec2[3] );

    /// Obtenim els vectors que defineixen els eixos de cada pla ( sagital , coronal )
    void getSagitalXVector( double x[3] );
    void getSagitalYVector( double y[3] );
    void getCoronalXVector( double x[3] );
    void getCoronalYVector( double y[3] );
    void getAxialXVector( double x[3] );
    void getAxialYVector( double y[3] );

    /// Actualitza valors dels plans i del reslice final \TODO: separar en dos m�todes diferenciats segons quin pla????
    void updatePlanes();

    /// Actualitza els valors del pla donat amb el reslice associat
    void updatePlane( vtkPlaneSource *planeSource , vtkImageReslice *reslice );

    /// Actualitza el punt d'intersecci� dels 3 plans
    void updateIntersectionPoint();
    
    /// inicialitza les orientacions dels plans de tall correctament perqu� tinguin un espaiat, dimensions i l�mits correctes
    void initOrientation();

    /// El reslice de cada vista
    vtkImageReslice *m_sagitalReslice, *m_coronalReslice;

    /// La tranformaci� que apliquem
    vtkTransform *m_transform;

    /// El volum al que se li practica l'MPR
    Volume *m_volume;

    /// Els actors que representen els eixos que podrem modificar. L�nia vermella, blava (axial), blava (sagital) respectivament i el thickSlab ( l�nies puntejades blaves en vista axial i sagital ).
    vtkAxisActor2D *m_sagitalOverAxialAxisActor, *m_axialOverSagitalIntersectionAxis, *m_coronalOverAxialIntersectionAxis , *m_coronalOverSagitalIntersectionAxis, *m_thickSlabOverAxialActor , *m_thickSlabOverSagitalActor;
    
    /// Ens ser� molt �til ens molts de c�lculs i a m�s ser� una dada constant un cop tenim l'input
    double m_axialSpacing[3];
    
    /// Punt d'intersecci� entre els 3 plans
    double m_intersectionPoint[3];

    /// Llegir/Escriure la configuraci� de l'aplicaci�
    void readSettings();
    void writeSettings();

    /// Torna a pintar els controls per modificar els plans
    void updateControls(); 

    /// Crea els actors i els incicialitza
    void createActors();

    /// Crea les accions 
    void createActions();
    
    /// crea les connexions entre signals i slots
    void createConnections();
    
    /// Els plans de tall per cada vista ( m�s el thickSlab )
    vtkPlaneSource *m_sagitalPlaneSource, *m_coronalPlaneSource , *m_axialPlaneSource , *m_thickSlabPlaneSource;
    
    /// ens retorna la l�nia d'intersecci� entre dos plans definida per un punt i un vector     
    void planeIntersection( vtkPlaneSource* plane1 , vtkPlaneSource *plane2 , double r[3] , double t[3] );

    /// Calcula el punt d'intersecci� de 3 plans a l'espai 
    void planeIntersection( vtkPlaneSource *plane1 , vtkPlaneSource *plane2 , vtkPlaneSource *plane3 , double intersectionPoint[3] );

    /// Ens d�na l'eix de rotaci� d'un planeSource
    void getRotationAxis( vtkPlaneSource *plane , double axis[3] );
    
    /// El directori on es desaran les imatges per defecte
    QString m_defaultSaveDir;

    /// Filtre de fitxers que es poden desar
    QString m_fileSaveFilter;

    /// Cosetes per controlar el moviment del plans a partir de l'interacci� de l'usuari
    double m_initialPickX , m_initialPickY;
    vtkPlaneSource *m_pickedActorPlaneSource;
    vtkImageReslice *m_pickedActorReslice;

    /// Gruix del thickSlab que servir� per al MIP
    double m_thickSlab;

    /// Acci� per poder controlar el layout horizontal
    QAction *m_horizontalLayoutAction;
    QAction *m_mipAction;

    /// Visor de MIP
    Q3DViewer *m_mipViewer;
    
private slots:
    /// S'encarreguen de rotar els eixos dels plans
    void detectAxialViewAxisActor( double x , double y );
    void releaseAxialViewAxisActor( double x , double y );
    
    void detectSagitalViewAxisActor( double x , double y );
    void releaseSagitalViewAxisActor( double x , double y );

    void moveAxialViewAxisActor( double x , double y );
    void rotateAxisActor( double x , double y );

    /// s'encarreguen de moure les llesques dels plans
    void detectPushAxialViewAxisActor( double x , double y );
    void releasePushAxialViewAxisActor( double x , double y );

    void detectPushSagitalViewAxisActor( double x , double y );
    void releasePushSagitalViewAxisActor( double x , double y );

    void pushAxisActor( double x , double y );
    void pushAxialActor( double x , double y );
    
    /// Fa les accions pertinents quan una llesca s'ha actualitzat
    void axialSliceUpdated( int slice );

    /// Fa el proc�s de guardar les imatges capturades
    void saveImages();

    /// Actualitza el valor del thickSlab i tot el que hi estigui relacionat amb ell
    void updateThickSlab( double value );
};

};  //  end  namespace udg 

#endif
