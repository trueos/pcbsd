#include <QApplication>
#include <QCheckBox>
#include <QDesktopServices>
#include <QGraphicsDropShadowEffect>
#include <QPushButton>
#include <QTimer>
#include <QUrl>

#include "classes.h"
#include "clickedWidget.h"
#include "softmanager-main.h"

void PBM::slotHomeButtonClicked()
{
	displayCategories();
}

void PBM::slotBackButtonClicked()
{
	if ( browseStack->count() <= 1)
		return;
	
	QWidget *dWid = browseStack->currentWidget();
	browseStack->setCurrentIndex(browseStack->count() - 2);
	browseStack->removeWidget(dWid);
	delete dWid;

}

void PBM::updateHistory(QString type, QString name)
{
	histType.append(type);
	histName.append(name);
	if ( histType.count() >= 100) {
		histType.takeFirst();
		histName.takeFirst();
	}
}

void PBM::displayRepos()
{
	// Add all the repos to the menu
	setupRepoMenu();

	setEnabled(true);
}

// Show loading screen for software browser
void PBM::displaySoftwareLoading() {
	QWidget *myWidget = new QWidget();
	QGridLayout *layoutBrowser = new QGridLayout();
	QLabel *busy = new QLabel(tr("Please wait.. Loading repositories..."));
	busy->setAlignment(Qt::AlignHCenter);
	busy->setWordWrap(true);
	layoutBrowser->addWidget(busy, 0, 0);
	myWidget->setLayout(layoutBrowser);
        addBrowseWidget(myWidget);
}

void PBM::clearSoftwareBrowser() {
	int sCount = browseStack->count();
	for ( int i = 0; i < sCount; i++ )
	{
		QWidget *dWid = browseStack->widget(0);
		browseStack->removeWidget(browseStack->widget(0));
		delete dWid;
	} 
}

void PBM::slotPBIClicked(const QString id) {
	displayPBI(id);
}

void PBM::slotCatClicked(const QString id) {
	displayCat(id);
}

void PBM::slotDownloadClicked(const QString id) {
	int pbiNum = -1;

	int pTot = pbiAvail->count();
	for( int i = 0; i < pTot; i++)
		if ( pbiAvail->getName(i) == id && pbiAvail->getRepo(i) == pbiRepos->getID(curRepo)) {
			pbiNum = i;
			break;
		}
	if ( pbiNum == -1 )
		return;

	// Confirm we want to load this app
	if (QMessageBox::warning(this, tr("Install Software?"), tr("Do you want to download and install") + " " + pbiAvail->getName(pbiNum) + "?", tr("&OK"), tr("&Cancel"), 0, 0, 1) == 0 )
		scheduleNewPBIDownload(pbiAvail->getName(pbiNum), curRepo, pbiAvail->getRoot(pbiNum));
	
		
}

void PBM::displayCat(QString cat)
{
	bool found;

	// Set some globals so we can refresh later
	curPageType="CATHOME";
	curPageName="";
	curPageRepo=pbiRepos->getID(curRepo);

	// Init the new widgets
	QWidget *myWidget = new QWidget();
	QVBoxLayout *layoutBrowser = new QVBoxLayout();

	// Build the display of PBIs for this cat
	found=false;
	int pTot = pbiAvail->count();
	for( int i = 0; i < pTot; i++) {
		if ( pbiAvail->getCat(i).toLower() != cat.toLower() || pbiAvail->getRepo(i) != pbiRepos->getID(curRepo) )
			continue;

		found = true;
		// Create the first horizontal box with icon / name
		clickedWidget *pbiWidget = new clickedWidget(0, pbiAvail->getName(i));
		QGridLayout *grid1 = new QGridLayout();

		// Create the category icon
		QPixmap pIcon(pbiAvail->getIcon(i));
		QLabel *pIconLabel = new QLabel();
		pIconLabel->setPixmap(pIcon.scaled(48,48, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
		pIconLabel->setAlignment(Qt::AlignLeft);
		pIconLabel->setMinimumSize(48,48);
		pIconLabel->setMaximumSize(48,48);
		pIconLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
		grid1->addWidget(pIconLabel, 0, 0, 2, 1);

		// Create the title
		QLabel *pLabel = new QLabel(pbiAvail->getName(i));
		pLabel->setAlignment(Qt::AlignLeft);
		pLabel->setWordWrap(true);
		QFont titleFont;
		titleFont.setBold(true);
		pLabel->setFont(titleFont);
		pLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
		grid1->addWidget(pLabel, 0, 1);

		// Create the category description
		QString desc = pbiAvail->getDesc(i);
		if ( desc.size() > 100 ) {
                  desc.truncate(100);
		  desc += "...";
		}
		QLabel *pDesc = new QLabel(desc);
		pDesc->setAlignment(Qt::AlignLeft);
		pDesc->setWordWrap(true);
		pDesc->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
		grid1->addWidget(pDesc, 1, 1);


		// Make the category clickable
		pbiWidget->setCursor(Qt::PointingHandCursor);
		connect(pbiWidget, SIGNAL( clicked(QString) ), this, SLOT(slotPBIClicked(QString) ) );

		// Add the category to the main layout
		pbiWidget->setLayout(grid1);
		layoutBrowser->addWidget(pbiWidget);

		// Add a <hr> spacer
		QLabel *myLine = new QLabel();
	   	myLine->setFrameStyle(QFrame::HLine);	
		myLine->setFixedHeight(12);
		layoutBrowser->addWidget(myLine);
	}
		
	if ( found ) {
		layoutBrowser->setAlignment(Qt::AlignTop);
		myWidget->setLayout(layoutBrowser);
		addBrowseWidget(myWidget);
	} else {
		QLabel *nf = new QLabel(tr("No PBIs for category:") +" " + cat);
		nf->setAlignment(Qt::AlignHCenter);
		nf->setWordWrap(true);
		layoutBrowser->addWidget(nf);
		myWidget->setLayout(layoutBrowser);
		addBrowseWidget(myWidget);
	}
}

void PBM::displayPBI(QString pbi, QString rID)
{
	int pbiNum;
	QString pRepo;
	bool installed;
	bool autoUpdate = false;

	if (! rID.isEmpty() )
		pRepo = rID;
	else
		pRepo = pbiRepos->getID(curRepo);

	// Set some globals so we can refresh later
	curPageType="PBI";
	curPageName=pbi;
	curPageRepo=pRepo;
	curPBIName=pbi;

	// Now display the selected PBI
	// Init the new widgets
	QWidget *myWidget = new QWidget();
	QGridLayout *layoutBrowser = new QGridLayout();

	// Find the number for this PBI
	pbiNum = -1;

	int pTot = pbiAvail->count();
	for( int i = 0; i < pTot; i++)
		if ( pbiAvail->getName(i).toLower().remove(" ") == pbi.toLower().remove(" ") && pbiAvail->getRepo(i) == pRepo) {
			pbiNum = i;
			break;
		}
		
		
	// Didn't find this PBI in our available list
	if ( pbiNum == -1 ) {
		QLabel *nf = new QLabel(tr("Invalid PBI selected:") + " " + pbi);
		nf->setAlignment(Qt::AlignHCenter);
		nf->setWordWrap(true);
		layoutBrowser->addWidget(nf);
		myWidget->setLayout(layoutBrowser);
		addBrowseWidget(myWidget);
		return;
	}

	// See if this PBI is installed already
	installed = checkPBILoaded(pbi);

	bool isPending = false;

        // If this PBI is loaded, see of we have a update status
	if ( installed ) {
		PBI *pbiItem = getPBIFromName(pbi);
		if ( pbiItem ) {
			autoUpdate = pbiItem->getAutoUpdateStatus();
			if ( pbiItem->getWorkingStatus() == PBINEWDOWNLOAD)
				isPending = true;
		} else {
			autoUpdate = false;
		}
	}
	

	// Create the title / Icon
	QWidget *tWidget = new QWidget();
	QGridLayout *tLayout = new QGridLayout();

	// Create the icon
	QPixmap pIcon(pbiAvail->getIcon(pbiNum));
	QLabel *pIconLabel = new QLabel();
	pIconLabel->setPixmap(pIcon.scaled(48,48, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
	pIconLabel->setAlignment(Qt::AlignLeft);
	pIconLabel->setMinimumSize(48,48);
	pIconLabel->setMaximumSize(48,48);
	pIconLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	tLayout->addWidget(pIconLabel, 0, 0);

	clickedWidget *authLabel = new clickedWidget(0, pbiAvail->getName(pbiNum));
	QHBoxLayout *authLabelLayout = new QHBoxLayout;
	authLabelLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
	QLabel *aLabel = new QLabel();
        aLabel->setText(pbiAvail->getAuth(pbiNum));
	aLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);
	QFont aFont;
	aFont.setUnderline(true);
	aLabel->setFont(aFont);
 	authLabelLayout->addWidget(aLabel);
	authLabel->setLayout(authLabelLayout);
	tLayout->addWidget(authLabel, 1, 0, 1, 2);
	authLabel->setCursor(Qt::PointingHandCursor);
	connect(authLabel, SIGNAL( clicked(QString) ), this, SLOT(slotAuthorClicked(QString) ) );

	// Get the PBI name label
	QLabel *pLabel = new QLabel(pbiAvail->getName(pbiNum));
	pLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	pLabel->setWordWrap(true);
	QFont titleFont;
	titleFont.setPointSize(16);
	titleFont.setBold(true);
	pLabel->setFont(titleFont);
	pLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
	tLayout->addWidget(pLabel, 0, 1);

	QWidget *spaceWidget = new QWidget();
	spaceWidget->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
	tLayout->addWidget(spaceWidget, 0, 2, -1, 1);

	// Set the title widget layout, and add it to master layout
	tWidget->setLayout(tLayout);
	layoutBrowser->addWidget(tWidget, 0, 0);



	// Create the download widget
	QWidget *downWidget = new QWidget();
	QHBoxLayout *downLayout = new QHBoxLayout();
	QSpacerItem *dSpace = new QSpacerItem(10,30, QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
	downLayout->insertSpacerItem(0, dSpace);


	clickedWidget *pbiWidget = new clickedWidget(0, pbiAvail->getName(pbiNum));
	QGridLayout *dLayout = new QGridLayout();

	QLabel *dIcon = new QLabel();
	dIcon->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	if ( installed )
		dIcon->setPixmap(QPixmap(":/dialog-ok.png").scaled(48,48, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
	else
		dIcon->setPixmap(QPixmap(":/download.png").scaled(48,48, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
	dIcon->setMinimumSize(48,48);
	dIcon->setMaximumSize(48,48);
	dIcon->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	dLayout->addWidget(dIcon, 0, 0);

	QLabel *dLabel = new QLabel();
	if ( installed && isPending )
		dLabel->setText("<b>" + tr("Pending") + "</b>");
	else if ( installed )
		dLabel->setText("<b>" + tr("Installed") + "</b>");
	else
		dLabel->setText(tr("Download"));
	dLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	dLabel->setWordWrap(true);
	dLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
	dLayout->addWidget(dLabel, 0, 1);

	// Show the auto-updating checkbox if installed
	if ( installed && ! isPending) {
		QCheckBox *dCheck = new QCheckBox(tr("Automatic Updating"));
		dCheck->setChecked(autoUpdate);
		connect(dCheck, SIGNAL( clicked(bool) ), this, SLOT(slotAutoUpdateClicked(bool) ) );
		dLayout->addWidget(dCheck, 1, 0, 1, 2);
	}
	
	pbiWidget->setLayout(dLayout);

	// Create the spacer widget so that only the icon / text is clickable
	downLayout->addWidget(pbiWidget);
	downWidget->setLayout(downLayout);

	// Add it to the main layout browser
	layoutBrowser->addWidget(downWidget, 0, 1);	

	// Make the download clickable
	if ( ! installed ) {
		pbiWidget->setCursor(Qt::PointingHandCursor);
		connect(pbiWidget, SIGNAL( clicked(QString) ), this, SLOT(slotDownloadClicked(QString) ) );
	}


	// Add a <hr> spacer
	QLabel *myLine = new QLabel();
   	myLine->setFrameStyle(QFrame::HLine);	
	myLine->setFixedHeight(12);
	layoutBrowser->addWidget(myLine, 1, 0, 1, 2);

	// Display the version
	QLabel *pVer = new QLabel("<b>" + tr("Version:") + "</b> " + pbiAvail->getVer(pbiNum) + "");
	pVer->setAlignment(Qt::AlignLeft);
	pVer->setWordWrap(true);
	pVer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
	layoutBrowser->addWidget(pVer, 2, 0);


	// Set the root status
	QString needsRoot;
	if ( pbiAvail->getRoot(pbiNum) )
		needsRoot="YES";
	else
		needsRoot="NO";
	QLabel *pRoot = new QLabel("<b>" + tr("Requires Root:") + "</b> " + needsRoot );
	pRoot->setAlignment(Qt::AlignRight);
	pRoot->setWordWrap(true);
	pRoot->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
	layoutBrowser->addWidget(pRoot, 2, 1);

	// Display the architecture
	QLabel *pArch = new QLabel("<b>" + tr("Platform:") + "</b> " + pbiAvail->getArch(pbiNum));
	pArch->setAlignment(Qt::AlignLeft);
	pArch->setWordWrap(true);
	pArch->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
	layoutBrowser->addWidget(pArch, 3, 0);

	// Display the license
	QLabel *pLic = new QLabel("<b>" + tr("License:") + "</b> " + pbiAvail->getLic(pbiNum) );
	pLic->setAlignment(Qt::AlignRight);
	pLic->setWordWrap(true);
	pLic->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
	layoutBrowser->addWidget(pLic, 3, 1);
	
	// Display the Type
	QLabel *pType = new QLabel("<b>" + tr("Type:") + "</b> " + pbiAvail->getType(pbiNum) );
	pType->setAlignment(Qt::AlignLeft);
	pType->setWordWrap(true);
	pType->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
	layoutBrowser->addWidget(pType, 4, 0);

	// Display the Size
	QLabel *pSize = new QLabel("<b>" + tr("Size:") + "</b> " + pbiAvail->getSize(pbiNum) );
	pSize->setAlignment(Qt::AlignRight);
	pSize->setWordWrap(true);
	pSize->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
	layoutBrowser->addWidget(pSize, 4, 1);

	// Add a <hr> spacer
	QLabel *bottomLine = new QLabel();
   	bottomLine->setFrameStyle(QFrame::HLine);	
	bottomLine->setFixedHeight(12);
	layoutBrowser->addWidget(bottomLine, 5, 0, 1, 2);

	// Create the PBI description
	QTextBrowser *pDesc = new QTextBrowser();
	pDesc->setHtml(pbiAvail->getDesc(pbiNum));
        pDesc->setFrameStyle(QFrame::NoFrame);
        pDesc->setOpenExternalLinks(true);
        pDesc->setAlignment(Qt::AlignLeft);
        pDesc->setFixedHeight(120);
        pDesc->setBackgroundRole(QPalette::NoRole);
	pDesc->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
	layoutBrowser->addWidget(pDesc, 6, 0, 1, 2);


	// Now display the PBI on the top widget
	myWidget->setLayout(layoutBrowser);

	// Create a top-lvl widget
	QWidget *topWidget = new QWidget();
	QVBoxLayout *topLayout = new QVBoxLayout();
	topLayout->addWidget(myWidget);

	// Align us to the top of the widget
	topLayout->setAlignment(Qt::AlignTop);

	topWidget->setLayout(topLayout);
	addBrowseWidget(topWidget);
}

void PBM::displayCategories() {

	// Set some globals so we can refresh later
	curPageType="CATHOME";
	curPageName="";
	curPageRepo=pbiRepos->getID(curRepo);

	QWidget *myWidget = new QWidget();
	QVBoxLayout *layoutBrowser = new QVBoxLayout();

	// Display the recent apps list
	/*************************************************/
	layoutBrowser->addWidget(getLatestPBIWidget(curRepo));


	// Add a <hr> spacer
	QLabel *mSep = new QLabel();
   	mSep->setFrameStyle(QFrame::HLine);	
	mSep->setFixedHeight(12);
	layoutBrowser->addWidget(mSep);


	// Build the display of categories
	/*************************************************/
	QGroupBox *catGroup = new QGroupBox();
        catGroup->setTitle(tr("Application Categories"));
        QFont stdFont = catGroup->font();
        QFont boldFont = catGroup->font();
        boldFont.setBold(true);
        stdFont.setBold(false);
        catGroup->setFont(boldFont);
	QVBoxLayout *catLayout = new QVBoxLayout();

	int cTot = pbiCats->count();
	for( int i = 0; i < cTot; i++) {

		if ( pbiCats->getRepo(i) != pbiRepos->getID(curRepo) )
			continue;

		// Create the first horizontal box with icon / name
		clickedWidget *catWidget = new clickedWidget(0, pbiCats->getName(i));
		catWidget->setFont(stdFont);
		QGridLayout *grid1 = new QGridLayout();
		grid1->setAlignment(Qt::AlignTop);
		
		// Create the category icon
		QPixmap cIcon(pbiCats->getIcon(i));
		QLabel *cIconLabel = new QLabel();
		cIconLabel->setPixmap(cIcon.scaled(48,48, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
		cIconLabel->setAlignment(Qt::AlignLeft);
		cIconLabel->setMinimumSize(48,48);
		cIconLabel->setMaximumSize(48,48);
		cIconLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
		cIconLabel->setStyleSheet("background-color: transparent;");
		grid1->addWidget(cIconLabel, 0, 0, 2, 1);

		// Create the category title
		QLabel *cLabel = new QLabel(pbiCats->getName(i));
		cLabel->setAlignment(Qt::AlignLeft);
		cLabel->setWordWrap(true);
		QFont titleFont;
		titleFont.setBold(true);
		cLabel->setFont(titleFont);
		cLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
		cLabel->setStyleSheet("background-color: transparent;");
		grid1->addWidget(cLabel, 0, 1);

		// Create the category description
		QLabel *cDesc = new QLabel(pbiCats->getDesc(i));
		cDesc->setFont(stdFont);
		cDesc->setAlignment(Qt::AlignLeft);
		cDesc->setWordWrap(true);
		cDesc->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
		cDesc->setStyleSheet("background-color: transparent;");
		grid1->addWidget(cDesc, 1, 1);


		// Make the category clickable
		catWidget->setCursor(Qt::PointingHandCursor);
		connect(catWidget, SIGNAL( clicked(QString) ), this, SLOT(slotCatClicked(QString) ) );

		// Add the category to the main layout
		catWidget->setLayout(grid1);
		catLayout->addWidget(catWidget);

		// Add a <hr> spacer
		QLabel *myLine = new QLabel();
	   	myLine->setFrameStyle(QFrame::HLine);	
		myLine->setFixedHeight(12);
		myLine->setStyleSheet("background-color: transparent;");
		catLayout->addWidget(myLine);
	}

	// Add the category groupbox to our main layout
 	catGroup->setLayout(catLayout);	

	layoutBrowser->addWidget(catGroup);
	layoutBrowser->setAlignment(Qt::AlignTop);
        

	myWidget->setLayout(layoutBrowser);
	addBrowseWidget(myWidget);
}

void PBM::startSoftwareBrowser()
{

	// Load information from pbi_*
	pbiRepos = new Repo();
	pbiCats = new Category();
   	pbiAvail = new AvailPBI();

	setEnabled(false);

	loadRepos();
        loadTranslations();
	loadCategories();
	loadAvailPBIs();

	// Start displaying
	displayRepos();
	clearSoftwareBrowser();
	displayCategories();

    AllPBIsLabel->setText(tr("Available: ") + QString::number(pbiAvail->count()));
}

QString PBM::getTranslation(QString trepoid, QString tcat, QString key, QString otext) {
	
	if ( tcat == "Cat" )
	{
        	for( int i = 0; i < transCats.size(); i++) {
			if ( transCats.at(i).at(0) != trepoid)
				continue;
		  	if ( transCats.at(i).at(1) == key )	
				return transCats.at(i).at(2);
		}
	} else {
        	for( int i = 0; i < transApps.size(); i++) {
			if ( transApps.at(i).at(0) != trepoid)
				continue;
		  	if ( transApps.at(i).at(1) == key )	
				return transApps.at(i).at(2);
		}
	}
	
	return otext;
}

// Load up any translation scripts
void PBM::loadTranslations() {

	QString key, trans, tmp, tmp2, langkey; 

	// Load the translations for a repo
        int rTot = pbiRepos->count();
        for( int i = 0; i < rTot; i++) {

                qDebug() << "Loading translations for repoid:" << pbiRepos->getID(i);
                // Make sure we have a meta-file
                if ( pbiRepos->getMeta(i).isEmpty() || ! QFile::exists(pbiRepos->getMeta(i) ) )
                        continue;
		QFile mFile;
		if ( wardenDir.isEmpty() ) 
                  mFile.setFileName(pbiRepos->getMeta(i));
		else
                  mFile.setFileName(wardenDir + pbiRepos->getMeta(i));
                if ( ! mFile.open(QIODevice::ReadOnly | QIODevice::Text))
                        continue;

		if ( Lang.section("_", 0, 0).toUpper() == Lang.section("_", 1, 1).toUpper() )
			langkey=Lang.section("_", 0, 0);
		else
			langkey=Lang;

                // Read in the meta-file for categories
		QTextStream in(&mFile);
		in.setCodec("UTF-8");
                while ( !in.atEnd() ) {
                        tmp = in.readLine();
                        tmp2 = tmp;
                        if ( tmp.indexOf("Cat[" + langkey + "]") != 0 \
			&& tmp.indexOf("App[" + langkey + "]") != 0 )
                                continue;

			// Found a translation
			tmp.remove(0, tmp.indexOf("=") + 1 );
			key = tmp.section(";", 0, 0);
			trans = tmp.section(";", 1, 10);
			if ( tmp2.indexOf("Cat") == 0 )
			{
			  QStringList cattrans;
			  cattrans << pbiRepos->getID(i) << key << trans;
			  transCats.append(cattrans);
			} else {
			  QStringList apptrans;
			  apptrans << pbiRepos->getID(i) << key << trans;
			  transApps.append(apptrans);

			}
		}
		mFile.close();
	}

}

void PBM::loadAvailPBIs() {

	QString tmp, tmpicon;
	QString repoID;
        QStringList indexTmp;
	
	QString name, ver, date, arch, auth, url, lic, type, keys, desc, icon, cat, size;
	bool needsRoot = false;
	bool found;

	// Load the PBIs for each category
	int rTot = pbiRepos->count();
	for( int r = 0; r < rTot; r++) {
		indexTmp.clear();
		repoID = pbiRepos->getID(r);
		
		qDebug() << "Getting PBIs for repo:" << repoID;

		// Make sure we have a meta-file and index file
		if ( wardenDir.isEmpty() ) {
		  if ( pbiRepos->getMeta(r).isEmpty() || ! QFile::exists(pbiRepos->getMeta(r)) || pbiRepos->getIndex(r).isEmpty() || ! QFile::exists(pbiRepos->getIndex(r)) ) {
	  	    QMessageBox::warning( this, tr("Warning!"), tr("Missing meta-files for RepoID:") + " " + repoID + ". " + tr("This is normally due to the internet connection being offline or mis-configured.") );
		    continue;
		  }
                } else {
		  if ( pbiRepos->getMeta(r).isEmpty() || ! QFile::exists(wardenDir + pbiRepos->getMeta(r)) || pbiRepos->getIndex(r).isEmpty() || ! QFile::exists(wardenDir + pbiRepos->getIndex(r)) ) {
	  	    QMessageBox::warning( this, tr("Warning!"), tr("Missing meta-files for RepoID:") + " " + repoID + ". " + tr("This is normally due to the internet connection being offline or mis-configured.") );
		    continue;
		  }
		} // End of warden check

		// Start by loading index file into memory
		QFile iFile;
		if ( wardenDir.isEmpty() )
		  iFile.setFileName(pbiRepos->getIndex(r));
		else
		  iFile.setFileName(wardenDir + pbiRepos->getIndex(r));
		if ( ! iFile.open(QIODevice::ReadOnly | QIODevice::Text))
			continue;

		// Load the index lines into memory
		while ( !iFile.atEnd() ) {
			tmp = iFile.readLine();	

			// Only need current applications
			if ( tmp.indexOf(":current") == -1 )
				continue;
			indexTmp.append(tmp);
		}
		iFile.close();

		// Now lets read the meta-file for applications
		QFile mFile;
		if ( wardenDir.isEmpty() )
		  mFile.setFileName(pbiRepos->getMeta(r));
 		else
		  mFile.setFileName(wardenDir + pbiRepos->getMeta(r));
		if ( ! mFile.open(QIODevice::ReadOnly | QIODevice::Text))
			continue;
	
		// Read in the parts of the file want
		QStringList tmpMetaAppList;
		QTextStream in(&mFile);
		in.setCodec("UTF-8");
		while ( !in.atEnd() ) {
			tmp = in.readLine();
			if ( tmp.indexOf("App") != 0 )
				continue;
			if ( tmp.indexOf("App=") != 0 )
				continue;
			tmpMetaAppList << tmp;
		}
		mFile.close();

		// Now we sort the list case insenitively
		QMap<QString, QString> map;
		foreach (const QString &str, tmpMetaAppList)
     		  map.insert(str.toLower(), str);
 		tmpMetaAppList = map.values();

		// Now parse the meta-list
		for ( int i = 0 ; i < tmpMetaAppList.size(); ++i)
		{
			tmp = tmpMetaAppList.at(i);
			tmp.remove(0, tmp.indexOf("=") + 1 );

			name.clear();
			ver.clear();
			cat.clear();
			date.clear();
			arch.clear();
			auth.clear();
			url.clear();
			lic.clear();
			type.clear();
			keys.clear();
			desc.clear();
			icon.clear();
			size.clear();
			needsRoot=false;
			found = false;


			// Get the PBI information
			name = tmp.section(';', 0, 0);
			cat = tmp.section(';', 1, 1);
			icon = tmp.section(';', 2, 2);
			tmpicon = icon;
			tmpicon.remove(0, icon.lastIndexOf("."));
			if ( wardenDir.isEmpty() )
			  icon = "/var/db/pbi/repo-icons/" + pbiRepos->getMd5(r) + "-" + name + tmpicon;
			else
			  icon = wardenDir + "/var/db/pbi/repo-icons/" + pbiRepos->getMd5(r) + "-" + name + tmpicon;
			auth = tmp.section(';', 3, 3);
			url = tmp.section(';', 4, 4);
			lic = tmp.section(';', 5, 5);
			type = tmp.section(';', 6, 6);
			keys = tmp.section(';', 7, 7);
			desc = tmp.section(';', 8, 8);
			desc = getTranslation(pbiRepos->getID(r), "App", name, desc);

			if ( tmp.section(';', 9, 9) == "YES" ) 
				needsRoot=true;

			// Now lets check if this PBI is available
			for ( int i = 0; i < indexTmp.size(); ++i) {
			  if ( name.toUpper() == indexTmp.at(i).section(':', 0, 0).toUpper() ) {
			    if ( Arch == indexTmp.at(i).section(':', 1, 1) ) {
			      arch = indexTmp.at(i).section(':', 1, 1); 
			      ver = indexTmp.at(i).section(':', 2, 2); 
			      date = indexTmp.at(i).section(':', 4, 4); 
			      size = indexTmp.at(i).section(':', 9, 9);
			      if(!size.isEmpty() ){ 
				float num = size.toFloat()/1024; //in MB
				num = int(num*100 + 0.5)/100.0; //round to 2 decimel places
				size.setNum( num ).append(" MB"); 
			      }
			      found = true;
			      break;
		            }
			    // Check if we have a 32bit version on amd64
			    if ( Arch == "amd64" && indexTmp.at(i).section(':', 1, 1) == "i386" ) {
			      arch = indexTmp.at(i).section(':', 1, 1); 
			      ver = indexTmp.at(i).section(':', 2, 2); 
			      date = indexTmp.at(i).section(':', 4, 4); 
			      size = indexTmp.at(i).section(':', 9, 9);
			      if( !size.isEmpty() ){ 
				float num = size.toFloat()/1024; //in MB
				num = int(num*100 + 0.5)/100.0; //round to 2 decimel places
				size.setNum( num ).append(" MB"); 
			      }
			      found = true;
			    }
			  }
			} // End of index check
			    
			// We have a current PBI
			if ( found )
				pbiAvail->addPBI(name, ver, date, arch, auth, url, lic, type, keys, desc, icon, repoID, cat, needsRoot, size);
		}

	}
}

void PBM::loadCategories() {

	QString tmp;
	QString name, desc, icon, icontmp;

	// Load the categories for each repo
	int rTot = pbiRepos->count();
	for( int i = 0; i < rTot; i++) {

		qDebug() << "Adding Categories for repoID:" << pbiRepos->getID(i);
		// Make sure we have a meta-file
		if ( wardenDir.isEmpty() )
		  if ( pbiRepos->getMeta(i).isEmpty() || ! QFile::exists(pbiRepos->getMeta(i) ) )
	  	    continue;
		else
		  if ( pbiRepos->getMeta(i).isEmpty() || ! QFile::exists(wardenDir + pbiRepos->getMeta(i) ) )
	  	    continue;

		QFile mFile;
		if ( wardenDir.isEmpty() )
		  mFile.setFileName(pbiRepos->getMeta(i));
		else
		  mFile.setFileName(wardenDir + pbiRepos->getMeta(i));
		if ( ! mFile.open(QIODevice::ReadOnly | QIODevice::Text))
			continue;
	
		// Read in the meta-file for categories
		QTextStream in(&mFile);
		in.setCodec("UTF-8");
		while ( !in.atEnd() ) {
			tmp = in.readLine();	
			if ( tmp.indexOf("Cat") != 0 )
				continue;
			if ( tmp.indexOf("Cat=") != 0 )
				continue;
			
			name.clear();
			desc.clear();
			icon.clear();
			tmp.remove(0, tmp.indexOf("=") + 1 );
			name = tmp.section(';', 0, 0);
			icon = tmp.section(';', 1, 1);

			// Get the local filename for this icon
			icontmp = icon;
			icontmp.remove(0, icon.lastIndexOf("."));
			if ( wardenDir.isEmpty() )
			  icon = "/var/db/pbi/repo-icons/" + pbiRepos->getMd5(i) + "-" + name + icontmp;
			else
			  icon = wardenDir + "/var/db/pbi/repo-icons/" + pbiRepos->getMd5(i) + "-" + name + icontmp;
			desc = tmp.section(';', 2, 2).simplified();
			desc = getTranslation(pbiRepos->getID(i), "Cat", name, desc);

			pbiCats->addCat(name, desc, icon, pbiRepos->getID(i));
		}

	}
}

void PBM::loadRepos()
{
	QString tmp, tmp2;
	QString id, desc, index, mirrors, localIndex, localMeta, md5;
	curRepo = -1;

	qDebug() << "Adding Repos...";
	QProcess rpocmd;
        if ( wardenDir.isEmpty() )
	  rpocmd.start(QString("pbi_listrepo"), QStringList() );
        else
	  rpocmd.start(QString("warden"), QStringList() << "chroot" << wardenIP << "pbi_listrepo" );

	while ( rpocmd.state() != QProcess::NotRunning) {
		rpocmd.waitForFinished(50);
		QCoreApplication::processEvents();
	}

	while(rpocmd.canReadLine() ) {
		tmp = rpocmd.readLine().simplified();
		if ( tmp.indexOf("[ID]") != -1 || tmp.indexOf("------") != -1 )
			continue;
		
		// get the ID number
		id = tmp.section(" ", 0, 0);
		desc.clear();
		index.clear();
		mirrors.clear();
		localIndex.clear();
		localMeta.clear();
		md5.clear();

		// Get the details for this ID
		qDebug() << "Getting details for repoID: " << id;
		QProcess rpocmd2;
                if ( wardenDir.isEmpty() )
	  	  rpocmd2.start(QString("pbi_listrepo"), QStringList() << id );
                else
	  	  rpocmd2.start(QString("warden"), QStringList() << "chroot" << wardenIP << "pbi_listrepo " + id );
		rpocmd2.waitForFinished();
		while(rpocmd2.canReadLine() ) {
			tmp2 = rpocmd2.readLine().simplified();
			if ( tmp2.indexOf("Description: ") == 0) {
				desc = tmp2.replace("Description: ", "");
				continue;
			}
			if ( tmp2.indexOf("LocalIndex: ") == 0) {
				localIndex = tmp2.replace("LocalIndex: ", "");
				continue;
			}
			if ( tmp2.indexOf("LocalMeta: ") == 0) {
				localMeta = tmp2.replace("LocalMeta: ", "");
				continue;
			}
			if ( tmp2.indexOf("IndexURL: ") == 0) {
				index = tmp2.replace("IndexURL: ", "");
				continue;
			}
			if ( tmp2.indexOf("MD5: ") == 0) {
				md5 = tmp2.replace("MD5: ", "");
				continue;
			}
			if ( tmp2.indexOf("http") == 0 || \
			     tmp2.indexOf("PCBSDCDN") == 0 || \
			     tmp2.indexOf("ftp") == 0) {
				if ( mirrors.isEmpty() )
					mirrors = tmp2;
				else
					mirrors = mirrors + ";" + tmp2;
			}
		}

		// Now add this to the repos
		if ( ! id.isEmpty() && ! desc.isEmpty() && \
		     ! index.isEmpty() && ! mirrors.isEmpty() ) {
			pbiRepos->addRepo(id, desc, index, mirrors, localIndex, localMeta, md5);
			curRepo = 0;
		}
		
	}


}

void PBM::slotSearchAsTyped()
{
	slotSearchForKeyword(false);
}

void PBM::slotSearchClicked()
{
	slotSearchForKeyword(true);
}

void PBM::slotSearchForKeyword(bool buttonPress)
{
	QString needle = lineSearch->text();
	bool found;

	if ( needle.isEmpty() )
		return;

	// If this is just raw data being typed, wait until we have >2 
	// characters before searching
	if ( ! buttonPress )
		if ( needle.length() < 3 )
			return;
	
	// Init the new widgets
	QWidget *myWidget = new QWidget();
	QVBoxLayout *layoutBrowser = new QVBoxLayout();

	// Build the display of PBIs for this cat
	found=false;
	int pTot = pbiAvail->count();
	int nFound = 0;
	QList<int> nMatch, kMatch, dMatch;
	//Find and sort all the PBI's that fit the search
	for( int i = 0; i < pTot; i++) {
	        //Find a PBI for a match
		if ( pbiAvail->getRepo(i) != pbiRepos->getID(curRepo) )
			continue;
		if ( pbiAvail->getKeys(i).indexOf(needle, 0, Qt::CaseInsensitive ) == -1 && \
			pbiAvail->getDesc(i).indexOf(needle, 0, Qt::CaseInsensitive) == -1 && \
			pbiAvail->getName(i).indexOf(needle, 0, Qt::CaseInsensitive) == -1 )
			continue;

		nFound++;
		//Sort the PBI into the proper grouping
		if( pbiAvail->getName(i).toLower() == needle.toLower() ){ nMatch.prepend(i); } //exact match - place at top of list
		else if( pbiAvail->getName(i).toLower().contains(needle.toLower()) ){ nMatch.append(i); } //name contains search term
		else if( pbiAvail->getKeys(i).toLower().contains(needle.toLower()) ){ kMatch.append(i); } //keys contain search term
		else if( pbiAvail->getDesc(i).toLower().contains(needle.toLower()) ){ dMatch.append(i); } //description contains search term
		else{ nFound--; } //add better "else" handling later - do nothing now
		
	}
	//Display all the PBI's that match the search criteria
	for( int j=0; j<nFound; j++){
	   //Get the "i" value for the chosen PBI
	   int i;
	   int nn = nMatch.count(); 
	   int nk = kMatch.count();
	   if( j < nn ){ i = nMatch[j]; } //Display Name Matches First
	   else if ( j < (nn+nk) ){ i = kMatch[j-nn]; } //Display Key Matches Second
	   else{ i = dMatch[j-nn-nk]; } // Display Description Matches Last

		if(j==0){
		  found = true; //PBI's have been found -- only set this once
		  if(nn > 0){ //If there are Name Matches
		    //Add a header for "Best Matches"
		    QLabel *myLine = new QLabel( "<i><b>"+tr("Best Matches")+"</b></i>" );
		    myLine->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
		    layoutBrowser->addWidget(myLine);
		    // Add a <hr> spacer
		    QLabel *myLine2 = new QLabel();
	   	    myLine2->setFrameStyle(QFrame::HLine);	
		    myLine2->setFixedHeight(12);
		    layoutBrowser->addWidget(myLine2);
		  }
		}else if( nn!=0 && j==nn && j<nFound ){ //After the last Name match and more matches available
		  //Add a header for  "Other Matches"
		  QLabel *myLine = new QLabel( "<i><b>"+tr("Other Matches")+"</b></i>" );	
		  myLine->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
		  layoutBrowser->addWidget(myLine);
		  // Add a <hr> spacer
		  QLabel *myLine2 = new QLabel();
	   	  myLine2->setFrameStyle(QFrame::HLine);	
		  myLine2->setFixedHeight(12);
		  layoutBrowser->addWidget(myLine2);
		}
		
		// Create the first horizontal box with icon / name
		clickedWidget *pbiWidget = new clickedWidget(0, pbiAvail->getName(i));
		QGridLayout *grid1 = new QGridLayout();

		// Create the category icon
		QPixmap pIcon(pbiAvail->getIcon(i));
		QLabel *pIconLabel = new QLabel();
		pIconLabel->setPixmap(pIcon.scaled(48,48, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
		pIconLabel->setAlignment(Qt::AlignLeft);
		pIconLabel->setMinimumSize(48,48);
		pIconLabel->setMaximumSize(48,48);
		pIconLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
		grid1->addWidget(pIconLabel, 0, 0, 2, 1);

		// Create the title
		QLabel *pLabel = new QLabel(pbiAvail->getName(i));
		pLabel->setAlignment(Qt::AlignLeft);
		pLabel->setWordWrap(true);
		QFont titleFont;
		titleFont.setBold(true);
		pLabel->setFont(titleFont);
		pLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
		grid1->addWidget(pLabel, 0, 1);

		// Create the category description
		QString desc = pbiAvail->getDesc(i);
		if ( desc.size() > 100 ) {
                  desc.truncate(100);
		  desc += "...";
		}
		QLabel *pDesc = new QLabel(desc);
		pDesc->setAlignment(Qt::AlignLeft);
		pDesc->setWordWrap(true);
		pDesc->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
		grid1->addWidget(pDesc, 1, 1);


		// Make the category clickable
		pbiWidget->setCursor(Qt::PointingHandCursor);
		connect(pbiWidget, SIGNAL( clicked(QString) ), this, SLOT(slotPBIClicked(QString) ) );

		// Add the category to the main layout
		pbiWidget->setLayout(grid1);
		layoutBrowser->addWidget(pbiWidget);

		// Add a <hr> spacer
		QLabel *myLine = new QLabel();
	   	myLine->setFrameStyle(QFrame::HLine);	
		myLine->setFixedHeight(12);
		layoutBrowser->addWidget(myLine);
	}
		
	if ( found ) {
		layoutBrowser->setAlignment(Qt::AlignTop);
		myWidget->setLayout(layoutBrowser);
		addBrowseWidget(myWidget);
	} else {
		QLabel *nf = new QLabel(tr("No applications found!"));
		nf->setAlignment(Qt::AlignHCenter);
		nf->setWordWrap(true);
		layoutBrowser->addWidget(nf);
		myWidget->setLayout(layoutBrowser);
		addBrowseWidget(myWidget);
	}
}

void PBM::slotAutoUpdateClicked(bool status) 
{
	QString arg;
	if ( status )
		arg = "--enable-auto";
	else
		arg = "--disable-auto";


	PBI *pbiItem = getPBIFromName(curPBIName);
	if ( !pbiItem )
		return;

	QProcess pbicmd;
	setEnabled(false);
        QString sCmd;

        if (wardenDir.isEmpty() ) {
          if ( pbiItem->getInstallBy() == "root" || pbiItem->getInstallBy() != RealUserName)
  	     sCmd="pc-su \"pbi_update " + arg + " " + pbiItem->getNameCLI() + "\"";
	  else
	     sCmd="pbi_update " + arg + " " + pbiItem->getNameCLI();
        } else
	  sCmd=getPBICmd("pbi_update " + arg + " " + pbiItem->getNameCLI());


	system(sCmd.toLatin1());
	setEnabled(true);
	pbiItem->setAutoUpdateStatus(status);

}

// Return a qstringlist of the latest PBIs
QStringList PBM::getLatestPBIs(int lCount, QString repoID)
{
	QStringList latest, tmpList;

	// Get the list of dates
	int pTot = pbiAvail->count();
	for( int i = 0; i < pTot; i++) {
		if ( pbiAvail->getRepo(i) != repoID )
                        continue;

		tmpList << pbiAvail->getDate(i);
	}

	// Sort the list in descending order
	qSort(tmpList.begin(), tmpList.end(), qGreater<QString>());


	// Get the top lCount results
	for ( int i = tmpList.length() ; i >= lCount; i-- )
		tmpList.removeAt(i);

	// Now return the PBI names
	for ( int z = 0; z < tmpList.length() ; z++ ) {

		pTot = pbiAvail->count();
		for( int i = 0; i < pTot; i++) {
			if ( pbiAvail->getRepo(i) != repoID )
               	         continue;
			if ( pbiAvail->getDate(i) != tmpList.at(z) )
               	         continue;

			latest << pbiAvail->getName(i);
		}
	}

	return latest;
}

QWidget* PBM::getLatestPBIWidget(int curRepo)
{
	QGroupBox *latestWidget = new QGroupBox();
        latestWidget->setTitle(tr("Latest Releases"));
        QFont stdFont = latestWidget->font();
        QFont boldFont = latestWidget->font();
        boldFont.setBold(true);
        stdFont.setBold(false);
        latestWidget->setFont(boldFont);
	QHBoxLayout *latestLayout = new QHBoxLayout();

	QStringList latestApps = getLatestPBIs(4, pbiRepos->getID(curRepo));
	for ( int i = 0; i < latestApps.length(); i++) {
		clickedWidget *lWidget = new clickedWidget(0, latestApps.at(i));
		lWidget->setFont(stdFont);
		QVBoxLayout *iLayout = new QVBoxLayout();
		iLayout->setAlignment(Qt::AlignCenter);
		lWidget->setFont(stdFont);

		// Find the correct PBI we are working on
		for ( int z = 0; z < pbiAvail->count(); z++ ) {
			if ( latestApps.at(i) != pbiAvail->getName(z) || pbiAvail->getRepo(z) != pbiRepos->getID(curRepo))
				continue;

			QWidget *sWidget = new QWidget();
			QGridLayout *sLayout = new QGridLayout();
			sLayout->setAlignment(Qt::AlignCenter);

			QPixmap pIcon(pbiAvail->getIcon(z));
			QLabel *pIconLabel = new QLabel();
			pIconLabel->setPixmap(pIcon.scaled(32,32, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
			pIconLabel->setMinimumSize(32,32);
			pIconLabel->setMaximumSize(32,32);
			pIconLabel->setAlignment(Qt::AlignCenter);
			pIconLabel->setToolTip(pbiAvail->getName(z) + " " + pbiAvail->getVer(z));
		        pIconLabel->setStyleSheet("background-color: transparent;");
			sLayout->addWidget(pIconLabel, 0, 0);
		        sWidget->setLayout(sLayout);
		        sWidget->setStyleSheet("background-color: transparent;");
			iLayout->addWidget(sWidget);

			QLabel *pLabel = new QLabel();
			pLabel->setFont(stdFont);
			pLabel->setAlignment(Qt::AlignCenter);
			pLabel->setText(pbiAvail->getName(z) + " " + pbiAvail->getVer(z));
		        pLabel->setStyleSheet("background-color: transparent;");
			iLayout->addWidget(pLabel);
		}
		lWidget->setLayout(iLayout);
		latestLayout->addWidget(lWidget);
		lWidget->setCursor(Qt::PointingHandCursor);
		connect(lWidget, SIGNAL( clicked(QString) ), this, SLOT(slotPBIClicked(QString) ) );
	}

	latestWidget->setLayout(latestLayout);
	latestWidget->setAlignment(Qt::AlignTop);
	return latestWidget;
}

void PBM::setupRepoMenu()
{
	repomenu->clear();
	repomenu->addAction(tr("Add Repository"), this, SLOT(slotAddButtonClicked()));
	repomenu->addAction(tr("Configure Repository"), this, SLOT(slotConfigButtonClicked()));
	repomenu->addSeparator();

        QActionGroup *actGroup = new QActionGroup(this);

	int rTot = pbiRepos->count();
	for( int i = 0; i < rTot; i++) {
		QAction *act = repomenu->addAction(pbiRepos->getDesc(i));
		act->setData(i);
		act->setCheckable(true);

		if ( curRepo != -1 && curRepo == i )
			act->setChecked(true);
		else
			act->setChecked(false);

		actGroup->addAction(act);		
	}

	connect(actGroup, SIGNAL( triggered(QAction*) ), this, SLOT(slotRepoChanged(QAction *) ) );
	
}

void PBM::slotRepoChanged(QAction *curAction)
{
	// Save the current repo
        bool ok;
	curAction->data().toInt(&ok);
	if ( ok )
		curRepo=curAction->data().toInt(&ok);

	// Clear the history
	clearSoftwareBrowser();

	// Display the landing page for this repo
	displayCategories();
}


void PBM::slotAuthorClicked(const QString id)
{
        int pbiNum = -1;

        int pTot = pbiAvail->count();
        for( int i = 0; i < pTot; i++)
                if ( pbiAvail->getName(i) == id && pbiAvail->getRepo(i) == pbiRepos->getID(curRepo)) {
                        pbiNum = i;
                        break;
                }
        if ( pbiNum == -1 )
                return;

	qDebug() << "Browser Launch:" << pbiAvail->getURL(pbiNum);
	if ( ! QDesktopServices::openUrl(QUrl(pbiAvail->getURL(pbiNum))))
		qDebug() << "Failed to launch browser for:" << pbiAvail->getURL(pbiNum);
}

// Function which adds the widget to a scroll area, and update the browser stack
void PBM::addBrowseWidget(QWidget *bwid)
{
        QScrollArea *scrollAreaPBI = new QScrollArea();
	//scrollAreaPBI->setAlignment(Qt::AlignHCenter);
	bwid->setMinimumSize(570, 300);
	//bwid->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
	scrollAreaPBI->setWidget(bwid);
        scrollAreaPBI->setWidgetResizable(true);

	// Only keep 30 pages in history
	if ( browseStack->count() > 30 )
		if ( browseStack->widget(0) != 0)
			browseStack->removeWidget(browseStack->widget(0));

	browseStack->setCurrentIndex(browseStack->addWidget(scrollAreaPBI));
	
}
