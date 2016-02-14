/*
 * SettingObject.cpp
 *
 *  Created on: 18.01.2016
 *      Author: Anonym
 */

#include "SettingObject.h"

/*
 * the only contructor of the SettingObject
 * it initialiazes all important attributes of the object
 * more about commentary the attributes is in the SettingObject.h file
 */
SettingObject::SettingObject(const char* execute, bool stepByStep, bool showAllSteps,
		bool search, bool webcam, bool speach,
	const char* file,
	char webcamVersion, char webcamStyle) {
	this->execute = execute;
	this->stepByStep = stepByStep;
	this->showAllSteps = showAllSteps;
	this->search = search;
	this->webcam = webcam;
	this->speach = speach;
	this->file = file;
	this->webcamVersion = webcamVersion;
	this->webcamStyle = webcamStyle;
}

//getter setter methods:
const char* SettingObject::getFile() const {
	return file;
}

bool SettingObject::isSearch() const {
	return search;
}

bool SettingObject::isShowAllSteps() const {
	return showAllSteps;
}

bool SettingObject::isStepByStep() const {
	return stepByStep;
}

bool SettingObject::isWebcam() const {
	return webcam;
}

char SettingObject::getWebcamStyle() const {
	return webcamStyle;
}

const char* SettingObject::getExecute() const {
	return execute;
}

char SettingObject::getWebcamVersion() const {
	return webcamVersion;
}

/*
 * prints all configured attributes on the console
 */
void SettingObject::printSettings() {
	cout << "---------------------------------------" << endl;
	cout << "Settings: " << endl;
	cout << "execute: " << execute << endl;
	cout << "stepByStep: "  << stepByStep << endl;
	cout << "showAllSteps: "  << showAllSteps << endl;
	cout << "search: "  << search << endl;
	cout << "webcam: "  << webcam << endl;
	cout << "speach: " << speach << endl;
	cout << "file: "  << file << endl;
	cout << "webcamVersion: "  << webcamVersion << endl;
	cout << "webcamStyle: "  << webcamStyle << endl;
	cout << "---------------------------------------" << endl;
}

/*
 * setter function to define or change all attributes manually
 */
void SettingObject::setValues(bool stepByStep, bool showAllSteps, bool search,
		bool webcam, bool speach, const char* file, char webcamVersion, char webcamStyle) {
	this->stepByStep = stepByStep;
	this->showAllSteps = showAllSteps;
	this->search = search;
	this->webcam = webcam;
	this->speach = speach;
	this->file = file;
	this->webcamVersion = webcamVersion;
	this->webcamStyle = webcamStyle;
}

/*
 * default destructor
 */
SettingObject::~SettingObject() {
	// TODO Auto-generated destructor stub
}

//getter setter methods:
bool SettingObject::isSpeach() const {
	return speach;
}

bool SettingObject::isBasic() const {
	return basic;
}

void SettingObject::setBasic(bool basic) {
	this->basic = basic;
	if (basic) {
		this->showAllSteps = false;
		this->stepByStep = false;
	}
}
