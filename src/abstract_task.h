//
// This is an example class
//

#ifndef ABSTRACT_TASK_H
#define ABSTRACT_TASK_H

#include <stdint.h>
#include <string>

using namespace std;

class abstract_task {
private:
    // put private/internal variables/functions here
    uint32_t sleep_polling_ms = 1000;
public:
    void sleep(void);
    virtual string getName(void);
    virtual void init(void);
    void create_task(void);
    static void task(void *pvParameter);

    virtual void re_init(void);
    virtual void exit(void);
    string getInfo(void);
};


#endif // ABSTRACT_TASK_H
