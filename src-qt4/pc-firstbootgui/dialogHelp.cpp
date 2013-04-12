#include <QTextEdit>

#include "dialogHelp.h"
#include "backend.h"

void dialogHelp::dialogInit(QString helpText)
{
	connect(pushClose, SIGNAL(clicked()), this, SLOT(slotClose()));
	textHelp->setText(helpText);
}

void dialogHelp::slotClose()
{
	close();
}
