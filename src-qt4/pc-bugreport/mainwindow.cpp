#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "report_places.h"

#include <QDebug>
#include <QProcess>

#include "pcbsd-utils.h"

static MainWindow* pMainWindow = NULL;

typedef enum{
	EMAIN_PAGE = 0,
	EFREEBSD_BASE_PAGE,
	EPCBSD_BASE_PAGE,
	EXORG_PAGE,
	EDE_PAGE,
    EGNOME_PAGE,
    EKDE_PAGE,
    EXFCE_PAGE,
    ELXDE_PAGE,
    EMATE_PAGE,
    ECINNAMON_PAGE,
    EUNSUPPORTED_DE_PAGE,
    EFREEBSDSOFT_PAGE,
    EPBISELECT_PAGE,
    EPBIIREPORT_PAGE,
}EPageNumber;



MainWindow::MainWindow(QWidget *parent) :
	QWizard(parent),
	ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    pMainWindow = this;

    setOption(QWizard::HaveFinishButtonOnEarlyPages);

	ui->FreeBSDSendPRW->setup(FREEBSD_SEND_PR);
	ui->FreeBSDBugsLW->setup(FREEBSD_BUGS_MAILLIST);

	ui->PCBSDTrackWidget->setup(PCBSD_TRAC);
	ui->PCBSDTestingLW->setup(PCBSD_TESTING_MAILLIST);

	ui->FreedesktopBugzillaWidget->setup(XORG_BUGZILLA);
	ui->XORGLW->setup(XORG_MAILLIST);
	ui->FreeBSDX11LW->setup(FREEBSD_X11_MAILLIST);
	ui->PCBSDTestingLW_2->setup(PCBSD_TRAC);

    ui->GnomeFreeBSDLW->setup(FREEBSD_GNOME_MAILLIST);
    ui->PCBSDTestingLW_3->setup(PCBSD_TRAC);

    ui->KDEBugs->setup(KDE_BUGTRACKER);
    ui->KDEFreeBSDLW->setup(KDE_FREEBSD_MAILLIST);
    ui->PCBSDTestingLW_4->setup(PCBSD_TRAC);

    ui->XFCEBugzillaWidget->setup(XFCE_BUGZILLA);
    ui->XFCEFreeBSDLW->setup(FREEBSD_XFCE_MAILLIST);
    ui->PCBSDTestingLW_5->setup(PCBSD_TRAC);

    ui->LXDEBugtrackerWidget->setup(LXDE_BUGTRACKER);
    ui->PCBSDTestingLW_6->setup(PCBSD_TRAC);

    ui->PCBSDTestingLW_7->setup(PCBSD_TRAC);

    ui->FreeBSDPortsBugsLW->setup(FREEBSD_PORTS_BUGS_MAILLIST);

    ui->PCBSDTPBIBugsLW->setup(PCBSD_PBI_BUGS_MAILLIST);

    ui->MATEGitHubWidget->setup(MATE_GITHUB_URL);
    ui->PCBSDTestingLW_8->setup(PCBSD_TRAC);

    ui->CinnamonBugTrackerWidget->setup(CINNAMON_BUGTRACKER_GITHUB);
    ui->PCBSDTestingLW_9->setup(PCBSD_TRAC);

	QObject::connect(this, SIGNAL(currentIdChanged(int)),
					 this, SLOT(on_MainWindow_currentIdChanged(int)));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
	QWizard::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

int MainWindow::nextId() const
{
	int curr= currentId();
	if (curr == EMAIN_PAGE)
	{
		if (ui->FreeBSDBaseRB->isChecked())
						//pMainWindow->setupButtons(true);
                        return EFREEBSD_BASE_PAGE;
		if (ui->PCBSDBaseRB->isChecked())
						//pMainWindow->setupButtons(true);
                        return EPCBSD_BASE_PAGE;
		if (ui->XORGCB->isChecked())
						//pMainWindow->setupButtons(true);
			return EXORG_PAGE;
        if (ui->DERB->isChecked())
        {
                        pMainWindow->fillDEList();
                        pMainWindow->setupButtons(false);                        
                        return EDE_PAGE;                        
		}

        if (ui->FreeBSDSoftRB->isChecked())
        {
            return EFREEBSDSOFT_PAGE;
        }

        if (ui->PBIRB->isChecked())
        {
                        //pMainWindow->fillPBIList();
                        return EPBISELECT_PAGE;
        }
        return curr+1;
    }
    if (curr == EDE_PAGE)
    {
        QTreeWidgetItem* item = ui->DEList->currentItem();
        if (!item)
            return EUNSUPPORTED_DE_PAGE;
        return item->data(0, Qt::UserRole).toInt();
    }

    if (curr == EPBISELECT_PAGE)
    {
        QString name;
        QString website;
        QIcon ico;
        if (ui->PBIList->currentItem())
        {
            name= ui->PBIList->currentItem()->text(1);
            website = ui->PBIList->currentItem()->data(0, Qt::UserRole).toString();
            ico = ui->PBIList->currentItem()->icon(0);
        }

        ui->PBISoftSiteWidget->setup(website, &ico);
        page(EPBIIREPORT_PAGE)->setTitle(tr("PBI software: ") + name);
    }

    switch (curr)
    {
        case EFREEBSD_BASE_PAGE:
        case EPCBSD_BASE_PAGE:
        case EXORG_PAGE:
        case EGNOME_PAGE:
        case EKDE_PAGE:
        case EPBIIREPORT_PAGE:
        case EFREEBSDSOFT_PAGE:
        case EMATE_PAGE:
        case ECINNAMON_PAGE:
            return -1;
    }
    return curr+1;
}

void MainWindow::on_MainWindow_currentIdChanged(int id)
{
        switch (id)
        {
			case EMAIN_PAGE:
			{
				QList<QWizard::WizardButton> layout;
				layout<< QWizard::Stretch<<QWizard::NextButton<<QWizard::CancelButton;
				setButtonLayout(layout);
				break;
			}
			case EFREEBSD_BASE_PAGE:
			case EPCBSD_BASE_PAGE:
			case EXORG_PAGE:
            case EGNOME_PAGE:
            case EKDE_PAGE:
            case EXFCE_PAGE:
            case ELXDE_PAGE:
            case EUNSUPPORTED_DE_PAGE:
            case EPBIIREPORT_PAGE:
            case EFREEBSDSOFT_PAGE:
            case EMATE_PAGE:
            case ECINNAMON_PAGE:
				pMainWindow->setupButtons(true);
				break;
            case EPBISELECT_PAGE:
                fillPBIList();
                pMainWindow->setupButtons(true);
			default:
				pMainWindow->setupButtons(false);
				break;
        }
}

void MainWindow::setupButtons(bool isLast)
{
    QList<QWizard::WizardButton> layout;
    if (isLast)
    {
        layout<<QWizard::Stretch<<QWizard::BackButton;
        layout<<QWizard::FinishButton;
		setButtonLayout(layout);
    }
    else
    {
		layout<< QWizard::Stretch<<QWizard::BackButton<<QWizard::NextButton<<QWizard::CancelButton;
		setButtonLayout(layout);
    }

}

void MainWindow::fillDEList()
{
    //InstalledDEList.refresh(true);
    QVector<pcbsd::DesktopEnvironmentInfo> InstalledDEList = pcbsd::Utils::installedDesktops();

    ui->DEList->clear();

    typedef struct SDEEntry
    {
        QString name;
        QString ico;
        EPageNumber page;

    };

    static SDEEntry DE[] =
    {
        {QString("KDE"),QString(":/images/kde.png"), EKDE_PAGE},
        {QString("gnome"),QString(":/images/gnome.png"), EGNOME_PAGE},
        {QString("xfce"),QString(":/images/xfce.png"), EXFCE_PAGE},
        {QString("lxde"),QString(":/images/lxde.png"), ELXDE_PAGE},
        {QString("cinnamon"),QString(":/images/cinnamon.png"), ECINNAMON_PAGE},
        {QString("mate"),QString(":/images/mate.png"), EMATE_PAGE},
    };

    const int DE_SIZE = 6;

    bool hasActiveDesktop = false;

    for (int i=0; i<InstalledDEList.size(); i++)
    {
        if (InstalledDEList[i].isInstalled)
        {
            QString ico;
            EPageNumber page = EUNSUPPORTED_DE_PAGE;

            for(int j=0; j<DE_SIZE; j++)
            {
                if (DE[j].name.trimmed().toLower() == InstalledDEList[i].Name.trimmed().toLower())
                {
                    ico = DE[j].ico;
                    page = DE[j].page;
                }                
            }
            if (!ico.length())
            {
                ico = ":/images/unsupported_de.png";
            }

            QTreeWidgetItem* item = new QTreeWidgetItem;
            QIcon anIcon(ico);

            item->setText(1, InstalledDEList[i].Name);
            item->setIcon(0, anIcon);
            item->setData(0, Qt::UserRole, page);
            ui->DEList->setIconSize(QSize(48,48));

            ui->DEList->addTopLevelItem(item);

            if (InstalledDEList[i].isActive)
            {
                item->setSelected(true);
                hasActiveDesktop = true;
            }//if active
        }//if installed
    }//for all installed

    QTreeWidgetItem* item = new QTreeWidgetItem;
    item->setText(1, tr("Not listed here?"));
    item->setIcon(0, QIcon(":/images/unsupported_de.png"));
    item->setSelected(hasActiveDesktop == false);
    item->setData(0, Qt::UserRole, EUNSUPPORTED_DE_PAGE);
    ui->DEList->addTopLevelItem(item);
}

void MainWindow::fillPBIList()
{
    ui->PBIList->clear();    
    QProcess* loadPBIs = new QProcess();
    loadPBIs->setProcessChannelMode(QProcess::MergedChannels);
    loadPBIs->start(QString("pbi_info"), QStringList() << "-v");
    loadPBIs->waitForFinished(-1);

    QString line;
    QString name;
    QString version;
    QString website;
    QString icon;
    while ( loadPBIs->canReadLine() )
    {
        line = loadPBIs->readLine().simplified(); // line of text excluding '\n'

        if (line.indexOf("PBI Information for:" ) ==  0 )
        {
            if (name.length() && website.length())
            {
                QTreeWidgetItem* item = new QTreeWidgetItem;
                QPixmap ico(icon);
                item->setIcon(0, ico.scaled(42, 42));
                item->setText(1, name);
                item->setText(2, version);
                item->setData(0, Qt::UserRole, website);
                ui->PBIList->addTopLevelItem(item);
            }

            name.clear();
            icon.clear();
            version.clear();
            website.clear();
            icon.clear();
            continue;
        }

        if (line.indexOf("Name: " ) ==  0 )
              name = line.replace("Name: ", "");

        if ( line.indexOf("Version: ") == 0)
          version = line.replace("Version: ", "");


        if ( line.indexOf("Icon:") == 0)
                icon = line.replace("Icon: ", "");


        if ( line.indexOf("Website:") == 0)
              website = line.replace("Website: ", "");
    }//for all pbis
}
