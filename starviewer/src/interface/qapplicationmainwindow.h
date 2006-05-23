/***************************************************************************
 *   Copyright (C) 2005 by Grup de Gr�fics de Girona                       *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// QT
#include <QMainWindow>
#include <QStringList>
#include <QMenu>

// Forward declarations
class QAction;
class QToolBar;
class QProgressDialog;
class QCloseEvent;

namespace udg{
// Forward declarations
class ExtensionWorkspace;
class ExtensionHandler;

class QApplicationMainWindow : public QMainWindow
{
Q_OBJECT
public:
    ///El constructor del Mainwindow. Aqu� es crea tot l'esquelet de l'interf�cie: Men�s, Barres d'eines, toolbox, working area, etc..
    QApplicationMainWindow(QWidget *parent = 0, const char *name = 0);
    
    ~QApplicationMainWindow();
    
    /// L'�rea de mini-aplicacions
    ExtensionWorkspace *m_extensionWorkspace;

    /// M�tode de conveni�ncia per sortir del pas de mentres. Es crida quan tenim ja un model obert i volem obri-ne un de nou
    void newAndOpen();
    void newAndOpenDir();
    
    /// Neteja la barra d'eines d'extensions
    void clearExtensionsToolBar();

    /// Retorna la barra d'eines d'extensions
    QToolBar *getExtensionsToolBar() const { return m_extensionsToolBar; }
    
public slots:
    /// Fa les tasques que cal quan s'ha carregat un volum
    void onVolumeLoaded();
    
protected:
    /// Aquest event ocurreix quanes tanca la finestra. �s el moment en que es realitzen algunes tasques com desar la configuraci�
    void closeEvent(QCloseEvent *event);

private:
    /// Gestor de serveis i miniaplicacions
    ExtensionHandler *m_extensionHandler;
    
    /// Tipus de fitxer a exportar
    enum ExportFiles{ JpegExport , MetaIOExport , TiffExport, PngExport , BmpExport };

//    /// Llista d'arxius recents
//     QStringList m_recentFiles;
//     enum { MaxRecentFiles = 5 };
//     QAction *m_recentFileActions[ MaxRecentFiles ];

    /// Arxiu actual que estem visualitzant
    QString m_currentFile;

    /// Filtres per als di�legs d'obrir/exportar arxius
//     QString m_exportFileFilters;
//     QString m_exportToJpegFilter;
//     QString m_exportToMetaIOFilter;
//     QString m_exportToTiffFilter;
//     QString m_exportToBmpFilter;
//     QString m_exportToPngFilter;
//     QString m_exportToDicomFilter;

    /// Path actual del directori de treball ( obrir fitxer )
    QString m_workingDirectory;

    ///Path del directori de treball ( exportaci� a altres formats )
    QString m_exportWorkingDirectory;

    /// Aquesta string indica el path del directori que cont� els arxius de traducci� de l'aplicaci�
    QString m_qmPath;

    /// Men�s
    QMenu *m_fileMenu;
    QMenu *m_visualizationMenu;
    QMenu *m_languageMenu;
    QMenu *m_helpMenu;
    
    /// Sub men�s
//     QMenu *m_importFilesMenu;
//     QMenu *m_exportFilesMenu;
//     QMenu *m_recentFilesMenu;
    
    /// Barres d'eines 
    QToolBar *m_extensionsToolBar;
    QToolBar *m_fileToolBar;
    
    /// Accions
    QAction *m_defaultViewerAction;
    QAction *m_mpr2DAction;
    QAction *m_mpr3DAction;
    QAction *m_mpr3D2DAction;
    QAction *m_pacsAction;
    QAction *m_newAction;
    QAction *m_openAction;
    QAction *m_openDirAction;
    QAction *m_closeAction;
    QAction *m_exitAction;
    QAction *m_aboutAction;
//     QAction *m_exportToJpegAction;
//     QAction *m_exportToMetaIOAction;
//     QAction *m_exportToTiffAction;
//     QAction *m_exportToPngAction;
//     QAction *m_exportToBmpAction;

    QAction *m_catalanAction;
    QAction *m_spanishAction;
    QAction *m_englishAction;

    /// Per indicar el proc�s al obrir-se un fitxer
    QProgressDialog *m_progressDialog;

    // :::::::::::::::::::::::::::
    // m�todes
    // :::::::::::::::::::::::::::
    
    ///Crea i inicialitza les accions de l'aplicaci�
    void createActions();

    /// Crea els men�s.
    void createMenus();

    /// Crea el men� per escollir l'idioma de l'aplicaci�
    void createLanguageMenu();
    
    /// Crea la barra d'eines.
    void createToolBars();

    /// Crea la barra d'estat de l'aplicaci�
    void createStatusBar();

    /// Llegeix la configuraci� inicial amb la que engega el programa
    void readSettings();

    /// Escriu la configuraci� amb la que s'engegar� el programa el pr�xim cop. \TODO: s'hauria de cridar tamb� quan obrim una finestra nova?
    void writeSettings();

    /// Retorna el numero de finestres amb diferents models que tenim obertes, el num. de QApplicationMainWindow
    unsigned int getCountQApplicationMainWindow();
/*
    /// Especifica el fitxer acctual que s'est� tractant \TODO \deprecated aquest m�tode esdevindr� antiquat en breu i no t� gaire sentit desar el nom del fitxer, el metode continua existint perqu� s'ha arrastrat des del principi
    void setCurrentFile( const QString &fileName );

    /// actualitza el llistat de noms de fitxers oberts recentment \TODO s'hauria d'adaptar, si cal quan hi hagi lo del pacs
    void updateRecentFileActions();
*/
private slots:
    /// Obre una nova finestra
    void newFile();

    /// Slot tancar volum
    void close();

    /// mostra el formulari d'about
    void about();

    /// canvia a l'idioma indicat
    void switchToLanguage( int id );
    
/*
    /// obre un dels arxius recents::\TODO aix� pot esdevenir \deprecated
    void openRecentFile();

    /// Exporta la imatge a un format especificat
    void exportFile( int type );

    /// Exporta la imatge a JPEG
    void exportToJpeg();

    /// Exporta la imatge a MetaIO
    void exportToMetaIO();

    /// Exporta la imatge a TIFF
    void exportToTiff();

    /// Exporta la imatge a PNG
    void exportToPng();

    /// Exporta la imatge a BMP
    void exportToBmp();
*/
signals:
    /// senyal emesa que indica si la finestra cont� un volum o no
    void containsVolume( bool );
};

}; // fi namespace udg

#endif
