/***************************************************************************
 *   Copyright (C) 2005 by Grup de Gr�fics de Girona                       *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/ 
#ifndef UDGQOPERATIONSTATESCREEN_H
#define UDGQOPERATIONSTATESCREEN_H

#include <QString>
#include <semaphore.h>

#include "study.h"
#include "image.h"
#include "starviewerprocessimage.h"
#include "ui_qoperationstatescreenbase.h"

/// Interf�cie que implementa la llista d'operacions realitzades cap a un PACS 
namespace udg {

class Status;
class Operation;

class QOperationStateScreen : public QDialog , private Ui::QOperationStateScreenBase{
Q_OBJECT
public:

    /** Constructor de la classe
     * @param parent 
     * @return 
     */
    QOperationStateScreen( QWidget *parent = 0 );

	/// destructor de la classe
    ~QOperationStateScreen();
    
public slots :
    
    /** Insereixu una nova operaci�
     *  @param operation operaci� a inserir
     */
    void insertNewOperation( Operation *operation );

    /** slot que s'invoca quant un StarviewerProcessImage emet un signal imageRetrieved
     * @param uid de l'estudi que ha finalitzat una operaci� d'una imatge
     * @param n�mero d'imatges descarregades
     */    
    void imageCommit( QString stidyUID , int numberOfImages );

    /** Augmenta en un el nombre de series descarregades
     * @param UID de l'estudi que s'ha descarregat una s�rie
     */
    void seriesCommit( QString studyUID );

    /** S'invoca quant s'ha acabat una operaci�. S'indica a la llista que l'operaci� relacionada amb l'estudi ha finalitzat
     * @param  UID de l'estudi descarregat
     */
    void setOperationFinished( QString studyUID );
    
    /** S'invoca quant es produeix algun error durant el processament de l'operaci�
     * @param studyUID UID de l'estudi descarregat
     */
	void setErrorOperation( QString studyUID );
    
    /** S'invoca quan es comen�a l'operaci� d'un estudi, per indicar-ho a la llista que aquell estudi ha comen�at l'operaci�
     * @param  UID de l'estudi que es comen�a l'operaci�
     */
	void setOperating( QString );
    
    /// Neteja la llista d'estudis excepte dels que s'estant descarregant en aquells moments 
    void clearList();
    
private:

    /// Crea les connexions pels signals i slots
    void createConnections();

    /** Esborra l'estudi enviat per parametre
     * @param UID de l'estudi
     */
    void deleteStudy( QString studyUID );
};

};

#endif

