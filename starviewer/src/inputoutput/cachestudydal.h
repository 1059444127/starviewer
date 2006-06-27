/***************************************************************************
 *   Copyright (C) 2005-2006 by Grup de Gr�fics de Girona                  *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/
#ifndef UDGCACHESTUDYDAL_H
#define UDGCACHESTUDYDAL_H

class string;

namespace udg {

class Study;
class Status;
class StudyList;
class StudyMask;

/** Aquesta classe s'encarrega de dur a terme les operacions relacionades amb l'objecte estudi de la cache de l'aplicaci�.
	@author Grup de Gr�fics de Girona  ( GGG ) <vismed@ima.udg.es>
*/
class CacheStudyDAL
{
public:
    
    CacheStudyDAL();

    /** Afegeix un nou estudi i pacient a la bd local, quant comencem a descarregar un nou estudi.
     *   La informaci� que insereix �s :
     *        Si el pacient no existeix - PatientId
     *                                  - PatientName
     *                                  - PatientBirthDate
     *                                  - PatientSex  
     *
     *       Si l'estudi no existeix    - PatientID
     *                                  - StudyUID
     *                                  - StudyDate
     *                                  - StudyTime
     *                                  - StudyID
     *                                  - AccessionNumber
     *                                  - StudyDescription
     *                                  - Status
     *  La resta d'informaci� no estar� disponible fins que les imatges estiguin descarregades, 
     *                    
     * @param Study[in]  Informaci� de l'estudi 
     * @return retorna l'estat de l'inserci�                                    
     */
    Status insertStudy( Study * ); 

    /** Cerca els estudis que compleixen la m�scara a la cach�
     * @param M�scara de la cerca
     * @param StudyList amb els resultats
     * @return retorna estat del m�tode
     */
    Status queryStudy( StudyMask mask , StudyList &list );

    /** Cerca l'estudi que compleix amb la m�scara de cerca. Cerca ens els estudis que estan en estat Retrived o Retrieving
     * @param  M�scara de  la cerca
     * @param  StudyList amb els resultats
     * @return retorna estat del m�tode
     */
    Status queryStudy( std::string studyUID , Study &study );

    /** Selecciona els estudis vells que no han estat visualitzats des de una data inferior a la que es passa per parametre
     * @param Data a partir de la qual es seleccionaran els estudis vells
     * @param StudyList amb els resultats dels estudis, que l'ultima vegada visualitzats es una data inferior a la passa per par�metre
     * @return retorna estat del m�tode
     */
    Status queryOldStudies( std::string OldStudiesDate, StudyList &list );
    
    /** Esborra un estudi de la cache, l'esborra la taula estudi,series, i image, i si el pacient d'aquell estudi, no t� cap altre estudi a la cache local tambe esborrem el pacient. Aquesta operaci� �s dur a terme en una transacci�, si falla el m�tode, es tirara endarrera, fins a l'estat estable de la base de dades
     * @param std::string UID de l'estudi
     * @return estat de l'operaci�
     */
    Status delStudy( std::string );
    
    /** Aquesta acci� es per mantenir la integritat de la base de dades, si ens trobem estudis al iniciar l'aplicaci� que tenen l'estat pendent o descarregant vol dir que l'aplicaci� en l'anterior execussi� ha finalitzat an�malament, per tant aquest estudis en estat pendents, les seves s�rie i imatges han de ser borrades perqu� es puguin tornar a descarregar. Aquesta acci� �s simplement per seguretat!
     * @return estat de l'operaci�
     */
    Status delNotRetrievedStudies();    

    /** actualitza l'�ltima vegada que un estudi ha estat visualitzat, d'aquesta manera quant haguem d'esborrar estudis
     * autom�ticament per falta d'espai, esborarrem els que fa m�s temps que no s'han visualitzat
     * @param UID de l'estudi
     * @param hora de visualitzaci� de l'estudi format 'hhmm'
     * @param data visualitzaci� de l'estudi format 'yyyymmdd'
     * @return estat el m�tode
     */
    Status updateStudyAccTime( std::string studyUID );
        
    /** Updata la modalitat d'un estudi
     * @param Uid de l'estudi a actualitzar
     * @param Modalitat de l'estudi
     * @return retorna estat del m�tode
     */
    Status setStudyModality( std::string studyUID , std::string modality );
    
    /** Updata un estudi PENDING a RETRIEVING, per indicar que l'estudi s'esta descarregant
     * @param Uid de l'estudi a actualitzar
     * @return retorna estat del m�tode
     */
    Status setStudyRetrieving( std::string studyUID );
    
    /** Updata un estudi a l'estat RETRIEVED, per indicar que s'ha descarregat
     * @param Uid de l'estudi a actualitzar
     * @return retorna estat del m�tode
     */
    Status setStudyRetrieved( std::string studyUID );
    
    
    ~CacheStudyDAL();

private :

    /** Construeix la sent�ncia sql per fer la query de l'estudi en funci� dels parametres de cerca
     * @param mascara de cerca
     * @return retorna estat del m�tode
     */
    std::string buildSqlQueryStudy( StudyMask *studyMask );
    
    /** Converteix l'asterisc, que conte el tag origen per %, per transformar-lo a la sintaxis de sql
     * @param string original
     * @return retorna l'string original, havent canviat els '*' per '%'
     */
    std::string replaceAsterisk( std::string original );

    /** retorna l'hora del sistema
     * @return retorna l'hora del sistema en format HHMM
     */
    int getTime();
    
    /** retorna la data del sistema
     *    @return retorna la data del sistema en format yyyymmdd
     */
    int getDate();

};
}

#endif
