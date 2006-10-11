/***************************************************************************
 *   Copyright (C) 2005 by Grup de Gr�fics de Girona                       *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/
#ifndef UDGQ2DVIEWEREXTENSION_H
#define UDGQ2DVIEWEREXTENSION_H

#include "ui_q2dviewerextensionbase.h"

// FWD declarations
class QAction;

namespace udg {

// FWD declarations
class Volume;
class ToolsActionFactory;

/**
Extensi� que s'executar� per defecte a l'obrir un model

	@author Grup de Gr�fics de Girona  ( GGG ) <vismed@ima.udg.es>
*/
class Q2DViewerExtension : public QWidget , private Ui::Q2DViewerExtensionBase {
Q_OBJECT
public:
    Q2DViewerExtension( QWidget *parent = 0 );

    ~Q2DViewerExtension();

    /// Li assigna el volum principal
    void setInput( Volume *input );
    
public slots:
    /// Canvia a la vista axial, sagital o coronal
    void changeViewToAxial();
    void changeViewToSagital();
    void changeViewToCoronal();

    /// Canvia el layout de visor singular/doble
    void changeViewToSingle();
    void changeViewToDouble();

    /// Li assigna el volum secundari. Aquest m�tode nom�s �s de conveni�ncia i �s temporal
    void setSecondInput( Volume *input );
    
private:
    /// Tipus de vistes que podem tenir
    enum ViewType{ Axial , Sagital , Coronal };

    /// canvia la vista actual
    void setView( ViewType view );
    
    /// La vista actual amb la que estem treballant
    ViewType m_currentView;
    
    /// El volum principal
    Volume *m_mainVolume;

    /// Membre temporal
    Volume *m_secondaryVolume;

    /// Accions
    QAction *m_axialViewAction;
    QAction *m_sagitalViewAction;
    QAction *m_coronalViewAction;
    QAction *m_singleViewAction;
    QAction *m_doubleViewAction;
    QAction *m_slicingAction;
    QAction *m_windowLevelAction;
    QAction *m_zoomAction;
    QAction *m_moveAction;
    ToolsActionFactory *m_actionFactory;

    /// crea les accions \TODO 'pujar' al pare com a m�tode virtual com� a Extensions? [hauria de ser protected]
    void createActions();
    
    /// Crea les connexions entre signals i slots
    void createConnections();

    /// El di�leg per escollir un window level ajustat per l'usuari
    QCustomWindowLevelDialog *m_customWindowLevelDialog;
    
private slots:
    /// Actua quan es canvia de p�gina, �s a dir es passa d'un determinat layout a un de simple, doble , triple , etc
    void pageChange( int index );

    /// sincronitza les llesques de les s�ries que es visualitzen
    void synchronizeSlices( bool ok );

    /// ens permet escollir una nova s�rie per a comparar
    void chooseNewSerie();

signals:
    /// Aquest senyal s'emetr� quan es vulgui canviar de s�rie per comparar
    void newSerie();
};

} // end namespace udg

#endif
