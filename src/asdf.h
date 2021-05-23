//
// This is an example class
//

#ifndef ABSTRACT_TASK_H
#define ABSTRACT_TASK_H

#include <stdint.h>
#include <string>

using namespace std;

class MyClass{
public:
    //static void init_task( void* pvParams);
    void task(void);
    void create_task(void);
    static void init_task( void* pvParams ){
        ((MyClass*)pvParams)->task();
    };
};


#endif // ABSTRACT_TASK_H
