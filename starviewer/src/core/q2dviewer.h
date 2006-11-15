/***************************************************************************
 *   Copyright (C) 2005 by Grup de Gr�fics de Girona                       *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/
#ifndef UDGQ2DVIEWER_H
#define UDGQ2DVIEWER_H

#include "qviewer.h"

// Fordward declarations
// qt
class QEvent;
class QAction;
// vtk
class vtkImageViewer2;
class vtkPropPicker;
class vtkTextActor;
class vtkObject;
class vtkCommand;
class vtkRenderer;
// class vtkRenderWindowInteractor;
class vtkCornerAnnotation;
class vtkAxisActor2D;
class vtkWindowToImageFilter;
class vtkCoordinate;
class vtkCaptionActor2D;
class vtkScalarBarActor;
class vtkInteractorStyleImage;

namespace udg {

/**

Classe base per als visualitzadors 2D.
El mode d'operaci� habitual ser� el de visualitar un sol volum.
Normalment per poder visualitzar un volum farem el seg�ent
\code
Q2DViewer* visor = new Q2DViewer();
visor->setInput( volum );
visor->setView( Q2DViewer::Axial );
visor->render();

En el cas que desitjem solapar dos volums haurem d'indicar el volum solapat mab el m�tode setOverlayInput().
Quan solapem volums tenim 3 maneres de solapar aquests volums, amb un patr� de checkerboard, aplicant un blending o un rectilinearWipe, en aquest cas haur�em de fer servir el m�tode setOverlay() indicant una de les opcions, Blend, CheckerBoard o RectilinearWipe
\TODO acabar la doc sobre solapament

Per defecte el visualitzador mostra la llesca central de la vista seleccionada, si volem canviar al llesca farem servir el m�tode setSlice().

Podem escollir quines annotacions textuals i de refer�ncia apareixeran en la vista 2D a trav�s dels flags "AnnotationFlags" definits com enums. Aquests flags es poden passar en el constructor o els podem modificar a trav�s dels m�todes \c addAnnotation() o \c removeAnnotation() que faran visible o invisible l'anotaci� indicada. Per defecte el flag �s \c AllAnnotation i per tant es veuen totes les anotacions per defecte.

@author Grup de Gr�fics de Girona  ( GGG )
*/

// Fordward declarations
class Volume;
class Q2DViewerToolManager;
class Tool;

class Q2DViewer  : public QViewer{
Q_OBJECT
public:
    /// Axial: XY, Coronal: XZ, Sagittal: YZ
    enum ViewType{ Axial, Coronal, Sagittal , None };

    /// tipus de fusi� dels models
    enum OverlayType{ Blend , CheckerBoard , RectilinearWipe };

    /// Aquests flags els farem servir per decidir quines anotacions seran visibles i quines no
    enum AnnotationFlags{ NoAnnotation = 0x0 , WindowInformationAnnotation = 0x2 , PatientOrientationAnnotation = 0x4 , RulersAnnotation = 0x8 , AllAnnotation = 0xE };

    Q2DViewer( QWidget *parent = 0 , unsigned int annotations = Q2DViewer::AllAnnotation );
    ~Q2DViewer();

    virtual vtkRenderer *getRenderer();
    virtual void setInput( Volume* volume );

    /// Retorna l'interactor style
    vtkInteractorStyleImage *getInteractorStyle();

    /// Li indiquem quina vista volem del volum: Axial, Coronal o Sagital
    void setView( ViewType view );
    void setViewToAxial(){ setView( Q2DViewer::Axial ); }
    void setViewToCoronal(){ setView( Q2DViewer::Coronal ); }
    void setViewToSagittal(){ setView( Q2DViewer::Sagittal ); }

    /// Actualitzaci� d'anotacions v�ries
    void updateWindowLevelAnnotation();

    /// ens retorna la vista que tenim en aquells moments del volum
    ViewType getView() const { return m_lastView; }

    /// Ens retorna l'ImageViewer
    vtkImageViewer2* getImageViewer(){ return m_viewer; }

    /// Afegim el volum solapat
    void setOverlayInput( Volume* volume );

    // M�todes espec�fics checkerboard
    /// Obtenim el nombre de divisions
    int * getDivisions();
    void getDivisions( int data[3] );

    /// Indiquem el nombre de divisions del checkerboard
    void setDivisions( int data[3] );

    /// Desa totes les llesques que es veuen al visor amb el nom de fitxer base \c baseName i en format especificat per \c extension
    void saveAll( const char *baseName , FileType extension );

    /// Desa la vista actual del visor amb el nom de fitxer base \c baseName i en format especificat per \c extension
    void saveCurrent( const char *baseName , FileType extension );

    /// Obt� el window level actual de la imatge
    double getCurrentColorWindow();
    double getCurrentColorLevel();

public slots:
    void eventHandler( vtkObject * obj, unsigned long event, void * client_data, void *call_data, vtkCommand * command );

    /// Indiquem si volem veure la informaci� del volum per pantalla \TODO realment es far� servir aquest m�tode?
    void displayInformationText( bool display );

    /// M�todes varis per mostrar o no certa informaci� en pantalla
    void displaySliceOn();
    void displaySliceOff();
    void displayProtocolNameOn();
    void displayProtocolNameOff();
    void displayWindowInformationOn();
    void displayWindowInformationOff();
    void displaySerieInformationOn();
    void displaySerieInformationOff();
    void displayRulersOn();
    void displayRulersOff();
    void displayRulersLabelsOn();
    void displayRulersLabelsOff();
    void displayPatientOrientationOn();
    void displayPatientOrientationOff();
    void displayScalarBarOn();
    void displayScalarBarOff();

    /// Habilita/deshabilita que es vegi la info de voxel
    void setVoxelInformationCaptionEnabled( bool enable );
    void enableVoxelInformationCaption();
    void disableVoxelInformationCaption();

    /// retorna la llesca actual
    int getSlice( void ){ return m_currentSlice; }

    /// canvia la llesca que veiem de la vista actual
    void setSlice(int value);

    /// indica el tipu de solapament dels volums, per defecte checkerboard
    void setOverlay( OverlayType overlay ){ m_overlay = overlay; }
    void setOverlayToBlend(){ setOverlay( Q2DViewer::Blend ); };
    void setOverlayToCheckerBoard(){ setOverlay( Q2DViewer::CheckerBoard ); };
    void setOverlayToRectilinearWipe(){ setOverlay( Q2DViewer::RectilinearWipe ); };

    /// Actualitza els valors on apunta el cursor
    void updateCursor( double x, double y , double z , double value )
    {
        m_currentCursorPosition[0] = x;
        m_currentCursorPosition[1] = y;
        m_currentCursorPosition[2] = z;
        m_currentImageValue = value;
    }

    virtual void render();

    void reset();

    // M�todes espec�fics checkerboard
    /// Indiquem el nombre de divisions del checkerboard
    void setDivisions( int x , int y , int z );

    void contextMenuRelease( vtkObject *object, unsigned long event, void *client_data, void *call_data, vtkCommand * command);

    /// Afegir o treure la visibilitat d'una anotaci� textual
    void addAnnotation( AnnotationFlags annotation )
    {
        m_enabledAnnotations = m_enabledAnnotations | annotation;
        updateAnnotations();
    }
    void removeAnnotation( AnnotationFlags annotation )
    {
        // si existeix, llavors la "restem", sin� no cal fer res
        if( m_enabledAnnotations & annotation )
        {
            m_enabledAnnotations -= annotation;
            updateAnnotations();
        }

    }

    /// Ajusta el window/level
    void setWindowLevel( double window , double level );

    /// Obt� el window/level original
    void getDefaultWindowLevel( double wl[2] );

    /// Obt� el window level actual de la imatge
    void getCurrentWindowLevel( double wl[2] );

    /// Reseteja el window level al que tingui per defecte el volum
    void resetWindowLevelToDefault();

    /// Actualitza la informaci� del voxel que hi ha per sota del cursor
    void updateVoxelInformation();

    /// Interroga al tool manager per la tool demanada. Segons si aquesta tool est� disponible o no el viewer far� el que calgui
    void setTool( QString toolName );

    /// Obt� la tool que li demanem. \TODO M�tode "temporal" (o no) per poder accedir a les dades d'una tool; per exemple, si tenim la tool de seeds, per certes extensions necessitarem obtenir el valor de la seed, i nom�s la SeedTool t� el m�tode espec�fic per accedir a aquestes dades
    Tool *getTool( QString toolName );

    void setEnableTools( bool enable );
    void enableTools();
    void disableTools();

    /// Aplica una rotaci� de 90 graus en el sentit de les agulles del rellotge
    void rotateClockWise();

    /// Aplica una rotaci� de 90 graus en el sentit contrari a les agulles del rellotge
    void rotateCounterClockWise();

protected:
    /// Connector d'events vtk i slots qt
    vtkEventQtSlotConnect *m_vtkQtConnections;

    /// Aquest m�tode es fa servir perqu� es mostri un men� contextual
    //virtual void contextMenuEvent( QContextMenuEvent *event );

    /// Acci� temporal de proves. Simplement fa un reset mostrant la llesca central de nou
    QAction *m_resetAction;

    /// per a les vistes 2D farem servir el vtkImageViewer2
    vtkImageViewer2 *m_viewer;

    /// conserva la vista actual
    ViewType m_lastView;

    /// La llesca actual que estem visualitzant
    int m_currentSlice;

    /// Aquest �s el segon volum afegit a solapar
    Volume* m_overlayVolume;

    /// El nombre de divisions per cada dimensi�
    int m_divisions[3];

    /// El picker per anotar punts de la imatge
    vtkPropPicker *m_picker;

    /// Textes informatius de l'image actor , ens estalviar� molta feina
    vtkCornerAnnotation *m_textAnnotation;

    /// Actualitza la vista en el rendering
    void updateView();

    // Processem l'event de resize de la finestra Qt
    virtual void resizeEvent( QResizeEvent* resize );

private:
    /// flag que ens indica quines anotacions es veuran per la finestra
    unsigned int m_enabledAnnotations;

    /// Refresca les anotacions que s'han de veure i les que no
    void updateAnnotations();

    /// inicialitza els strings d'informaci� que es mostraran a la pantalla
    void initInformationText();

    /// Tipu de solapament dels volums en cas que en tinguem m�s d'un
    OverlayType m_overlay;

    /// configuraci� de la interacci� amb l'usuari
    void setupInteraction();

    /// Crea i inicialitza totes les accions d'aquest widget
    void createActions();

    /// Crea i inicialitza totes les anotacions que apareixeran per pantalla
    void createAnnotations();

    /// crea els indicadors d'escala
    void createRulers();

    /// Actualitza els rulers
    void updateRulers();

    /// Crea la barra de valors
    void createScalarBar();

    /// Actualitza la barra de valors
    void updateScalarBar();

    /// crea els actors necessaris per mostrar la llegenda flotant amb la informaci� de voxel
    void createVoxelInformationCaption();

    /// crea les anotacions de l'orientaci� del pacient
    void createOrientationAnnotations();

    /// Afegeix tots els actors a l'escena
    void addActors();

    /// Els strings amb els textes de cada part de la imatge
    QString m_lowerLeftText, m_lowerRightText, m_upperLeftText, m_upperRightText;

    /// Aquest string indica les anotacions que ens donen les refer�ncies del pacient ( Right,Left,Posterior,Anterior,Inferior,Superior)
    QString m_patientOrientationText[4];

    /// A partir de l'string d'orientaci� del pacient mapeja les anotacions correctes segons com estem mirant el model. A aix� li afecta tamb� si la vista �s axial, sagital o coronal
    void mapOrientationStringToAnnotation();

    /// Marcadors que indicaran les mides relatives del model en les dimensions x,y i z ( ample , al�ada i profunditat ). Al ser visor 2D en veurem nom�s dues. Aquestes variaran en funci� de la vista en la que ens trobem.
    vtkAxisActor2D *m_sideRuler , *m_bottomRuler;

    /// Coordenades fixes dels rulers que els ajustaran a un dels extrems inferiors/superiors o laterals de la pantalla
    vtkCoordinate *m_anchoredRulerCoordinates;

    /// coordenades dels extrems per cada dimensi� del ruler
    double m_rulerExtent[6];

    /// Textes adicionals d'anotoaci�
    vtkTextActor *m_patientOrientationTextActor[4];

    /// Actualitzaci� d'anotacions v�ries
    void updateSliceAnnotation();
    void updateWindowInformationAnnotation();
    void updateSerieInformationAnnotation();
    void updateProtocolNameAnnotation();

    /// Mides (x,y) de la imatge que mostrarem com informaci� adicional
    int m_imageSizeInformation[2];

    /// Valors dels window level per defecte. Pot venir donat pel DICOM o assignat per nosaltres a un valor est�ndar de constrast
    double m_defaultWindow, m_defaultLevel;

    /// Llegenda que segueix el cursor amb el valor del voxel
    vtkCaptionActor2D *m_voxelInformationCaption;

    /// Barra que mostra l'escala de colors del model que estem visualitzant \TODO quan tinguem models fusionats tindrem una o dues barres d'escala de colors?
    vtkScalarBarActor *m_scalarBar;

    /// El manager de les tools
    Q2DViewerToolManager *m_toolManager;

    /// per controlar si la info de voxel est� habilitada o no
    bool m_voxelInformationEnabled;

    /// Factor de rotaci�. En sentit de les agulles del rellotge 0: 0�, 1: 90�, 2: 180�, 3: 270�.
    int m_rotateFactor;

private slots:
    /// Actualitza la rotaci� de la c�mera \sa rotateClockWise() i rotateCounterClockWise()
    void updateCameraRotation();

signals:
    /// envia la nova llesca en la que ens trobem
    void sliceChanged(int);

    /// indica el nou window level
    void windowLevelChanged( double window , double level );
};

};  //  end  namespace udg

#endif
