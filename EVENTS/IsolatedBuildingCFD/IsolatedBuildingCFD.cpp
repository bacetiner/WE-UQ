/* *****************************************************************************
Copyright (c) 2016-2017, The Regents of the University of California (Regents).
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are those
of the authors and should not be interpreted as representing official policies,
either expressed or implied, of the FreeBSD Project.

REGENTS SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
THE SOFTWARE AND ACCOMPANYING DOCUMENTATION, IF ANY, PROVIDED HEREUNDER IS
PROVIDED "AS IS". REGENTS HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT,
UPDATES, ENHANCEMENTS, OR MODIFICATIONS.

*************************************************************************** */

// Written: abiy

#include "IsolatedBuildingCFD.h"
#include "QVector3D"
#include "SnappyHexMeshWidget.h"
#include "BoundaryConditionsWidget.h"
#include "TurbulenceModelingWidget.h"
#include "SimCenterVTKRenderingWidget.h"
#include "NumericalSetupWidget.h"
#include "WindCharacteristicsWidget.h"
#include "ResultMonitoringWidget.h"
#include <qcustomplot.h>
#include <QPushButton>
#include <QScrollArea>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QLabel>
#include <QLineEdit>
#include <QDebug>
#include <QFileDialog>
#include <QPushButton>
#include <SectionTitle.h>
#include <QFileInfo>
#include <QMovie>
#include <QPixmap>
#include <QIcon>
#include <RandomVariablesContainer.h>
#include <QRadioButton>
#include <QButtonGroup>
#include <QComboBox>
#include <QSpinBox>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QVector>
#include <LineEditRV.h>
#include <QDebug>
#include <QOpenGLWidget>
#include <SimCenterPreferences.h>
#include <GeneralInformationWidget.h>
#include <QProcess>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QTextEdit>
#include <QFormLayout>


IsolatedBuildingCFD::IsolatedBuildingCFD(RandomVariablesContainer *theRandomVariableIW, QWidget *parent)
    : SimCenterAppWidget(parent), theRandomVariablesContainer(theRandomVariableIW)
{

}

bool IsolatedBuildingCFD::initialize()
{
    femSpecific = 0;
    const int windowWidth = 850;

    mainWindowLayout = new QHBoxLayout();

    inputWindowLayout = new QVBoxLayout();
    inputWindowGroup = new QGroupBox();

    visWindowLayout = new QVBoxLayout();
    visWindowGroup = new QGroupBox();

    QTabWidget *inputTab = new QTabWidget(this);

    QWidget *generalWidget = new QWidget();
    QWidget *geometryWidget = new QWidget();
    QWidget *meshWidget = new QWidget();
    QWidget *BCWidget = new QWidget();
    QWidget *numericalSetupWidget = new QWidget();
    QWidget *monitoringWidget = new QWidget();
    QWidget *resultsWidget = new QWidget();

    QVBoxLayout* generalLayout  = new QVBoxLayout();
    QVBoxLayout* geometryLayout  = new QVBoxLayout();
    QVBoxLayout* meshLayout  = new QVBoxLayout();
    QVBoxLayout* BCLayout  = new QVBoxLayout();
    QVBoxLayout* numericalSetupLayout  = new QVBoxLayout();
    QVBoxLayout* monitoringLayout  = new QVBoxLayout();
    QVBoxLayout* resultsLayout  = new QVBoxLayout();


    generalDescriptionGroup = new QGroupBox("Modeling Processes");
    generalDescriptionLayout = new QHBoxLayout();

    caseDirectoryGroup = new QGroupBox("Case Directory");
    caseDirectoryLayout = new QGridLayout();

    unitSystemGroup = new QGroupBox("Units");
    unitSystemLayout = new QGridLayout();

    openFoamVersionGroup = new QGroupBox("OpenFOAM Version");
    openFoamVersionLayout = new QGridLayout();

    dimAndScaleGroup = new QGroupBox("Dimentions and Scale");
    dimAndScaleLayout = new QGridLayout();

    buildingAndDomainInformationGroup = new QWidget();
    buildingAndDomainInformationLayout = new QGridLayout();

    coordinateSystemGroup = new QGroupBox("Coordinate System");
    coordinateSystemLayout = new QGridLayout();

    buildingInformationGroup = new QGroupBox("Building Geometry");
    buildingInformationLayout = new QGridLayout();

    domainInformationGroup = new QGroupBox("Domain Dimensions");
    domainInformationLayout = new QGridLayout();

    cfdResultsGroup = new QGroupBox("CFD Results", this);
    cfdResultsLayout = new QGridLayout();

    QLabel *buildingShapeLabel = new QLabel("Building Shape:");
    buildingShape = new QComboBox();
    buildingShape->addItem("Generic");
    buildingShape->addItem("ImportSTL");

    theBuildingButton = new QPushButton();
    QPixmap pixmapFlat(":/Resources/IsolatedBuildingCFD/buildingGeometry.png");

    theBuildingButton->setIcon(pixmapFlat);
    theBuildingButton->setIconSize(pixmapFlat.rect().size()*.30);
    theBuildingButton->setFixedSize(pixmapFlat.rect().size()*.30);
    buildingInformationLayout->addWidget(theBuildingButton, 0, 0, 5, 1, Qt::AlignVCenter);

    QLabel *buildingWidthLabel = new QLabel("Building Width:");
    buildingWidthWidget = new QLineEdit();
    buildingWidthWidget->setText("45.72");

    QLabel *buildingDepthLabel = new QLabel("Building Depth:");
    buildingDepthWidget = new QLineEdit();
    buildingDepthWidget->setText("30.48");

    QLabel *buildingHeightLabel = new QLabel("Building Height:");
    buildingHeightWidget = new QLineEdit();
    buildingHeightWidget->setText("182.88");

    QLabel *geometricScaleLabel = new QLabel("Geometric Scale:");
    geometricScaleWidget = new QLineEdit();
    geometricScaleWidget->setText("400.0");

    QLabel *windDirectionLabel = new QLabel("Wind Direction:");
    //QLabel *angleUnit = new QLabel("degrees");
    windDirectionWidget = new QSpinBox;
    windDirectionWidget->setRange(0, 90);
    windDirectionWidget->setSingleStep(10);
    windDirectionWidget->setValue(0);

    QLabel *domainLengthLabel = new QLabel("Domain Length (X-axis):");
    domainLengthWidget = new QLineEdit();
    domainLengthWidget->setText("20");

    QLabel *domainWidthLabel = new QLabel("Domain Width (Y-axis):");
    domainWidthWidget = new QLineEdit();
    domainWidthWidget->setText("10");

    QLabel *domainHeightLabel = new QLabel("Domain Height (Z-axis):");
    domainHeightWidget = new QLineEdit();
    domainHeightWidget->setText("6");

    QLabel *fetchLengthLabel = new QLabel("Fetch Length (X-axis):");
    fetchLengthWidget = new QLineEdit();
    fetchLengthWidget->setText("5");

    QLabel *useCOSTDimLabel = new QLabel("COST Recommendation:");
    useCOSTDimWidget = new QCheckBox();
    useCOSTDimWidget->setChecked(true);

    QLabel *normalizationTypeLabel = new QLabel("Input Dimension Normalization:");
    normalizationTypeWidget = new QComboBox();
    normalizationTypeWidget->addItem("Relative");
    normalizationTypeWidget->addItem("Absolute");

    QLabel *originOptionsLabel = new QLabel("Location of Absolute Origin: ");
    QLabel *originCoordinateLabel = new QLabel("Coordinate: ");
    QLabel *originXLabel = new QLabel("X<sub>o</sub>:");
    QLabel *originYLabel = new QLabel("Y<sub>o</sub>:");
    QLabel *originZLabel = new QLabel("Z<sub>o</sub>:");

    originOptions = new QComboBox();
    originOptions->addItem("Building Bottom Center");
    originOptions->addItem("Domain Bottom Left Corner");
    originOptions->addItem("Custom");

    originXWidget = new QLineEdit();
    originYWidget = new QLineEdit();
    originZWidget = new QLineEdit();

    originXWidget->setText("0");
    originYWidget->setText("0");
    originZWidget->setText("0");

    originXWidget->setEnabled(false);
    originYWidget->setEnabled(false);
    originZWidget->setEnabled(false);

    QLabel *casePathLabel = new QLabel("Path: ");
    QPushButton* browseCaseDirectoryButton  = new QPushButton("Browse");

    caseDirectoryPathWidget = new QLineEdit();
    QString currentAppDir = QCoreApplication::applicationDirPath();

    QDir workingDir(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
    if (!workingDir.exists())
        workingDir.mkpath(".");

    QString workingDirPath = workingDir.filePath(QCoreApplication::applicationName() + QDir::separator()
                                                 + "LocalWorkDir" + QDir::separator()
                                                 + "IsolatedBuildingCFD");

    if (!workingDir.exists(workingDirPath))
        workingDir.mkpath(workingDirPath);

    caseDirectoryPathWidget->setText(workingDirPath);


    QLabel *massUnitLabel = new QLabel("Mass :");
    QLabel *lengthUnitLabel = new QLabel("Length :");
    QLabel *timeUnitLabel = new QLabel("Time :");
    QLabel *angleUnitLabel = new QLabel("Angle :");

    massUnit = new QComboBox ();
    massUnit->addItem("kg");
    massUnit->setEnabled(false);

    lengthUnit = new QComboBox ();
    lengthUnit->addItem("m");
    lengthUnit->setEnabled(false);

    timeUnit = new QComboBox ();
    timeUnit->addItem("s");
    timeUnit->setEnabled(false);

    angleUnit = new QComboBox ();
    angleUnit->addItem("degree");
    angleUnit->setEnabled(false);


    QLabel *openFoamVersionLabel = new QLabel("Version of OpenFOAM Distribution: ");

    openFoamVersion = new QComboBox ();
    openFoamVersion->addItem("7");
    openFoamVersion->addItem("9");
    openFoamVersion->addItem("10");
    openFoamVersion->setCurrentIndex(1);


    QLabel *domainSizeNoteLabel = new QLabel("**Normalization is done relative to the building height**");


    QTextEdit *modelingProcedureText = new QTextEdit ();
    modelingProcedureText->setReadOnly(true);

    QTextDocument* document = modelingProcedureText->document();
    QTextCursor* cursor = new QTextCursor(document);

    cursor->insertText("A CFD (virtual wind tunnel) model for a generic rectangularly shaped building to perform wind load simulation. The modeling procedure involves: ");

    QTextListFormat listFormat;
    listFormat.setStyle(QTextListFormat::ListDecimal);
    cursor->insertList(listFormat);

    cursor->insertText(" Define the geometry\n");
    cursor->insertText(" Generate mesh\n");
    cursor->insertText(" Define boundary conditions\n");
    cursor->insertText(" Specify numerical setup\n");
    cursor->insertText(" Monitor wind loads\n");
    cursor->insertText(" Run simulation\n");
    cursor->insertText(" Post-process results");


    generalDescriptionLayout->addWidget(modelingProcedureText);

    caseDirectoryLayout->addWidget(casePathLabel, 0, 0);
    caseDirectoryLayout->addWidget(caseDirectoryPathWidget, 0, 1);
    caseDirectoryLayout->addWidget(browseCaseDirectoryButton, 0, 2);

    openFoamVersionLayout->addWidget(openFoamVersionLabel, 0, 0);
    openFoamVersionLayout->addWidget(openFoamVersion, 0, 1);
    openFoamVersionLayout->setAlignment(Qt::AlignLeft);


    unitSystemLayout->addWidget(massUnitLabel, 0, 0);
    unitSystemLayout->addWidget(lengthUnitLabel, 1, 0);
    unitSystemLayout->addWidget(timeUnitLabel, 2, 0);
    unitSystemLayout->addWidget(angleUnitLabel, 3, 0);

    unitSystemLayout->addWidget(massUnit, 0, 1);
    unitSystemLayout->addWidget(lengthUnit, 1, 1);
    unitSystemLayout->addWidget(timeUnit, 2, 1);
    unitSystemLayout->addWidget(angleUnit, 3, 1);
    unitSystemLayout->setAlignment(Qt::AlignLeft);

    dimAndScaleLayout->addWidget(normalizationTypeLabel, 0, 0);
    dimAndScaleLayout->addWidget(normalizationTypeWidget, 0, 1);

    dimAndScaleLayout->addWidget(geometricScaleLabel, 0, 2, Qt::AlignRight);
    dimAndScaleLayout->addWidget(geometricScaleWidget, 0, 3, Qt::AlignLeft);

    buildingInformationLayout->addWidget(buildingShapeLabel, 0, 1);
    buildingInformationLayout->addWidget(buildingShape,0, 3);

    buildingInformationLayout->addWidget(buildingWidthLabel,1,1);
    buildingInformationLayout->addWidget(buildingWidthWidget,1,3);

    buildingInformationLayout->addWidget(buildingDepthLabel,2,1);
    buildingInformationLayout->addWidget(buildingDepthWidget,2,3);

    buildingInformationLayout->addWidget(buildingHeightLabel,3,1);
    buildingInformationLayout->addWidget(buildingHeightWidget,3,3);

    buildingInformationLayout->addWidget(windDirectionLabel, 4, 1);
    buildingInformationLayout->addWidget(windDirectionWidget, 4, 3);
    //buildingInformationLayout->addWidget(angleUnit, 3, 4);


    domainInformationLayout->addWidget(domainLengthLabel,0,0);
    domainInformationLayout->addWidget(domainLengthWidget,0,1);

    domainInformationLayout->addWidget(domainWidthLabel,1,0);
    domainInformationLayout->addWidget(domainWidthWidget,1,1);

    domainInformationLayout->addWidget(domainHeightLabel,2,0);
    domainInformationLayout->addWidget(domainHeightWidget,2,1);

    domainInformationLayout->addWidget(fetchLengthLabel,3,0);
    domainInformationLayout->addWidget(fetchLengthWidget,3,1);

    domainInformationLayout->addWidget(useCOSTDimLabel,5,0);
    domainInformationLayout->addWidget(useCOSTDimWidget,5,1);

    buildingAndDomainInformationLayout->addWidget(buildingInformationGroup, 0, 0);
    buildingAndDomainInformationLayout->addWidget(domainInformationGroup, 0, 1);
    buildingAndDomainInformationLayout->addWidget(domainSizeNoteLabel, 1, 0,1,2, Qt::AlignRight);

    coordinateSystemLayout->addWidget(originOptionsLabel,0,0);
    coordinateSystemLayout->addWidget(originOptions,0,1);
    coordinateSystemLayout->addWidget(originCoordinateLabel,1,0, Qt::AlignRight);
    coordinateSystemLayout->addWidget(originXLabel,1,1, Qt::AlignRight);
    coordinateSystemLayout->addWidget(originXWidget,1,2, Qt::AlignLeft);
    coordinateSystemLayout->addWidget(originYLabel,1,3, Qt::AlignLeft);
    coordinateSystemLayout->addWidget(originYWidget,1,4, Qt::AlignLeft);
    coordinateSystemLayout->addWidget(originZLabel,1,5, Qt::AlignLeft);
    coordinateSystemLayout->addWidget(originZWidget,1,6, Qt::AlignLeft);

    generalDescriptionGroup->setLayout(generalDescriptionLayout);
    openFoamVersionGroup->setLayout(openFoamVersionLayout);
    unitSystemGroup->setLayout(unitSystemLayout);
    dimAndScaleGroup->setLayout(dimAndScaleLayout);
    caseDirectoryGroup->setLayout(caseDirectoryLayout);
    buildingInformationGroup->setLayout(buildingInformationLayout);
    domainInformationGroup->setLayout(domainInformationLayout);
    buildingAndDomainInformationGroup->setLayout(buildingAndDomainInformationLayout);
    coordinateSystemGroup->setLayout(coordinateSystemLayout);
    cfdResultsGroup->setLayout(cfdResultsLayout);

    generalWidget->setLayout(generalLayout);
    geometryWidget->setLayout(geometryLayout);
    meshWidget->setLayout(meshLayout);
    BCWidget->setLayout(BCLayout);
    numericalSetupWidget->setLayout(numericalSetupLayout);
    monitoringWidget->setLayout(monitoringLayout);
    resultsWidget->setLayout(resultsLayout);

    //Controls for wind characteristics setup
    windCharacteristics = new WindCharacteristicsWidget(this);

    //Controls for snappyHexMesh
    snappyHexMesh = new SnappyHexMeshWidget(this);

    //Controle for turbulence modeling
    turbulenceModeling = new TurbulenceModelingWidget(this);

    //Controls for boundary conditions
    boundaryConditions = new BoundaryConditionsWidget(this);

    //Controls for numerical setup
    numericalSetup = new NumericalSetupWidget(this);

    //Add result monitoring widget
    resultMonitoring = new ResultMonitoringWidget(this);

    //Populate each tab
    generalLayout->addWidget(generalDescriptionGroup);
    generalLayout->addWidget(caseDirectoryGroup);
    generalLayout->addWidget(openFoamVersionGroup);
    generalLayout->addWidget(unitSystemGroup);
    generalLayout->addStretch();

    geometryLayout->addWidget(dimAndScaleGroup);
    geometryLayout->addWidget(buildingAndDomainInformationGroup);
    geometryLayout->addWidget(coordinateSystemGroup);
    geometryLayout->addStretch();

    meshLayout->addWidget(snappyHexMesh);
    meshLayout->addStretch();

    BCLayout->addWidget(windCharacteristics);
    BCLayout->addWidget(boundaryConditions);
    BCLayout->addStretch();

    numericalSetupLayout->addWidget(turbulenceModeling);
    numericalSetupLayout->addWidget(numericalSetup);
    numericalSetupLayout->addStretch();

    monitoringLayout->addWidget(resultMonitoring);
    monitoringLayout->addStretch();

    resultsLayout->addWidget(cfdResultsGroup);
    resultsLayout->addStretch();

    inputTab->addTab(generalWidget, "Start");
    inputTab->addTab(geometryWidget, "Geometry");
    inputTab->addTab(meshWidget, "Mesh");
    inputTab->addTab(BCWidget, "Boundary Conditions");
    inputTab->addTab(numericalSetupWidget, "Numerical Setup");
    inputTab->addTab(monitoringWidget, "Monitoring");
    inputTab->addTab(resultsWidget, "Results");

    inputWindowLayout->addWidget(inputTab);
    inputWindowGroup->setLayout(inputWindowLayout);
    inputWindowGroup->setMaximumWidth(windowWidth - 100);

    mainWindowLayout->addWidget(inputWindowGroup);

    plotWindProfiles = new QPushButton("Plot Wind Profiles");
    plotWindLoads = new QPushButton("Plot Wind Loads");

    cfdResultsLayout->addWidget(plotWindProfiles);
    cfdResultsLayout->addWidget(plotWindLoads);

    connect(plotWindProfiles, SIGNAL(clicked()), this, SLOT(onShowResultsClicked()));
    connect(browseCaseDirectoryButton, SIGNAL(clicked()), this, SLOT(onBrowseCaseDirectoryButtonClicked()));
    connect(originOptions, SIGNAL(currentIndexChanged(QString)), this, SLOT(originChanged(QString)));
    connect(useCOSTDimWidget, SIGNAL(stateChanged(int)), this, SLOT(useCOSTOptionChecked(int)));
    connect(buildingShape, SIGNAL(currentIndexChanged(QString)), this, SLOT(buildingShapeChanged(QString)));

    //=====================================================
    // Sync with general information tab
    //=====================================================

    GeneralInformationWidget *theGI = GeneralInformationWidget::getInstance();


    connect(theGI, &GeneralInformationWidget::buildingDimensionsChanged ,
            [=] (double width, double depth, double area) {
                buildingWidthWidget->setText(QString::number(convertToMeter(width, theGI->getLengthUnit())));
                buildingDepthWidget->setText(QString::number(convertToMeter(depth, theGI->getLengthUnit())));
            });

    connect(theGI, &GeneralInformationWidget::numStoriesOrHeightChanged ,
            [=] (int nFloors, double height) {
                buildingHeightWidget->setText(QString::number(convertToMeter(height, theGI->getLengthUnit())));
            });

    //Disable editing in the event section
    buildingWidthWidget->setEnabled(false);
    buildingHeightWidget->setEnabled(false);
    buildingDepthWidget->setEnabled(false);

    //=====================================================
    // Setup the case directory
    //=====================================================

    if(!isCaseConfigured())
    {
        setupCase();
    }

    if (!isMeshed())
    {
        snappyHexMesh->onRunBlockMeshClicked();
    }

    //Read all the case data from const/simCenter
    readCaseData();

    //=====================================================
    // Setup the visulalization window
    //=====================================================

    visWindowGroup->setLayout(visWindowLayout);
    mainWindowLayout->addWidget(visWindowGroup);

    visWidget = new SimCenterVTKRenderingWidget(this);

    visWindowLayout->addWidget(visWidget);

    this->setLayout(mainWindowLayout);

    caseInitialized = true;

    theGI->setLengthUnit("m");
    theGI->setNumStoriesAndHeight(numberOfFloors(), buildingHeight());
    theGI->setBuildingDimensions(buildingWidth(), buildingDepth(), buildingWidth()*buildingDepth());

    return true;
}


IsolatedBuildingCFD::~IsolatedBuildingCFD()
{

}

void IsolatedBuildingCFD::updateJSON()
{
    //Write it to JSON becase it is needed for the mesh generation before the final simulation is run.
    //In future only one JSON file in temp.SimCenter directory might be enough
    QString inputFilePath = caseDir() + QDir::separator() + "constant" + QDir::separator() + "simCenter"
                            + QDir::separator() + "input" + QDir::separator() + "IsolatedBuildingCFD.json";


    QFile jsonFile(inputFilePath);
    if (!jsonFile.open(QFile::WriteOnly | QFile::Text))
    {
        qDebug() << "Cannot find the path: " << inputFilePath;
    }

    QJsonObject jsonObject;

    outputToJSON(jsonObject);

    QJsonDocument jsonDoc = QJsonDocument(jsonObject);

    jsonFile.write(jsonDoc.toJson());

    jsonFile.close();
}


void IsolatedBuildingCFD::writeOpenFoamFiles()
{

    updateJSON();

    //Run python script to prepare case directory
    QString scriptPath = pyScriptsPath() + "/setup_case.py";
    QString jsonPath = caseDir() + QDir::separator() + "constant" + QDir::separator() + "simCenter" + QDir::separator() + "input";
    QString templatePath = templateDictDir();
    QString outputPath =caseDir();

    QString program = SimCenterPreferences::getInstance()->getPython();
    QStringList arguments;

    arguments << scriptPath << jsonPath << templatePath << outputPath;

    QProcess *process = new QProcess(this);

    process->start(program, arguments);

    process->waitForFinished(-1);

//    QMessageBox msgBox;
//    msgBox.setText(process->readAllStandardOutput() + "\n" + process->readAllStandardError());
//    msgBox.exec();

    process->close();
}



void IsolatedBuildingCFD::readCaseData()
{

    //Write it to JSON becase it is needed for the mesh generation before the final simulation is run.
    //In future only one JSON file in temp.SimCenter directory might be enough
    QString inputFilePath = caseDir() + QDir::separator() + "constant" + QDir::separator() + "simCenter"
                            + QDir::separator() + "input" + QDir::separator() + "IsolatedBuildingCFD.json";


    QFile jsonFile(inputFilePath);
    if (!jsonFile.open(QFile::ReadOnly | QFile::Text))
    {
       qDebug() << "Cannot find the path: " << inputFilePath;
    }


    QString val = jsonFile.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(val.toUtf8());
    QJsonObject jsonObject = doc.object();

    inputFromJSON(jsonObject);

    // close file
    jsonFile.close();

    removeOldFiles();
}


void IsolatedBuildingCFD::originChanged(const QString &arg)
{
    if(arg == "Building Bottom Center")
    {
       originXWidget->setText("0");
       originYWidget->setText("0");
       originZWidget->setText("0");

       originXWidget->setEnabled(false);
       originYWidget->setEnabled(false);
       originZWidget->setEnabled(false);
    }
    else if(arg == "Domain Bottom Left Corner")
    {
       originXWidget->setText(QString::number(-fetchLength()));
       originYWidget->setText(QString::number(-domainWidth()/2.0));
       originZWidget->setText(QString::number(0.0));

       originXWidget->setEnabled(false);
       originYWidget->setEnabled(false);
       originZWidget->setEnabled(false);
    }
    else if(arg == "Custom")
    {
        originXWidget->setEnabled(true);
        originYWidget->setEnabled(true);
        originZWidget->setEnabled(true);
    }
}

void IsolatedBuildingCFD::buildingShapeChanged(const QString &arg)
{
    if(arg == "STL-Surface")
    {
        QDialog *dialog  = new QDialog(this);

        int dialogHeight = 200;
        int dialogWidth = 400;

        dialog->setMinimumHeight(dialogHeight);
        dialog->setMinimumWidth(dialogWidth);
        dialog->setWindowTitle("Import STL Surface");


        QWidget* samplePointsWidget = new QWidget();


        QGridLayout* dialogLayout = new QGridLayout();


        QLabel *importSTL = new QLabel("Yes ");


        QPushButton *okButton = new QPushButton("OK");
        QPushButton *cancelButton = new QPushButton("Canacel");
        QPushButton *saveButton = new QPushButton("Save");


        dialogLayout->addWidget(importSTL, 0, 0);
        dialogLayout->addWidget(okButton, 1, 0);
        dialogLayout->addWidget(cancelButton, 1, 1);
        dialogLayout->addWidget(okButton, 1, 2);
//        dialogLayout->addWidget(IuPlot, 0, 1);
//        dialogLayout->addWidget(LuPlot, 0, 2);
//        dialogLayout->addWidget(SuPlot, 1, 0, 1, 2);

//        dialogLayout->addWidget(samplePointsWidget, 0, 0);

        dialog->setLayout(dialogLayout);
        dialog->exec();





//        buildingDepthWidget->hide();
//        buildingWidthWidget->hide();
//        buildingHeightWidget->hide();

//        buildingWidthLabel->hide();
//        buildingDepthLabel->hide();
//        buildingHeightLabel->hide();
    }

}


void IsolatedBuildingCFD::onShowResultsClicked()
{

//    //Run prepare case directory
//    QString scriptPath = pyScriptsPath() + "/postProcessing/process_output_data.py";
//    QString outputPath = caseDir();

//    QString program = SimCenterPreferences::getInstance()->getPython();
//    QStringList arguments;

//    arguments << scriptPath << outputPath;

//    QProcess *process = new QProcess(this);

//    process->start(program, arguments);

//    process->waitForFinished(-1);

//    QMessageBox msgBox;
//    msgBox.setText(process->readAllStandardOutput() + "\n" + process->readAllStandardError());
//    msgBox.exec();

//    process->close();



    QDialog *dialog  = new QDialog(this);

    int dialogHeight = 800;
    int dialogWidth = 1000;

    dialog->setMinimumHeight(dialogHeight);
    dialog->setMinimumWidth(dialogWidth);
    dialog->setWindowTitle("CFD Results");


    QWidget* samplePointsWidget = new QWidget();


    QGridLayout* dialogLayout = new QGridLayout();


    // generate some data:

    QString profName  = caseDir() + "/constant/simCenter/output/windProfiles.txt";
    QVector<QVector<double>> windProfile  =  readTxtData(profName) ;

    double H = buildingHeight()/geometricScale();

    QPen pen;
    pen.setColor(QColor(0,0,0));
    pen.setWidth(2);
//    pen.setStyle(Qt::NoPen);
    pen.setJoinStyle(Qt::RoundJoin);


    QCustomPlot* UavPlot  = new QCustomPlot();
    UavPlot->addGraph();
    UavPlot->graph()->setPen(pen);
    UavPlot->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));
    UavPlot->graph(0)->setData(windProfile[1], windProfile[0]);
    UavPlot->graph()->setLineStyle((QCPGraph::LineStyle)10);

    // give the axes some labels:
    UavPlot->xAxis->setLabel("Uav[m/s]");
    UavPlot->yAxis->setLabel("z[m]");
    // set axes ranges, so we see all data:
    UavPlot->xAxis->setRange(0, 15);
    UavPlot->yAxis->setRange(0, 1.0);
    UavPlot->replot();

    QCustomPlot* IuPlot  = new QCustomPlot();
    IuPlot->addGraph();
    IuPlot->graph()->setPen(pen);
    IuPlot->graph()->setLineStyle((QCPGraph::LineStyle)10);
    IuPlot->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));
    IuPlot->graph(0)->setData(windProfile[2], windProfile[0]);
    // give the axes some labels:
    IuPlot->xAxis->setLabel("Iu");
    IuPlot->yAxis->setLabel("z[m]");
    // set axes ranges, so we see all data:
    IuPlot->xAxis->setRange(0, 0.5);
    IuPlot->yAxis->setRange(0, 1.0);
    IuPlot->replot();


    QCustomPlot* LuPlot  = new QCustomPlot();
    LuPlot->addGraph();
    LuPlot->graph()->setPen(pen);
    LuPlot->graph()->setLineStyle((QCPGraph::LineStyle)10);
    LuPlot->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));
    LuPlot->graph(0)->setData(windProfile[3], windProfile[0]);
    // give the axes some labels:
    LuPlot->xAxis->setLabel("Lu[m]");
    LuPlot->yAxis->setLabel("z[m]");
    // set axes ranges, so we see all data:
    LuPlot->xAxis->setRange(0, 2.0);
    LuPlot->yAxis->setRange(0, 1.0);
    LuPlot->replot();


    QString SuName  = caseDir() + "/constant/simCenter/output/Suh.txt";
    QVector<QVector<double>> Suh  =  readTxtData(SuName) ;

    QCustomPlot* SuPlot  = new QCustomPlot();


    SuPlot->addGraph();
//    SuPlot->plotLayout()->addElement(0,0, new QCPPlotTitle(plot,"TITEL"));
    SuPlot->graph()->setPen(pen);
    SuPlot->graph()->setLineStyle((QCPGraph::LineStyle)10);
    SuPlot->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));
    SuPlot->graph(0)->setData(Suh[0], Suh[1]);
    SuPlot->yAxis->setLabel("Su[m^2/s]");
    SuPlot->xAxis->setLabel("f[Hz]");
    SuPlot->xAxis->setRange(0.1, 1000.0);
    SuPlot->yAxis->setRange(0.0001, 10.0);
    SuPlot->yAxis->setScaleType(QCPAxis::stLogarithmic);
    SuPlot->xAxis->setScaleType(QCPAxis::stLogarithmic);
    QSharedPointer<QCPAxisTickerLog> logTicker(new QCPAxisTickerLog);
    SuPlot->yAxis->setTicker(logTicker);
    SuPlot->xAxis->setTicker(logTicker);
    SuPlot->replot();



    dialogLayout->addWidget(UavPlot, 0, 0);
    dialogLayout->addWidget(IuPlot, 0, 1);
    dialogLayout->addWidget(LuPlot, 0, 2);
    dialogLayout->addWidget(SuPlot, 1, 0, 1, 2);

    dialogLayout->addWidget(samplePointsWidget, 0, 0);

    dialog->setLayout(dialogLayout);
    dialog->exec();
}

void IsolatedBuildingCFD::onBrowseCaseDirectoryButtonClicked(void)
{
    QString fileName = QFileDialog::getExistingDirectory(this, tr("Open Directory"), caseDir(),
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);


    caseDirectoryPathWidget->setText(fileName);

    if(!isCaseConfigured())
    {
        setupCase();
    }

    if (!isMeshed())
    {
        snappyHexMesh->onRunBlockMeshClicked();
    }
}


void IsolatedBuildingCFD::clear(void)
{

}

void IsolatedBuildingCFD::useCOSTOptionChecked(int state)
{
    //Works fine when Height > Width
    if (useCOSTDimWidget->isChecked())
    {
        domainLengthWidget->setText(QString::number(getNormDim(20.0*buildingHeight())));
        domainWidthWidget->setText(QString::number(getNormDim(10.0*buildingHeight())));
        domainHeightWidget->setText(QString::number(getNormDim(6.0*buildingHeight())));
        fetchLengthWidget->setText(QString::number(getNormDim(5.0*buildingHeight())));
    }

}

void IsolatedBuildingCFD::updateWidgets()
{
    numericalSetup->updateWidgets();
}

bool IsolatedBuildingCFD::inputFromJSON(QJsonObject &jsonObject)
{
    this->clear();

    normalizationTypeWidget->setCurrentText(jsonObject["normalizationType"].toString());
    geometricScaleWidget->setText(QString::number(jsonObject["geometricScale"].toDouble()));
    caseDirectoryPathWidget->setText(jsonObject["caseDirectoryPath"].toString());

    buildingWidthWidget->setText(QString::number(jsonObject["buildingWidth"].toDouble()));
    buildingDepthWidget->setText(QString::number(jsonObject["buildingDepth"].toDouble()));
    buildingHeightWidget->setText(QString::number(jsonObject["buildingHeight"].toDouble()));

    windDirectionWidget->setValue(jsonObject["windDirection"].toInt());

    domainLengthWidget->setText(QString::number(jsonObject["domainLength"].toDouble()));
    domainWidthWidget->setText(QString::number(jsonObject["domainWidth"].toDouble()));
    domainHeightWidget->setText(QString::number(jsonObject["domainHeight"].toDouble()));
    fetchLengthWidget->setText(QString::number(jsonObject["fetchLength"].toDouble()));
    useCOSTDimWidget->setChecked(jsonObject["useCOST"].toBool());

    QJsonArray originPoint  = jsonObject["origin"].toArray();

    originXWidget->setText(QString::number(originPoint[0].toDouble()));
    originYWidget->setText(QString::number(originPoint[1].toDouble()));
    originZWidget->setText(QString::number(originPoint[2].toDouble()));

    originOptions->setCurrentText(jsonObject["originOption"].toString());
    openFoamVersion->setCurrentText(jsonObject["OpenFoamVersion"].toString());

    snappyHexMesh->inputFromJSON(jsonObject);
    windCharacteristics->inputFromJSON(jsonObject);
    boundaryConditions->inputFromJSON(jsonObject);
    turbulenceModeling->inputFromJSON(jsonObject);
    numericalSetup->inputFromJSON(jsonObject);
    resultMonitoring->inputFromJSON(jsonObject);

    return true;
}

bool IsolatedBuildingCFD::outputToJSON(QJsonObject &jsonObject)
{

    jsonObject["EventClassification"] = "Wind";
    jsonObject["type"] = "IsolatedBuildingCFD";

    jsonObject["normalizationType"] = normalizationTypeWidget->currentText();
    jsonObject["geometricScale"]  = geometricScaleWidget->text().toDouble();
    jsonObject["caseDirectoryPath"] = caseDirectoryPathWidget->text();

    jsonObject["buildingWidth"] = buildingWidthWidget->text().toDouble();
    jsonObject["buildingDepth"] = buildingDepthWidget->text().toDouble();
    jsonObject["buildingHeight"] = buildingHeightWidget->text().toDouble();

    jsonObject["windDirection"] = windDirectionWidget->value();

    jsonObject["domainLength"] = domainLengthWidget->text().toDouble();
    jsonObject["domainWidth"] = domainWidthWidget->text().toDouble();
    jsonObject["domainHeight"] = domainHeightWidget->text().toDouble();
    jsonObject["fetchLength"] = fetchLengthWidget->text().toDouble();
    jsonObject["useCOST"] = useCOSTDimWidget->isChecked();

    jsonObject["massUnit"] = massUnit->currentText();
    jsonObject["lengthUnit"] = lengthUnit->currentText();
    jsonObject["timeUnit"] = timeUnit->currentText();
    jsonObject["angleUnit"] = angleUnit->currentText();

    QJsonArray originPoint;
    originPoint.append(coordSysOrigin()[0]);
    originPoint.append(coordSysOrigin()[1]);
    originPoint.append(coordSysOrigin()[2]);

    jsonObject["origin"] = originPoint;
    jsonObject["originOption"] = originOptions->currentText();
    jsonObject["OpenFoamVersion"] = openFoamVersion->currentText();

    snappyHexMesh->outputToJSON(jsonObject);
    windCharacteristics->outputToJSON(jsonObject);
    boundaryConditions->outputToJSON(jsonObject);
    turbulenceModeling->outputToJSON(jsonObject);
    numericalSetup->outputToJSON(jsonObject);
    resultMonitoring->outputToJSON(jsonObject);

    return true;
}


bool IsolatedBuildingCFD::outputAppDataToJSON(QJsonObject &jsonObject) {

    //
    // per API, need to add name of application to be called in AppLication
    // and all data to be used in ApplicationDate
    //

    jsonObject["EventClassification"]="Wind";
    jsonObject["Application"] = "IsolatedBuildingCFD";
    QJsonObject dataObj;
    jsonObject["ApplicationData"] = dataObj;

    return true;
}


bool IsolatedBuildingCFD::inputAppDataFromJSON(QJsonObject &jsonObject) {

    Q_UNUSED(jsonObject);
    return true;
}


bool IsolatedBuildingCFD::copyFiles(QString &destDir) {

     writeOpenFoamFiles();

     QString caseName = "IsolatedBuildingCFD";

     bool result = this->copyPath(caseDir(), destDir + QDir::separator() + caseName, false);

     if (result == false) {
         QString errorMessage; errorMessage = "IsolatedBuildingCFD - failed to copy file: " + caseDir() + " to: " + destDir;
         emit sendFatalMessage(errorMessage);
         qDebug() << errorMessage;
     }

     return result;
 }

bool IsolatedBuildingCFD::cleanCase()
{
    QDir zeroDir(caseDir() + QDir::separator() + "0");
    QDir constDir(caseDir() + QDir::separator() + "constant");
    QDir systemDir(caseDir() + QDir::separator() + "system");

    zeroDir.removeRecursively();
    constDir.removeRecursively();
    systemDir.removeRecursively();

    QFile logFile(caseDir() + QDir::separator() + "log.txt");

    logFile.remove();

    return true;
}

bool IsolatedBuildingCFD::removeOldFiles()
{
    //Clean extra files if exist in 0 folder
    QFile nSurfaceLayersFile(caseDir() + QDir::separator() + "0" + QDir::separator() + "nSurfaceLayers");
    QFile pointLevelFile(caseDir() + QDir::separator() + "0" + QDir::separator() + "pointLevel");
    QFile thicknessFile(caseDir() + QDir::separator() + "0" + QDir::separator() + "thickness");
    QFile thicknessFractionFile(caseDir() + QDir::separator() + "0" + QDir::separator() + "thicknessFraction");
    QFile cellLevelFile(caseDir() + QDir::separator() + "0" + QDir::separator() + "cellLevel");

    nSurfaceLayersFile.remove();
    pointLevelFile.remove();
    thicknessFile.remove();
    thicknessFractionFile.remove();
    cellLevelFile.remove();

    return true;
}

bool IsolatedBuildingCFD::setupCase()
{
    cleanCase();

    QDir targetDir(caseDir());

    if (!targetDir.exists())
    {
        targetDir.mkpath(caseDir());
    }

    targetDir.mkpath("0");
    targetDir.mkpath("constant");
    targetDir.mkpath("constant/geometry");
    targetDir.mkpath("constant/simCenter");
    targetDir.mkpath("constant/simCenter/output");
    targetDir.mkpath("constant/simCenter/input");
    targetDir.mkpath("constant/boundaryData");
    targetDir.mkpath("constant/boundaryData/windProfile");
    targetDir.mkpath("constant/boundaryData/sampledData");
    targetDir.mkpath("system");

    QFile visFoam(caseDir() + "/vis.foam");
    visFoam.open(QIODevice::WriteOnly);

    //Write dictionary files
    writeOpenFoamFiles();


    snappyHexMesh->snappyHexMeshCompleted = false;

    return true;
}


QVector<QVector<double>> IsolatedBuildingCFD::readTxtData(QString fileName)
{
    QVector<QVector<double>>  data;

    int colCount  = 0;

    QFile inputFileTest(fileName);

    if (inputFileTest.open(QIODevice::ReadOnly))
    {
       QTextStream in(&inputFileTest);


       while (!in.atEnd())
       {
            QString line = in.readLine();

            QStringList  fields = line.split(" ");

            colCount  = fields.size();
            break;
       }
       inputFileTest.close();
    }

    for (int i=0; i < colCount; i++)
    {
        QVector<double> row;
        data.append(row);
    }

    int count  = 0;

    QFile inputFile(fileName);
    if (inputFile.open(QIODevice::ReadOnly))
    {
       QTextStream in(&inputFile);

       while (!in.atEnd())
       {
            QString line = in.readLine();

            QStringList  fields = line.split(" ");

            for (int i=0; i < colCount; i++)
            {
                data[i].append(fields[i].toDouble());
            }
       }
       inputFile.close();
    }

    return data;
}


bool IsolatedBuildingCFD::isCaseConfigured()
{
    QDir zeroDir(caseDir() + QDir::separator() +  "0");
    QDir constDir(caseDir() + QDir::separator() + "constant");
    QDir systemDir(caseDir() + QDir::separator() + "system");
    QFile contrlDict(caseDir() + QDir::separator() + "system" + QDir::separator() + "controlDict");
    QFile blockDict(caseDir() + QDir::separator() + "system" +  QDir::separator() + "blockMeshDict");
    QFile snappyDict(caseDir() + QDir::separator() + "system" + QDir::separator() + "snappyHexMeshDict");

    //Better if we check other files too, for now this are enought to run mesh
    return zeroDir.exists() && constDir.exists() && systemDir.exists() &&
           contrlDict.exists() && blockDict.exists() && snappyDict.exists();
}

bool IsolatedBuildingCFD::isMeshed()
{
    QFile pointsFile(caseDir() + QDir::separator() + "constant" + QDir::separator() + "polyMesh" + QDir::separator() + "points");

    //Better if we check other files too, for now this are enought to run mesh
    return isCaseConfigured() && pointsFile.exists();
}

bool IsolatedBuildingCFD::isSnappyHexMeshCompleted()
{
    return snappyHexMesh->snappyHexMeshCompleted;
}

double IsolatedBuildingCFD::domainLength()
{
    return domainLengthWidget->text().toDouble();
}

double IsolatedBuildingCFD::domainWidth()
{
    return domainWidthWidget->text().toDouble();
}

double IsolatedBuildingCFD::domainHeight()
{
    return domainHeightWidget->text().toDouble();
}

double IsolatedBuildingCFD::fetchLength()
{
    return fetchLengthWidget->text().toDouble();
}

double IsolatedBuildingCFD::buildingWidth()
{
    return buildingWidthWidget->text().toDouble();
}

double IsolatedBuildingCFD::buildingDepth()
{
    return buildingDepthWidget->text().toDouble();
}

double IsolatedBuildingCFD::buildingHeight()
{
    return buildingHeightWidget->text().toDouble();
}
int IsolatedBuildingCFD::numberOfFloors()
{
    return resultMonitoring->numStories->value();
}


double IsolatedBuildingCFD::geometricScale()
{
    return geometricScaleWidget->text().toDouble();
}

double IsolatedBuildingCFD::windDirection()
{
    return windDirectionWidget->text().toDouble();
}

QVector<double> IsolatedBuildingCFD::coordSysOrigin()
{
    QVector<double> origin = {originXWidget->text().toDouble(), originYWidget->text().toDouble(), originZWidget->text().toDouble()};

    return origin;
}

QString IsolatedBuildingCFD::normalizationType()
{
    return normalizationTypeWidget->currentText();
}

QString IsolatedBuildingCFD::caseDir()
{
    return caseDirectoryPathWidget->text();
}

QString IsolatedBuildingCFD::pyScriptsPath()
{
    QString backendAppDir = SimCenterPreferences::getInstance()->getAppDir() + QDir::separator()
             + QString("applications") + QDir::separator() + QString("createEVENT") + QDir::separator()
             + QString("IsolatedBuildingCFD");

    return backendAppDir;
}

QString IsolatedBuildingCFD::templateDictDir()
{
    QString templateDictsDir = SimCenterPreferences::getInstance()->getAppDir() + QDir::separator()
             + QString("applications") + QDir::separator() + QString("createEVENT") + QDir::separator()
             + QString("IsolatedBuildingCFD") + QDir::separator() + QString("templateOF10Dicts");

    return templateDictsDir;
}

QString IsolatedBuildingCFD::simulationType()
{
    return turbulenceModeling->simulationType();
}

void IsolatedBuildingCFD::reloadMesh()
{
    if (isInitialize())
    {
        visWidget->onReloadCaseClicked();
    }
}

bool IsolatedBuildingCFD::isInitialize()
{
    return caseInitialized;
}

double IsolatedBuildingCFD::convertToMeter(double dim, QString unit)
{
    // Converts to meter from other unit system;
    if (unit == "m")
    {
       return dim;
    }
    else if (unit == "in")
    {
       return dim*0.0254;
    }
    else if (unit == "cm")
    {
       return dim*100.0;
    }
    else if (unit == "mm")
    {
       return dim*1000.0;
    }
    else if (unit == "ft")
    {
       return dim*0.3048;
    }
    else
       qDebug() << "Unit system not recognized";
}

double IsolatedBuildingCFD::convertFromMeter(double dim, QString unit)
{
    //Convert from meters to other units
    if (unit == "m")
    {
       return dim;
    }
    else if (unit == "in")
    {
       return dim/0.0254;
    }
    else if (unit == "cm")
    {
       return dim/100.0;
    }
    else if (unit == "mm")
    {
       return dim/1000.0;
    }
    else if (unit == "ft")
    {
       return dim/0.3048;
    }
    else
       qDebug() << "Unit system not recognized";
}

double IsolatedBuildingCFD::getNormDim(double dim)
{
    if(normalizationTypeWidget->currentText() == "Relative")
    {
       return dim/buildingHeight();
    }

    return dim/geometricScale();
}


double IsolatedBuildingCFD::getTimeStep()
{
    double meshSize = 0.0;

    if(snappyHexMesh->addPrismLayers->isChecked())
    {
       meshSize = snappyHexMesh->prismLayerMeshSize->text().toDouble();
    }
    else if(snappyHexMesh->addEdgeRefinement->isChecked())
    {
        meshSize = snappyHexMesh->edgeRefinementMeshSize->text().toDouble();
    }
    else if(snappyHexMesh->addSurfaceRefinement->isChecked())
    {
        meshSize = snappyHexMesh->surfaceRefinementMeshSize->text().toDouble();
    }
    else
    {
        meshSize = snappyHexMesh->refinementBoxesTable->item(snappyHexMesh->refinementBoxesTable->rowCount()-1, 8)->text().toDouble();
    }

    meshSize = meshSize*buildingHeight()/geometricScale();


    double maxCo = numericalSetup->maxCourantNumber->value();
    double U = windCharacteristics->referenceWindSpeed->text().toDouble();

    return maxCo*meshSize/U;
}



