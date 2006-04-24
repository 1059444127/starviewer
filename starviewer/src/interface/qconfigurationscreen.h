/***************************************************************************
 *   Copyright (C) 2005 by Grup de Gr�fics de Girona                       *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/ 
#ifndef UDGQCONFIGURATIONSCREEN_H
#define UDGQCONFIGURATIONSCREEN_H

#include "ui_qconfigurationscreenbase.h"
#include "pacslistdb.h"
using namespace Ui; // \TODO aix� s'hauria d'evitar!

namespace udg {

/** Interf�cie que permet configurar els par�metres del pacs i de la cach�
@author marc
*/
class QConfigurationScreen : public QDialog , private QConfigurationScreenBase{
Q_OBJECT


public:
    QConfigurationScreen( QWidget *parent = 0 );

    ~QConfigurationScreen();
     
public slots :

    /** Configuraci� de la llista de Pacs */
    void clear();
    void addPacs();
    void selectedPacs( QTreeWidgetItem * item, int);
    void updatePacs();
    void deletePacs();
    void test();
    
    /*Configuraci� de la cach�*/
    void examinateDataBaseRoot();
    void examinateCacheImagePath();

    void deleteStudies();
    void compactCache();
    
    /*Configuraci� dels par�metres del Pacs */
    void applyChanges();
    void acceptChanges();
    void cancelChanges();

    void configurationChanged( const QString& );
    
    void cacheImagePathEditingFinish();
       
signals :

    void pacsListChanged();
    void cacheCleared();

private :

    int m_PacsID;
    bool m_configurationChanged;

    void connectSignalAndSlots();
    bool validatePacsParameters();
    bool validateChanges();
    void fillPacsListView();

    void databaseError(Status *);
    
    void loadCacheDefaults();
    void loadPacsDefaults();
    void loadCachePoolDefaults();
    
    void applyChangesPacs();
    void applyChangesCache();

};

};

#endif
