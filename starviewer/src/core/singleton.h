/***************************************************************************
 *   Copyright (C) 2005-2006 by Grup de Gr�fics de Girona                  *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/
#ifndef UDGSINGLETON_H
#define UDGSINGLETON_H

namespace udg {

/**
    Classe que implementa el pattern singleton. La implementaci� no �s thread-safe, per tant, no s'hauria d'utilitzar des de diferents
    threads. Qualsevol classe que es vulgui convertir a Singleton haur� de tenir un constructor sense par�metres.
    La manera d'utilitzar-la seria la seg�ent:
    \code
    typedef Singleton<TestingClass> TestingClassSingleton;
    ...
    TestingClass* instance = TestingClassSingleton::instance();
    instance->metode();
    ...
    \endcode
    Com es pot veure, �s molt recomenable utilizar un typedef per tal de que no disminueixi la llegibilitat del codi. Aquesta manera
    d'utilitzar el singleton �s perillosa si es vol que no hi pugui haver cap altra inst�ncia de la classe TestingClass. D'aquesta manera
    el que s'est� fent �s garantir que del tipus TestingClassSingleton tindrem una inst�ncia globalment accessible. Per� res ens impedeix
    crear classes del tipus TestingClass a part.
    Una altra manera de declarar una classe com a singleton seria la seg�ent:
    \code
    class OnlyOne : public Singleton<OnlyOne>
    {
        //..resta del codi
    };
    \endcode
    D'aquesta forma s� que estem assegurant que de la classe OnlyOne, en tota la vida del programa, nom�s n'hi haur� una i ser� la
    mateixa per tota l'execuci�.
    \todo Fer-la thread-safe.

	@author Grup de Gr�fics de Girona  ( GGG ) <vismed@ima.udg.es>
*/
template <typename T>
class Singleton{

public:

    /// Ens serveix per accedir a l'�nica inst�ncia de la classe T
    static T* instance()
    {
        static T theSingleInstance;
        return &theSingleInstance;
    }

protected:
    Singleton(){}; // No s'implementa
    Singleton(const Singleton&){}; // No s'implementa
    Singleton &operator=(const Singleton&){}; // No s'implementa
    
};

}

#endif
