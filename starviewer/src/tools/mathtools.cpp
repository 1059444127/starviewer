/***************************************************************************
 *   Copyright (C) 2005 by Grup de Gr�fics de Girona                       *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/

#include "mathtools.h"
#include <vtkMath.h>
#include <vtkPlane.h>
namespace udg{
 
 
 ///logaritme en base 2
double MathTools::logTwo(const double x, const bool zero)
{
    if (x < 0) 
        std::cerr << "MathTools::logTwo >> Log of negative number" << std::endl;

    if (zero) 
    {
        return (x == 0) ? 0 : double( log(double(x)) ) * M_LOG2E; 
    } 
    else 
    { 
        if (x == 0) 
            std::cerr << "MathTools::logTwo >> Log of zero" << std::endl;
            
        return double( log(double(x)) ) * M_LOG2E; 
    }
}

/// C�lcul de l'entropia bin�ria
double MathTools::binaryEntropy(const double p)
{
    if ( p < 0 || p > 1 )
        std::cerr << "MathTools::binaryEntropy >> Value [" << p << "] out of range (0..1) -> " << std::endl;

    return - p * MathTools::logTwo(p) - (1 - p) * MathTools::logTwo(1 - p) ;
}

double MathTools::divReal( const double dividend,  const double divisor, const bool zero, const double value )
{
    if (zero) 
    {
        return (divisor == 0) ? value : dividend / divisor; 
    }
    else 
    {
        if (divisor == 0) 
            std::cerr <<"MathTools::divReal >> Division by zero" << std::endl;
        return dividend / divisor; 
    }
}

double MathTools::angleInRadians( double vec1[3] , double vec2[3] )
{
    return acos( vtkMath::Dot( vec1,vec2 ) / ( vtkMath::Norm(vec1)*vtkMath::Norm(vec2) ) );
}

double MathTools::angleInDegrees( double vec1[3] , double vec2[3] )
{
    return angleInRadians( vec1 , vec2 ) * vtkMath::DoubleRadiansToDegrees();
}
 
int MathTools::planeIntersection( double p[3] , double n[3], double q[3] , double m[3], double r[3] , double t[3] )
{
    
    if( angleInDegrees( n , m ) == 0.0 )
    {
        return 0;
    }
    //
    // soluci� extreta de http://vis.eng.uci.edu/courses/eecs104/current/GraphicsMath.pdf, p�g. 64
    // pla1 definit per (p,n); p: punt del pla, p.ex. origen; n: normal
    // pla2 definit per (q,m); q: punt del pla, p.ex. origen; m: normal
    // l�nia d'intersecci� (r,t); r: punt de la recta que pertany a tots dos plans; t: vector director
    // u: vector perpendicular a n i t;
    // Cross: producte vectorial
    // Dot: producte escalar
    // * : multiplicaci� de vectors
    // + : suma de vectors
    //
    // ******* FORMULETA *************
    //
    // t = Cross(n,m) 
    // u = Cross(n,t)
    // r = p + Dot(p-q)*u / Dot(u,m)
    //
    double u[3]; 
    vtkMath::Cross( n , m , t );
    vtkMath::Cross( n , t , u );
     
    double pq[3] , sum[3] , pqDotm , dot_u_m;
   
    pq[0] = q[0] - p[0];
    pq[1] = q[1] - p[1] ;
    pq[2] = q[2] - p[2];
    
    
    pqDotm = vtkMath::Dot( pq , m );
    dot_u_m = vtkMath::Dot( u , m );
    sum[0] = ( pqDotm * u[0] ) / dot_u_m;
    sum[1] = ( pqDotm * u[1] ) / dot_u_m;
    sum[2] = ( pqDotm * u[2] ) / dot_u_m;
    
    r[0] = sum[0] + p[0];
    r[1] = sum[1] + p[1];
    r[2] = sum[2] + p[2];
    
    return 1;
}

int MathTools::planeIntersection( double p[3] , double n[3], double q[3] , double m[3], double r[3] , double t[3] , double intersectionPoint[3] )
{
    //
    // soluci� extreta de http://vis.eng.uci.edu/courses/eecs104/current/GraphicsMath.pdf, p�g. 65
    // pla1 definit per (p,n); p: punt del pla, p.ex. origen, n: normal del pla
    // pla2 definit per (q,m); 
    // pla3 definit per (r,t);
    // la intersecci� ser� un punt w
    // si w = p + a�n + b�m + c�t
    // llavors caldria resoldre el sistema lineal
    // pw � n  = 0 , qw � m = 0 , rw � t = 0
    // per a, b i c 
    //
    // o b� calcular la l�nia d'intersecci� entre dos plans i el punt d'intersecci� de la l�nia amb el pla restant
    //
    // Sembla m�s f�cil la segona opci�
    double point[3] , vector[3];
    planeIntersection( p , n , q , m , point , vector );
    //c�lcul intersecci� l�nia pla
    // L�nia representada per punt i vector(p,t), pla per punt(origen) i normal (r,n), q �s la intersecci�
    // q = p + (pr�n)t / (t�n)
    
    double tt , point2[3];
    point2[0] = point[0] + vector[0];
    point2[1] = point[1] + vector[1];
    point2[2] = point[2] + vector[2];
    
    
    // li donem una recta definida per dos punts , i el pla definit per la normal i un punt. T �s la coordenada param�trica al llarg de la recta i el punt de la intersecci� queda a intersectPoint
    
    if( vtkPlane::IntersectWithLine( point , point2 , t , r ,  tt , intersectionPoint ) == 0 )
    {
        // si retorna 0 �s que o b� l�nia i pla no intersecten o s�n paralels entre s�
        if( tt == VTK_DOUBLE_MAX )
        {
            std::cerr << "No hi ha hagut intersecci�! Valor coord param�trica :" << tt << std::endl;
            return -1;
        }
        else
            return 0;
        
    }
    
    return 1;    
}

}; // end namespace udg
