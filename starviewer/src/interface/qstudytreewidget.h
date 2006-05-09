
/***************************************************************************
 *   Copyright (C) 2005 by marc                                            *
 *   marc@localhost                                                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef UDGQSTUDYTREEWIDGET_H
#define UDGQSTUDYTREEWIDGET_H

#include "ui_qstudytreewidgetbase.h"
#include <QMenu>

namespace udg {

/** Aquesta classe  mostrar estudis i s�ries d'una manera organitzada i f�cilment.
  * Aquesta classe �s una modificaci� de la QTreeWidget que s'ha adaptat per poder visualitzar la informaci� de la cerca d'estudis, permetent consultar les series d'aquell estudi. Aquesta classe es sincronitzar amb la informaci� mostrada a QSeriesListWidget
@author marc
*/

class StudyList;
class Series;
class Study;

class QStudyTreeWidget : public QWidget , private Ui::QStudyTreeWidgetBase
{
Q_OBJECT
public:
    
    /// Constructor de la classe
    QStudyTreeWidget( QWidget *parent = 0 );

    /** Mostra l'estudi pel ListWidget que se li passa per par�metre
     *  @param StudyList a mostrar
     */
    void insertStudyList( StudyList * studyList );
    
    /**Insereix una serie d'un estudi, i emiteix un signal al QSeriesListWidget per a insereixi tamb� la informaci� de la s�rie
     *                    @param informaci� de la serie
     */
    void insertSeries( Series *serie );
    
    /** removes study from the list
     * @param esbora l'estudi amb StudyUID de la llista
     */
    void removeStudy( QString StudyUID );
 
    /** Ordena per columna
     * @param Indica la columna per la que s'ha d'ordenar
     */
    void setSortColumn(int);
    
    /** Retorna el AETitle del PACS  l'estudi seleccionat
     * @return AETitle del PACS de l'estudi seleccionat
     */
    QString getSelectedStudyPacsAETitle();
    
    /** Retorna el UID Study de l'estudi seleccionat
     * @return UID de l'estudi seleccionat
     */
    QString getSelectedStudyUID();
    
    /** Retorna el UID Study de la s�rie seleccionada, si en aquell moment no hi ha cap s�rie seleccionada, retorna un QString buit
     *  @return UID de la s�rie seleccionat
     */
    QString getSelectedSeriesUID();

    /// guarda de la mida de les columnes
    void saveColumnsWidth();
    
    /// Destructor de la classe
    ~QStudyTreeWidget();
    
protected:

    /** Mostra el menu contextual
     *  @param Dades de l'event sol�licitat
     */
    void contextMenuEvent(QContextMenuEvent *event);

signals :
    ///que s'activa al fer click sobre un estudi, demanem a la queryScreen que busqui la informacio de les series d'aquell estudi
    void expand( QString , QString );
    
    ///signal que s'emet quan es vol descarregar l'estudi seleccionat a la QStudyTreeView
    void retrieve();
    
    ///signal que s'emet quan es vol esborrar un estudi seleccionat a la QStudyTreeView
    void delStudy();
    
    ///signal que s'emet quan es vol veure un estudi seleccionat a la QStudyTreeView
    void view();
    
    ///signal que s'emet quan es vol afegir una serie al QSeriesListWidget
    void addSeries(Series *serie);
    
    ///signal que s'emet per netejar el QSeriesListWidget
    void clearSeriesListWidget();
    
    ///signal que s'emet quan seleccionem una serie, cap al QSeriesListWidget, per a que seleccioni la mateixa serie que nosaltres tenim seleccionada
    void selectedSeriesList( QString ); 

public slots:
    
    /** Si es selecciona una serie del QSeriesListWidget s'ha seleccionar la mateixa en el QStudyTreeWidget, al seleccionar una serie del SeriesIconView, salta aquest slot i selecciona la serie de l'estudi seleccionada al SeriesIconView
     * @param SeriesUID Uid de la serie seleccionada en QSeriesListWidget
     */
    void selectedSeriesIcon( QString );
    
    /** Quant seleccionem una serie de la llista, emiteix un signal cap al QSeriesListWidget per a que hi seleccioni la serie, seleccionada, a mes si clickem sobre un estudi expandid, s'ha de tornar a recarregar el QSeriesListWidget amb les series d'aquell estudi tamb� en el QSeriesListWidget
     * @param item sobre el que s'ha fet click
     */
    void clicked( QTreeWidgetItem * , int );
    
    /**  Si fem doble click a una serie del TreeView es visualitzar� si �s una s�rie, o si �s un estudi es mostraran les series o s'amagaran si abans es mostraven
     * @param item sobre el que s'ha fet click
     */
    void doubleClicked( QTreeWidgetItem * , int );
    
    /// Neteja el TreeView
    void clear( );
    
    /// Slot que descarrega un estudi
    void retrieveStudy( );

    /// ESborra un estudi de la cach�     
    void deleteStudy( );
    
    /// Slot que visualitza l'estudi
    void viewStudy( );

private :

    QMenu m_contextMenu;///<Menu contextual
    QString m_parentName , m_oldStudyUID , m_oldPacsAETitle , m_OldInstitution; ///< strings per guardar valors de l'anterior element
    QIcon m_openFolder , m_closeFolder , m_iconSeries;///< icones utilitzades com a root al TreeWidget
    
    /// crea les connexions dels signals i slots
    void createConnections( );
    
    /// Assigna l'ampla a les columnes segons els par�metres guardats a StarviewerSettings
    void setWidthColumns( );
    
    /// Creem el menu contextual, en funcio de a quin tab pertany activa unes o altres opcions del menu
    void createContextMenu( );
    
    /** Inseriex la informaci� d'un estudi
     * @param Dades de l'estudi
     */
    void insertStudy( Study * );
    
    /** Quant es consulten les s�ries d'un estudi, es fa un acces al pacs demanant la informaci� d'aquelles series,si es tornen a consultar una segona vegada les s�ries de l'estudi,no cal tornar a accedir al pacs perqu� ja tenim la informaci� de la s�rie al TreeView, per� s'ha d'actualitzar QSeriesListWidget amb  la informaci� de les s�ries de l'estudi, com no tornem a accedir al pacs, la informaci� de les s�ries li hem de passar d'algun mode, per aix� el que fem �s invocar aquest m�tode que crea reconstrueix l'objecte series, amb la principal informaci� de les s�ries, i que fa un emit, que �s capturat pel QSeriesInconView, per mostrar la informaci� de la s�rie (la connexi� entre el QStudyTreeWidget i QSeriesListWidget es fa la constrcutor de la QueryScreen)
     * @param Apuntador a l'estudi al tree view
     */
    void setSeriesToSeriesListWidget( QTreeWidgetItem *item );
    
    /** Expandeix l'estudi i mostra la llista series
     * @param item a expandir
     */
    void expand( QTreeWidgetItem * );

    /** Formata el nom
     * @param Nom i cognoms del pacient
     */
    QString formatName( const std::string );
    
    /** formata l'edat per mostrar per pantalla
     * @param edat
     */
    QString formatAge( const std::string );
    
    /**Formata la data
     * @param data de l'estudi
     */
    QString formatDate( const std::string );
    
    /**Formata l'hora
     * @param Hora de l'estudi
     */
    QString formatHour( const std::string );
};

}; // end namespace

#endif
