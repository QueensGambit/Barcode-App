/*
 * SettingObject.cpp
 *
 *  Created on: 18.01.2016
 *      Author: Anonym
 */

#include "SettingObject.h"

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

SettingObject::~SettingObject() {
	// TODO Auto-generated destructor stub
}

bool SettingObject::isSpeach() const {
	return speach;
}
