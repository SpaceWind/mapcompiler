#ifndef HELPERS_H
#define HELPERS_H


#include <QVector>
#include <QRect>
#include <QtGlobal>

struct FloatRect
{
        float left, top, right, bottom;
        float Width(){return right-left;}
        float Height(){return bottom-top;}
};


#define KEY_UP          0x0100
#define KEY_DOWN        0x0200

#define KB_STATE_WINAPI_FORMAT          0
#define KB_STATE_BOOLEAN_FORMAT         1


#define NO_COLLISION            0
#define COLLISION_RIGHT         1//rect1 -> rect2
#define COLLISION_UP            2//rect1 \  rect2
#define COLLISION_DOWN          3//rect1 /  rect2
#define COLLISION_LEFT          4//rect2 <- rect1

#define KH_BUFFER_SIZE                          16
#define KH_BUFFER_BOUND                         KH_BUFFER_SIZE-1

#define list_foreach(t,n,e) for(std::list<t>::iterator n = e.begin(); n!=e.end(); ++n)
#define map_foreach(k,v,n,e) for(std::map<k,v>::iterator n = e.begin(); n!=e.end(); ++n)


typedef unsigned int uint;
typedef unsigned char byte;



struct Pointer2D
{
        double *l;
        double *t;
};
struct TDoubleRect
{
        double left, top, right, bottom;
        double Width(){return right-left;}
        double Height(){return bottom-top;}
};
struct kbState
{
        kbState(){memset(state,0,256);}
        static kbState get(int format);
        byte state[256];
        byte operator[](int index) {return state[index];}
};

int collisionType(QRect a, QRect b);
double rectRange(QRect A, QRect B);



#endif // HELPERS_H
