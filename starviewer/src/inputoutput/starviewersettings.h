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
const QString dicomdirColumnWidthKey("pacs/interface/studyDicomdirList/columnWidth" );//en aquesta clau a darrera s'hi concatena el n�mero de columna ,  per diferenciar cada columna
const QString MaximumDaysNotViewedStudy("pacs/cache/MaximumDaysNotViewedStudy" );
const QString InstitutionName("pacs/institution/name");
const QString InstitutionAddress("pacs/institution/address");
const QString InstitutionTown("pacs/institution/town");
const QString InstitutionZipCode("pacs/institution/zipCode");
const QString InstitutionCountry("pacs/institution/country");
const QString InstitutionEmail("pacs/institution/Email");
const QString InstitutionPhoneNumber("pacs/institution/phoneNumber");

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
    
    /** guarda la mida de la columna que se li passa per par�metre del QStudyListView ,  encarregat de mostrar les dades d'un dicomdir
     * @param n�mero de columna
     * @param amplada de la columna
     */
    void setStudyDicomdirListColumnWidth( int col , int width ); 
      
	/** retorna l'amplada del n�mero de columna de la llista d'estudis del PACS ,  passat per par�metre
	 * @return amplada de la columna
	 */
   	int getStudyPacsListColumnWidth( int column );

	/** retorna l'amplada del n�mero de columna de la llista d'estudis de la cache ,  passat per par�metre
	 * @return amplada de la columna
	 */
   	int getStudyCacheListColumnWidth( int column );
      
    /** retorna l'amplada del n�mero de columna de la llista d'estudis de Dicomdir,  passat per par�metre
     * @return amplada de la columna
     */
    int getStudyDicomdirListColumnWidth( int column );

    //informaci� de l'instituci�
    
    /** Guarda el nom de la instituci�
     * @param institutionName nom de l'instituci�
     */
    void setInstitutionName( QString institutionName );
    
    /** Guarda l'adre�a de la instituci�
     * @param institutionAddress adre�a de la instituci�
     */
    void setInstitutionAddress( QString institutionNameString );
    
    /** Guarda la poblacio de la instituci�
     * @param institutionTown poblaci� de la instituci�
     */
    void setInstitutionTown( QString institutionTownString );
    
    /** Guarda el codi postal de la instituci�
     * @param institutionZipCode codi postal del a instituci�
     */
    void setInstitutionZipCode( QString institutionZipCodeString );

    /** Guarda el pa�s de la instituci�
     * @param institutionCountry 
     */
    void setInstitutionCountry( QString institutionCountryString );

    /** Guarda el correu electr�nic de la instituci�
     * @param institutionEmail correu electr�nic
     */
    void setInstitutionEmail( QString institutionEmailString );

    /** Guarda el telefon de la instituci�
     * @param institutionPhoneNumber tel�fon 
     */
    void setInstitutionPhoneNumber( QString institutionPhoneNumberString );
       
    /** retorna el nom de la instituci�
     * @return nom de la instituci�
     */
    QString getInstitutionName();
    
    /** retorna el adre�a de la instituci�
     * @return adre�a de la instituci�
     */
    QString getInstitutionAddress();
    
    /** retorna la ciutat de la instituci�
     * @return ciutat de la instituci�
     */
    QString getInstitutionTown();
    
    /** retorna el codi postal de la instituci�
     * @return codi postal de la instituci�
     */
    QString getInstitutionZipCode();
    
    /** retorna el pa�s de la instituci�
     * @return pa�s de la instituci�
     */
    QString getInstitutionCountry();
    
    /** retorna el correu electr�nic de la instituci�
     * @return correu electr�nic de la instituci�
     */
    QString getInstitutionEmail();
    
    /** retorna el tel�fon de la instituci�
     * @return tel�fon de la instituci�
     */
    QString getInstitutionPhoneNumber();
    
    
private :

    QSettings m_starviewerSettings;
    
};

};

#endif
