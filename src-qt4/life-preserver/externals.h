#include <QString>
#include "lifePreserverMain.h"
#include "lifePreserverWelcome.h"
#include "../config.h"

const QString LIFEPRESERVERPATH( PREFIX + "/share/lifePreserver");
const QString LIFEPRESERVERSDIR( LIFEPRESERVERPATH + QString("/preservers"));

extern bool havePreservers;
extern lifePreserver *m;
extern lifePreserverWelcome *w;
