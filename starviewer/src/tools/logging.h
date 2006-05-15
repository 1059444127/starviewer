/***************************************************************************
 *   Copyright (C) 2005-2006 by Grup de Gr�fics de Girona                  *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/

#ifndef _LOGGING_
#define _LOGGING_
 
/*!
    Aquest arxiu cont� totes les macros per a fer logs en l'aplicaci�.
*/

// include log4cxx header files.
#include <log4cxx/logger.h>
#include <log4cxx/basicconfigurator.h>
#include <log4cxx/propertyconfigurator.h>
#include <log4cxx/helpers/exception.h>

#define DEBUG
// std::string file = "log4cxx.properties"; \
// log4cxx::BasicConfigurator::configure();
/// Macro per a inicialitzar els logger
#define LOGGER_INIT( file ) \
{ \
    log4cxx::PropertyConfigurator::configure( file ); \
    INFO_LOG("Inicialitzaci� de l'aplicaci�") \
}

// if ( fileExist( filename ) )
// {
//         // Si el fitxer de configuraci� existeix
//         PropertyConfigurator::configure( filename );
// }
// else
// {
//         // configuraci� per defecte, quin remei!
//         BasicConfigurator::configure();
// }

/// Macro per a missatges de debug
#ifdef DEBUG 
#define DEBUG_LOG( msg ) LOG4CXX_DEBUG( log4cxx::Logger::getLogger("development") , msg )
#else
#define DEBUG_LOG( msg )
#endif

/// Macro per a missatges d'informaci� general
#define INFO_LOG( msg ) LOG4CXX_INFO( log4cxx::Logger::getLogger("info.release") , msg )

/// Macro per a missatges de warning
#define WARN_LOG( msg ) LOG4CXX_WARN( log4cxx::Logger::getLogger("info.release") , msg )

/// Macro per a missatges d'error
#define ERROR_LOG( msg ) LOG4CXX_ERROR( log4cxx::Logger::getLogger("errors.release") , msg )

/// Macro per a missatges d'error fatals/cr�tics
#define FATAL_LOG( msg ) LOG4CXX_FATAL( log4cxx::Logger::getLogger("errors.release") , msg );

#endif
