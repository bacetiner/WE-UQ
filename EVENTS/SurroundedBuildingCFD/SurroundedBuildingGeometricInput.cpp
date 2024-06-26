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

// Written by: Abiy

#include "SurroundedBuildingGeometricInput.h"
#include "SurroundedBuildingCFD.h"
#include <QPushButton>
#include <QScrollArea>
#include <QJsonArray>
#include <QJsonObject>
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
#include <QMessageBox>
#include <QComboBox>
#include <QSpinBox>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QVector>
#include <LineEditRV.h>
#include <QProcess>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDialog>
#include <SimCenterPreferences.h>
#include <GeneralInformationWidget.h>

//#include <InputWidgetParameters.h>

 SurroundedBuildingGeometricInput:: SurroundedBuildingGeometricInput(SurroundedBuildingCFD *parent)
    : SimCenterAppWidget(parent), mainModel(parent)
{
    layout = new QVBoxLayout();

    buildingAndDomainInformationGroup = new QWidget();
    buildingAndDomainInformationLayout = new QGridLayout();

    buildingInformationGroup = new QGroupBox("Building Dimension and Orientation");
    buildingInformationLayout = new QGridLayout();

    domainInformationGroup = new QGroupBox("Domain Size");
    domainInformationLayout = new QGridLayout();

    surroundingBuildingsGroup = new QGroupBox("Surrounding Buildings Information");
    surroundingBuildingsLayout = new QGridLayout();

    coordinateSystemGroup = new QGroupBox("Coordinate System");
    coordinateSystemLayout = new QGridLayout();

    QLabel *geometricScaleLabel = new QLabel("Geometric Scale:");
    geometricScaleWidget = new QLineEdit();
    geometricScaleWidget->setText("400.0");


    theBuildingButton = new QPushButton();
    QPixmap pixmapFlat(":/Resources/SurroundedBuildingCFD/buildingGeometry.png");

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

    QLabel *windDirectionLabel = new QLabel("Wind Direction:");
    windDirectionWidget = new QSpinBox;
    windDirectionWidget->setRange(0, 90);
    windDirectionWidget->setSingleStep(10);
    windDirectionWidget->setValue(0);

    QLabel *domainLengthLabel = new QLabel("Domain Length (X-axis):");
    domainLengthWidget = new QLineEdit();
    domainLengthWidget->setText(QString::number(getNormDim(20.0*buildingHeightWidget->text().toDouble())));

    QLabel *domainWidthLabel = new QLabel("Domain Width (Y-axis):");
    domainWidthWidget = new QLineEdit();
    domainWidthWidget->setText(QString::number(getNormDim(10.0*buildingHeightWidget->text().toDouble())));

    QLabel *domainHeightLabel = new QLabel("Domain Height (Z-axis):");
    domainHeightWidget = new QLineEdit();
    domainHeightWidget->setText(QString::number(getNormDim(5.0*buildingHeightWidget->text().toDouble())));

    QLabel *fetchLengthLabel = new QLabel("Fetch Length (X-axis):");
    fetchLengthWidget = new QLineEdit();
    fetchLengthWidget->setText(QString::number(getNormDim(5.0*buildingHeightWidget->text().toDouble())));

    QLabel *useCOSTDimLabel = new QLabel("COST Recommendation:");
    useCOSTDimWidget = new QCheckBox();
    useCOSTDimWidget->setChecked(true);


    //------------------------------------
    // Surrounding Building Information
    //------------------------------------

    theSurroundingsButton = new QPushButton();
    QPixmap surrPixmap(":/Resources/SurroundedBuildingCFD/surroundingBuildingsGeometry.png");

    theSurroundingsButton->setIcon(surrPixmap);
    theSurroundingsButton->setIconSize(surrPixmap.rect().size()*0.20);
    theSurroundingsButton->setFixedSize(surrPixmap.rect().size()*0.20);
    surroundingBuildingsLayout->addWidget(theSurroundingsButton, 0, 0, 7, 1, Qt::AlignVCenter);


    QLabel *surroundingBuildingsWidthLabel = new QLabel("Surrounding Buildings Width:");
    surroundingBuildingsWidthWidget = new QLineEdit();
    surroundingBuildingsWidthWidget->setText("30");

    QLabel *surroundingBuildingsDepthLabel = new QLabel("Surrounding Buildings Depth:");
    surroundingBuildingsDepthWidget = new QLineEdit();
    surroundingBuildingsDepthWidget->setText("30");

    QLabel *surroundingBuildingsHeightLabel = new QLabel("Surrounding Buildings Height:");
    surroundingBuildingsHeightWidget = new QLineEdit();
    surroundingBuildingsHeightWidget->setText("75");


    QLabel *streetWidthXLabel = new QLabel("Average Street Width (X-axis):");
    streetWidthXWidget = new QLineEdit();
    streetWidthXWidget->setText("30");

    QLabel *streetWidthYLabel = new QLabel("Average Street Width (Y-axis):");
    streetWidthYWidget = new QLineEdit();
    streetWidthYWidget->setText("30");


    QLabel *boundingRadiusLabel = new QLabel("Approximate Bounding Radius:");
    boundingRadiusWidget = new QLineEdit();
    boundingRadiusWidget->setText("500");

    QLabel *randomnessLabel = new QLabel("Building Height Randomness(%):");
    randomnessWidget = new QSpinBox;
    randomnessWidget->setRange(0, 100);
    randomnessWidget->setSingleStep(10);
    randomnessWidget->setValue(0);


    generateSurroundings = new QPushButton("Generate Surroundings");


    surroundingBuildingsLayout->addWidget(surroundingBuildingsWidthLabel, 0, 1);
    surroundingBuildingsLayout->addWidget(surroundingBuildingsWidthWidget, 0, 2);
    surroundingBuildingsLayout->addWidget(surroundingBuildingsDepthLabel, 1, 1);
    surroundingBuildingsLayout->addWidget(surroundingBuildingsDepthWidget, 1, 2);
    surroundingBuildingsLayout->addWidget(surroundingBuildingsHeightLabel, 2, 1);
    surroundingBuildingsLayout->addWidget(surroundingBuildingsHeightWidget, 2, 2);
    surroundingBuildingsLayout->addWidget(streetWidthXLabel, 3, 1);
    surroundingBuildingsLayout->addWidget(streetWidthXWidget, 3, 2);
    surroundingBuildingsLayout->addWidget(streetWidthYLabel, 4, 1);
    surroundingBuildingsLayout->addWidget(streetWidthYWidget, 4, 2);
    surroundingBuildingsLayout->addWidget(boundingRadiusLabel, 5, 1);
    surroundingBuildingsLayout->addWidget(boundingRadiusWidget, 5, 2);
    surroundingBuildingsLayout->addWidget(randomnessLabel, 6, 1);
    surroundingBuildingsLayout->addWidget(randomnessWidget, 6, 2);

    surroundingBuildingsLayout->addWidget(generateSurroundings,7,1,1,2);


    QLabel *originOptionsLabel = new QLabel("Absolute Origin: ");
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


    buildingInformationLayout->addWidget(geometricScaleLabel, 0, 1);
    buildingInformationLayout->addWidget(geometricScaleWidget, 0, 2);

    buildingInformationLayout->addWidget(buildingWidthLabel, 1,1);
    buildingInformationLayout->addWidget(buildingWidthWidget, 1,2);

    buildingInformationLayout->addWidget(buildingDepthLabel, 2,1);
    buildingInformationLayout->addWidget(buildingDepthWidget, 2,2);

    buildingInformationLayout->addWidget(buildingHeightLabel, 3,1);
    buildingInformationLayout->addWidget(buildingHeightWidget, 3,2);

    buildingInformationLayout->addWidget(windDirectionLabel, 4, 1);
    buildingInformationLayout->addWidget(windDirectionWidget, 4, 2);


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


    coordinateSystemLayout->addWidget(originOptionsLabel,0,0);
    coordinateSystemLayout->addWidget(originOptions,0,1);
    coordinateSystemLayout->addWidget(originCoordinateLabel,1,0, Qt::AlignRight);
    coordinateSystemLayout->addWidget(originXLabel,1,1, Qt::AlignRight);
    coordinateSystemLayout->addWidget(originXWidget,1,2, Qt::AlignLeft);
    coordinateSystemLayout->addWidget(originYLabel,1,3, Qt::AlignLeft);
    coordinateSystemLayout->addWidget(originYWidget,1,4, Qt::AlignLeft);
    coordinateSystemLayout->addWidget(originZLabel,1,5, Qt::AlignLeft);
    coordinateSystemLayout->addWidget(originZWidget,1,6, Qt::AlignLeft);

    buildingInformationGroup->setLayout(buildingInformationLayout);
    domainInformationGroup->setLayout(domainInformationLayout);
    buildingAndDomainInformationGroup->setLayout(buildingAndDomainInformationLayout);
    surroundingBuildingsGroup->setLayout(surroundingBuildingsLayout);
    coordinateSystemGroup->setLayout(coordinateSystemLayout);

    connect(originOptions, SIGNAL(currentTextChanged(QString)), this, SLOT(originChanged(QString)));
    connect(useCOSTDimWidget, SIGNAL(stateChanged(int)), this, SLOT(useCOSTOptionChecked(int)));
    connect(generateSurroundings, SIGNAL(clicked()), this, SLOT(onGenerateSurroundings()));



    //Disable editing in the event section
    buildingWidthWidget->setEnabled(false);
    buildingHeightWidget->setEnabled(false);
    buildingDepthWidget->setEnabled(false);


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

    layout->addWidget(buildingAndDomainInformationGroup);
    layout->addWidget(surroundingBuildingsGroup);
    //    layout->addWidget(coordinateSystemGroup);
    layout->addStretch();

    this->setLayout(layout);
}


SurroundedBuildingGeometricInput::~ SurroundedBuildingGeometricInput()
{

}

void  SurroundedBuildingGeometricInput::clear(void)
{

}


bool  SurroundedBuildingGeometricInput::outputToJSON(QJsonObject &jsonObject)
{
    // Writes wind characterstics (flow properties) to JSON file.
    QJsonObject geometricDataJson = QJsonObject();

//    geometricDataJson["normalizationType"] = normalizationTypeWidget->currentText();
    geometricDataJson["geometricScale"]  = geometricScaleWidget->text().toDouble();

//    geometricDataJson["buildingShape"] = buildingShape->currentText();
//    geometricDataJson["importedSTLPath"] = importedSTLPath->text();
//    geometricDataJson["stlScaleFactor"] = stlScaleFactor->text().toDouble();
//    geometricDataJson["recenterToOrigin"] = recenterToOrigin->isChecked();
//    geometricDataJson["accountWindDirection"] = accountWindDirection->isChecked();
//    geometricDataJson["useSTLDimensions"] = useSTLDimensions->isChecked();

    geometricDataJson["buildingWidth"] = buildingWidthWidget->text().toDouble();
    geometricDataJson["buildingDepth"] = buildingDepthWidget->text().toDouble();
    geometricDataJson["buildingHeight"] = buildingHeightWidget->text().toDouble();
    geometricDataJson["windDirection"] = windDirectionWidget->value();

    QJsonObject surroundingsData = QJsonObject();
    surroundingsData["surroundingBuildingsWidth"] = surroundingBuildingsWidthWidget->text().toDouble();
    surroundingsData["surroundingBuildingsDepth"] = surroundingBuildingsDepthWidget->text().toDouble();
    surroundingsData["surroundingBuildingsHeight"] = surroundingBuildingsHeightWidget->text().toDouble();
    surroundingsData["streetWidthX"] = streetWidthXWidget->text().toDouble();
    surroundingsData["streetWidthY"] = streetWidthYWidget->text().toDouble();
    surroundingsData["boundingRadius"] = boundingRadiusWidget->text().toDouble();
    surroundingsData["randomness"] = randomnessWidget->value();
    geometricDataJson["surroundingBuildingsInformation"] = surroundingsData;

    geometricDataJson["domainLength"] = domainLengthWidget->text().toDouble();
    geometricDataJson["domainWidth"] = domainWidthWidget->text().toDouble();
    geometricDataJson["domainHeight"] = domainHeightWidget->text().toDouble();
    geometricDataJson["fetchLength"] = fetchLengthWidget->text().toDouble();
    geometricDataJson["useCOST"] = useCOSTDimWidget->isChecked();

    QJsonArray originPoint;
    originPoint.append(coordSysOrigin()[0]);
    originPoint.append(coordSysOrigin()[1]);
    originPoint.append(coordSysOrigin()[2]);

    geometricDataJson["origin"] = originPoint;
    geometricDataJson["originOption"] = originOptions->currentText();

    jsonObject["GeometricData"] = geometricDataJson;

    return true;
}

bool  SurroundedBuildingGeometricInput::inputFromJSON(QJsonObject &jsonObject)
{
    // Read geometric information including wind direction from a JSON file.
    //The JSON file is located in caseDir/constant/simCenter

    QJsonObject geometricDataJson = jsonObject["GeometricData"].toObject();

    geometricScaleWidget->setText(QString::number(geometricDataJson["geometricScale"].toDouble()));

    buildingWidthWidget->setText(QString::number(geometricDataJson["buildingWidth"].toDouble()));
    buildingDepthWidget->setText(QString::number(geometricDataJson["buildingDepth"].toDouble()));
    buildingHeightWidget->setText(QString::number(geometricDataJson["buildingHeight"].toDouble()));

    windDirectionWidget->setValue(geometricDataJson["windDirection"].toInt());

    QJsonObject surroundingsData = geometricDataJson["surroundingBuildingsInformation"].toObject();
    surroundingBuildingsWidthWidget->setText(QString::number(surroundingsData["surroundingBuildingsWidth"].toDouble()));
    surroundingBuildingsDepthWidget->setText(QString::number(surroundingsData["surroundingBuildingsDepth"].toDouble()));
    surroundingBuildingsHeightWidget->setText(QString::number(surroundingsData["surroundingBuildingsHeight"].toDouble()));
    streetWidthXWidget->setText(QString::number(surroundingsData["streetWidthX"].toDouble()));
    streetWidthXWidget->setText(QString::number(surroundingsData["streetWidthY"].toDouble()));
    boundingRadiusWidget->setText(QString::number(surroundingsData["boundingRadius"].toDouble()));
    randomnessWidget->setValue(surroundingsData["randomness"].toInt());

    domainLengthWidget->setText(QString::number(geometricDataJson["domainLength"].toDouble()));
    domainWidthWidget->setText(QString::number(geometricDataJson["domainWidth"].toDouble()));
    domainHeightWidget->setText(QString::number(geometricDataJson["domainHeight"].toDouble()));
    fetchLengthWidget->setText(QString::number(geometricDataJson["fetchLength"].toDouble()));
    useCOSTDimWidget->setChecked(geometricDataJson["useCOST"].toBool());

    QJsonArray originPoint  = geometricDataJson["origin"].toArray();

    originXWidget->setText(QString::number(originPoint[0].toDouble()));
    originYWidget->setText(QString::number(originPoint[1].toDouble()));
    originZWidget->setText(QString::number(originPoint[2].toDouble()));

    originOptions->setCurrentText(geometricDataJson["originOption"].toString());

    return true;
}

void  SurroundedBuildingGeometricInput::updateWidgets()
{
//    onVelocityScaleChanged();
}

void  SurroundedBuildingGeometricInput::onGenerateSurroundings()
{
    mainModel->writeOpenFoamFiles();
    mainModel->reloadMesh();
}

QVector<double> SurroundedBuildingGeometricInput::coordSysOrigin()
{
    QVector<double> origin = {originXWidget->text().toDouble(), originYWidget->text().toDouble(), originZWidget->text().toDouble()};

    return origin;
}

void SurroundedBuildingGeometricInput::useCOSTOptionChecked(int state)
{
    //Works fine when Height > Width i.e., high rise buildings
    if (useCOSTDimWidget->isChecked())
    {
        domainLengthWidget->setText(QString::number(getNormDim(20.0*buildingHeightWidget->text().toDouble())));
        domainWidthWidget->setText(QString::number(getNormDim(10.0*buildingHeightWidget->text().toDouble())));
        domainHeightWidget->setText(QString::number(getNormDim(6.0*buildingHeightWidget->text().toDouble())));
        fetchLengthWidget->setText(QString::number(getNormDim(5.0*buildingHeightWidget->text().toDouble())));
    }

}

void SurroundedBuildingGeometricInput::originChanged(const QString &arg)
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
        originXWidget->setText(QString::number(-fetchLengthWidget->text().toDouble()));
        originYWidget->setText(QString::number(-domainWidthWidget->text().toDouble()/2.0));
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


double SurroundedBuildingGeometricInput::getNormDim(double dim)
{
    return dim/geometricScaleWidget->text().toDouble();
}


double SurroundedBuildingGeometricInput::convertToMeter(double dim, QString unit)
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

double SurroundedBuildingGeometricInput::convertFromMeter(double dim, QString unit)
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
