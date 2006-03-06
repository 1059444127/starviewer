/***************************************************************************
 *   Copyright (C) 2005 by Grup de Gr�fics de Girona                       *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/ 
#ifndef UDGDATABASECONNECTION_H
#define UDGDATABASECONNECTION_H
#include <sqlite.h>
#include <semaphore.h>
#include <string>

namespace udg {

/** Com tenim m�s d'una classe que han d'accedir a la mateixa Base de dades, i amb SQLITE nom�s podem tenir una connexi� per la BD creem una classe
  * singleton que s'encarregar� de gestionar la connexi� a la Base de Dades. De la mateixa manera nom�s un thread alhora pot accedir a la BD, aquest 
  * classe implementar� m�todes per evitar que dos threads es trobin a la vegada dins la BD
@author marc
*/
class DatabaseConnection{
public:

    /** Constructor estatic del singleton retorna la refer�ncia a la casse
      */     
     static DatabaseConnection* getDatabaseConnection()
     {
         static DatabaseConnection database;
         return &database;
     }
     
     void setDatabasePath(std::string);
     
     sqlite * getConnection();
     
     void getLock();
     void releaseLock();
        
private :
    DatabaseConnection();

    sqlite *m_db;
    sem_t *m_databaseLock;
    
    std::string m_databasePath;
    bool  m_ConnectionOpened;
    
    void closeDB(); 
    void connectDB();
    
    ~DatabaseConnection();

};

};

#endif
