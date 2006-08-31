/***************************************************************************
 *   Copyright (C) 2005 by Grup de Gr�fics de Girona                       *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/ 
#ifndef UDGDATABASECONNECTION_H
#define UDGDATABASECONNECTION_H

#include <sqlite3.h>
#include <semaphore.h>

namespace udg {

class string;
class Status;

/** Com tenim m�s d'una classe que han d'accedir a la mateixa Base de dades, i amb SQLITE nom�s podem tenir una connexi� per la BD creem una classe singleton que s'encarregar� de gestionar la connexi� a la Base de Dades. De la mateixa manera nom�s un thread alhora pot accedir a la BD, aquesta classe implementar� m�todes per evitar que dos threads es trobin a la vegada dins la BD
@author marc
*/
class DatabaseConnection
{
public:

    /// Constructor estatic del singleton retorna la refer�ncia a la casse     
     static DatabaseConnection* getDatabaseConnection()
     {
         static DatabaseConnection database;
         return &database;
     }
     
     /** Establei el path de la base de dades, per defecte, si no s'estableix, el va a buscar a la classe StarviewerSettings
      * @param path de la base de dades
      */
    void setDatabasePath(std::string);
     
    /** Retorna la connexi� a la base de dades
     * @return connexio a la base de dades, si el punter �s nul, �s que hi hagut error alhora de connectar, o que el path no �s correcte
     */
     sqlite3 * getConnection();
     
    /** Indica s'esta connectat a la base de dades
     * @return indica si s'esta connectat a la base de dades
     */
    bool connected();
    
    /// Demana el candeu per accedir a la base de dades!. S'ha de demanar el candau per poder accedir de manera correcte i segura a la base de dades ja que si hi accedeixen dos objectes, amb la mateixa connexi� al mateix temps, donar� error, per aix� des de la connexi� ens hem d'assegurar que nom�s �s utilitzada una vegada
    void getLock();
    
    /// Allibera al candau per a que altres processos puguin accedir a la base de dades
    void releaseLock();
    
    ///Construeix l'estat de la base de dades en funci� del valor que ha retornat la operaci�
    Status databaseStatus( int state );
    
private :
    
    /// Constructor de la classe
    DatabaseConnection();

    sqlite3 *m_databaseConnection;
    sem_t *m_databaseLock;
    
    std::string m_databasePath;
    
    /// tanca la connexi� de la base de dades*/
    void closeDB(); 
    
    ////connecta amb la base de dades segons el path
    void connectDB();
    
    ///destructor de la classe
    ~DatabaseConnection();
};
};//end namespace

#endif
