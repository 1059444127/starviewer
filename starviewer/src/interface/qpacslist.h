/***************************************************************************
 *   Copyright (C) 2005 by Grup de Gr�fics de Girona                       *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/ 
#ifndef UDGQPACSLIST_H
#define UDGQPACSLIST_H

#include "ui_qpacslistbase.h"
#include <QWidget>
#include "status.h"
using namespace Ui;

namespace udg {

class PacsList;

/** Interf�cie que mostra els PACS els quals es pot connectar l'aplicaci�, permet seleccionar quins es vol connectar l'usuari
@author marc
*/

class QPacsList : public QWidget, private /*Ui::*/QPacsListBase
{
Q_OBJECT
public:

    ///Constructor de la classe
    QPacsList(QWidget *parent = 0 );
    
    /** Retorna els pacs seleccionats per l'usuari per a realitzar la cerca
     * @param pacslist , parametre de sortida que conte la llista de pacs seleccionats 
     * @return estat de l'operaci�
     */
    Status getSelectedPacs(PacsList * pacsList);
    
    ///Destructor de la classe
    ~QPacsList();
   
public slots :
    
    /// Carrega al ListView la Llista de Pacs disponibles
    void refresh();   
   
private :
    
    /// Aquesta accio selecciona en el PacsListView els Pacs que tenen a 'S' a Default. Son els pacs que per defecte l'usuari te que es realitzin les cerques
    void setSelectedDefaultPacs();
    
    /** Tracta els errors que s'han produ�t durant els accessos a la base dades
     *  @param state Estat de l'acci� retrieve
      */
    void databaseError(Status *state);

};

};

#endif
