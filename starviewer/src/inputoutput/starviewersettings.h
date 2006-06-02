/***************************************************************************
 *   Copyright (C) 2005 by Grup de Gr�fics de Girona                       *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/ 
#ifndef UDGSTARVIEWERSETTINGS_H
#define UDGSTARVIEWERSETTINGS_H

#include <QSettings>
#include <QString>

namespace udg {

/** Aquesta classe permet accedir i guardar els parametres de configuracio de l'starviewer
@author marc
*/

const QString databaseRootKey("pacs/cache/sdatabasePath" ); //indica on es troba la bd
const QString poolSizeKey("pacs/cache/poolSize" );
const QString cacheImagePathKey("pacs/cache/imagePath" );
const QString AETitleMachineKey("pacs/pacsparam/AETitle" );
const QString localPortKey("pacs/pacsparam/localPort" );
const QString timeoutPacsKey("pacs/pacsparam/timeout" );
const QString maxConnectionsKey("pacs/pacsparam/MaxConnects" );
const QString selectLanguageKey("pacs/language" );
const QString pacsColumnWidthKey("pacs/interfase/studyPacsList/columnWidth" );//en aquesta clau a darrera s'hi concatena el n�mero de columna ,  per diferenciar cada columna
const QString cacheColumnWidthKey("pacs/interface/studyCacheList/columnWidth" );//en aquesta clau a darrera s'hi concatena el n�mero de columna ,  per diferenciar cada columna
const QString MaximumDaysNotViewedStudy("pacs/cache/MaximumDaysNotViewedStudy" );

class StarviewerSettings{
public:

	/// Constructor de la casse
    StarviewerSettings();

	///Destructor de la classe
    ~StarviewerSettings();

    //Cache
	/** estableix el path de la base de dades
	 * @param path de la base de dades local
	 */
    void setDatabasePath( QString );

	/** estableix la mida de la pool
	 * @param mida de la pool
	 */
    void setPoolSize( QString );

	/** estableix el path on es guarden les imatges de la cache
	 * @param path de la cache
	 */
    void setCacheImagePath( QString );

	/** Estableix el n�mero de dies m�xim que un estudi pot estar a la cache sense ser vist ,  a partir d'aquest n�mero de dies l'estudi �s esborrat
	 * @param n�mero maxim de dies
	 */
    void setMaximumDaysNotViewedStudy( QString  );
      
	/** retorna el path de la base de dades
	 * @return retorna el path de la base de dades
	 */
    QString getDatabasePath();

	/** retorna la mida de la pool
	 * @return retorna la mida de la pool
	 */
    QString getPoolSize();

	/** retorna el Path on es guarden les imatges
	 * @return retorn el path de la cache
	 */
    QString getCacheImagePath();

	/** retorna el n�mero m�xim de dies que un estudi pot estar a la cach� sense ser visualitzat
	 * @return retorn el path de la cache
	 */
    QString getMaximumDaysNotViewedStudy();
    
    //Pacs
	/** guarda el AETitle de la m�quina
	 * @param AETitle de la m�quina
	 */
    void setAETitleMachine( QString );

	/** estableix el Time out
	 * @param Time out
	 */
    void setTimeout( QString );

	/** guarda el port Local pel qual rebrem les imatges descarregades
	 * @param port local per la descarrega d'imatges
	 */
    void setLocalPort( QString );

	/** estableix l'idioma de l'aplicaci�
	 * @param idioma de l'aplicaci�
	 */
    void setLanguage( QString );

	/** Nombre m�xim de connexions simult�nies al PACS
	 * @param nombre m�xim de connexions
  	 */
    void setMaxConnections( QString );
      
	/** retorna el AEtitle de la m�quina
	 * @return AETitle de la m�quina
	 */
    QString getAETitleMachine();

	/** retorna el time out de l'aplicacio
	 * @return timeout
 	 */
    QString getTimeout();

	/** retorna el port pel qual esperem rebre les imatges descarregades
	 * @return port Local
	 */
    QString getLocalPort();

	/** retorna l'idioma seleccionat per l'usuari
	 * @return idioma seleccionat
	 */
    QString getLanguage();

	/** retorna el nombre m�xim de connexions simult�nies que es poden tenir atacant a un PACS
	 * @return nombre m�xim de connexions
	 */
    QString getMaxConnections();
      
    //interficie

	/** guarda la mida de la columna que se li passa per par�metre del QStudyListView ,  encarregat de mostrar les dades del Pacs
	 * @param n�mero de columna
	 * @param amplada de la columna
	 */
    void setStudyPacsListColumnWidth( int col , int width );

	/** guarda la mida de la columna que se li passa per par�metre del QStudyListView ,  encarregat de mostrar les dades de la cache
	 * @param n�mero de columna
	 * @param amplada de la columna
	 */
    void setStudyCacheListColumnWidth( int col , int width );      
      
	/** retorna l'amplada del n�mero de columna de la llista d'estudis del PACS ,  passat per par�metre
	 * @return amplada de la columna
	 */
   	int getStudyPacsListColumnWidth( int column );

	/** retorna l'amplada del n�mero de columna de la llista d'estudis de la cache ,  passat per par�metre
	 * @return amplada de la columna
	 */
   	int getStudyCacheListColumnWidth( int column );
      
private :

    QSettings m_starviewerSettings;
    
};

};

#endif
