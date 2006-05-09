/***************************************************************************
 *   Copyright (C) 2005 by Grup de Gr�fics de Girona                       *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/ 
#ifndef UDGQCONFIGURATIONSCREEN_H
#define UDGQCONFIGURATIONSCREEN_H

#include "ui_qconfigurationscreenbase.h"

using namespace Ui; // \TODO aix� s'hauria d'evitar!

namespace udg {

class Status;
/** Interf�cie que permet configurar els par�metres del pacs i de la cach�
@author marc
*/
class QConfigurationScreen : public QDialog , private QConfigurationScreenBase
{
Q_OBJECT

public:
    
    /// Constructor de la classe
    QConfigurationScreen( QWidget *parent = 0 );

    ///Destructor de classe
    ~QConfigurationScreen();
     
public slots :

    /// Neteja els line edit de la pantalla
    void clear();
    
    /// Slot que dona d'alta el PACS a la la base de dades
    void addPacs();
    
    /// Slot que s'activa quant seleccionem un Pacs del PacsListView, emplena les caixes de texts amb les dades del Pacs
    void selectedPacs( QTreeWidgetItem * item , int );
    
    /// Slot que updata les dades d'un pacs
    void updatePacs();
    
    /// Slot que esborra el pacs seleccionat
    void deletePacs();
    
    /// Fa un echo a un pacs seleccionat per saber si aquest est� viu
    void test();
    
    /// Mostra un QDialog per especificar on es troba la base de dades de la cach�
    void examinateDataBaseRoot();
    
    /// Mostra un QDialog per especificar on s'han de guardar les imatges descarregades
    void examinateCacheImagePath();

    /// Esborra tota la cach�
    void deleteStudies();
    
    /// Compacta la base de dades de la cache
    void compactCache();
    
    /// Aplica els canvis de la configuraci�
    void applyChanges();
    
    /// Guarda els canvis a la configuraci� dels par�metres del PACS
    void acceptChanges();
    
    /// Tanca la pantalla de configuraci�, i desprecia els canvis
    void cancelChanges();

    /// Slot que s'utilitza quant es fa algun canvi a la configuraci�, per activar els buttons ap
    void configurationChanged( const QString& );
    
    /// Afegeix la '/' al final del path del directori si l'usuari no l'ha escrit
    void cacheImagePathEditingFinish();
       
signals :
    
    ///signal que s'emet quan hi ha algun canvi a la llista de PACS, per a que la QPacsList es pugui refrescar
    void pacsListChanged();
    
    ///signal que s'emet quan la cache ha estat netejada cap a QueryScreen, pq netegi el QStudyTreeView que mostra els estudis de la cache
    void cacheCleared();

private :

    int m_PacsID; ///<ID del pacs seleccionat
    bool m_configurationChanged; ///<Indica si la configuraci� ha canviat

    ///crea els connects dels signals i slots
    void createConnections();
    
    /** Comprovem que els par�metres dels PACS siguin correctes. 
     *  1r Que el AETitle no estigui en blanc,
     *  2n Que l'adre�a del PACS no estigui en blanc,
     *  3r Que el Port del Pacs sigui entre 0 i 65535
     *  4t Que l'instituci� no estigui buida 
     * @return bool, retorna cert si tots els parametres del pacs son correctes
     */
    bool validatePacsParameters();
    
    /** Valida que els canvis de la configuraci� siguin correctes
     *  Port local entre 0 i 65535
     *  Numero m�xim de connexions 25
     *  Path de la base de dades i directori dicom's existeix
     *  @return indica si els canvis son correctes
     */
    bool validateChanges();
    
    /// Emplena el ListView amb les dades dels PACS que tenim guardades a la bd
    void fillPacsListView();

    /** Tracta els errors que s'han produ�t a la base de dades en general
     *           @param state  Estat del m�tode
     */
    void databaseError(Status * state);
    
    /// Carrega les dades de configuraci� de la cache
    void loadCacheDefaults();
    
    /// Emplena els textboxs amb les dades del PACS
    void loadPacsDefaults();
    
    /// calcula les dades del pool
    void loadCachePoolDefaults();
    
    /// Guarda els canvis a la configuraci� dels par�metres del PACS
    void applyChangesPacs();
    
    ///  Aplica els canvis fets a la configuraci� de la cache
    void applyChangesCache();
};

};// end namespace udg

#endif
